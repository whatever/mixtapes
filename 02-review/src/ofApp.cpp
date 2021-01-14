#include "ofApp.h"

#define SAMPLE_RATE 44100
#define FPS 30

//--------------------------------------------------------------
void ofApp::setup(){

  outBuffer = std::vector<float>(0);

  ofSetFrameRate(FPS);

  // Set data root
  string newRoot = "../Resources";
  ofEnableDataPath();
  ofSetDataPathRoot(newRoot);

  auto audioPath = ofFilePath::getCurrentWorkingDirectory();
  audioPath = ofFilePath::join(audioPath, "books-of-war.mp3");
  audioPath = ofFilePath::getAbsolutePath(audioPath);
  std::cout << "Audio path = " << audioPath << "\n";


  // Audio
  audioPlayer.load(audioPath);
  audioPlayer.setLoop(true);
  audioPlayer.setVolume(0.0);
  audioPlayer.play();

  setupMicrophone();

  fbo.allocate(100, 100, GL_RGB);
  setupRecorder();
}

void ofApp::setupRecorder() {

  // Video
  recorder.setVideoCodec("mpeg4");
  recorder.setVideoBitrate("1800k");

  // Audio
  recorder.setAudioCodec("mp3");
  recorder.setAudioBitrate("256k");

  // Handler
  ofAddListener(recorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
  recorder.setup("../../../../recorded.mov", 100, 100, FPS, SAMPLE_RATE, 1);
  recorder.start();
}

void ofApp::audioIn(ofSoundBuffer & input) {
  {
    auto soundBuffer = audioPlayer.getCurrentSoundBufferMono(256);

    for (int i=0; i < soundBuffer.getNumFrames(); i++) {
      std::cout << soundBuffer[i] << " ";
    }

    std::cout << "\n";

    std::cout
      << soundBuffer.getNumFrames() << " " 
      << soundBuffer.getNumChannels() << " "
      << "\n";

    recorder.addAudioSamples(&soundBuffer[0], soundBuffer.getNumFrames(), soundBuffer.getNumChannels());
    buffer = soundBuffer.getBuffer();
  }

}

/*
void ofApp::audioOut(ofSoundBuffer &buff) {

  std::cout << "<<< audioIn = " << buff.size() << "\n";
  // buff.copyTo(lastBuffer, 100, 2);

  for (size_t i = 0; i < buff.getNumFrames(); i++) {
    std::cout << buff.getSample(i, 0) << " ";
	}
  std::cout << "\n";

  lastBuffer = buff;

  std::cout << ">>> audioIn = " << lastBuffer.size() << "\n";
}
*/

void ofApp::drawBars(float y_pos, std::vector<float> &ys, float scale) {

  if (ys.empty()) {
    return;
  }

  float w = ofGetWidth();
  float h = 100;
  int width = (ofGetWidth()) / ys.size();

  for (int i=0; i < ys.size(); i++) {
    float x = i*width;
    float y = y_pos;
    float h = ys[i]*scale;
    ofSetColor(255);
    ofDrawRectangle(x, y, width, h);
  }
}

void ofApp::setupMicrophone() {

  int bufferSize = 256;

  ofSoundStreamSettings settings;
  auto devices = inStream.getMatchingDevices("Microphone");

  std::cout << "Device list:\n";

  for (int i=0; i < devices.size(); i++) {
    auto device = devices[i];
    std::cout << " " << i << ") " << device.name << "\n";
  }

  assert(devices.size() > 0);

  std::cout << "\n\n";

  std::cout << "Using \"" << devices[0].name << "\" as input device...\n";
  settings.setInDevice(devices[0]);

  settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 1;
	settings.bufferSize = bufferSize;

  inStream.setup(settings);
  inStream.stop();
  inStream.start();


  /* //
  {
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 512;
    settings.numBuffers = 4;
    settings.setOutListener(this);
    outSoundStream.setup(settings);
  }
  // */
}

void ofApp::exit(ofEventArgs &args) {
  ofRemoveListener(recorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
  recorder.close();
}

//--------------------------------------------------------------
void ofApp::update() {

  ofSoundUpdate();

  fbo.begin();
    ofClear(255, 0, 0, 255);
    ofSetColor(255, 255, 255);
  fbo.end();

  ofPixels pixels;
  fbo.getTexture().readToPixels(pixels);
  recorder.addFrame(pixels);

  {
    auto soundBuffer = audioPlayer.getCurrentSoundBufferMono(512);

    std::cout
      << soundBuffer.size() << " "
      << soundBuffer.getNumFrames() << " " 
      << soundBuffer.getNumChannels() << " "
      << "\n";

    // recorder.addAudioSamples(&soundBuffer[0], soundBuffer.getNumFrames(), soundBuffer.getNumChannels());
  }


  if (audioPlayer.isLoaded()) {
    outBuffer = audioPlayer.getCurrentBuffer(256);
  }

  // std::cout << recorder.hasAudioError() << ", " << recorder.hasVideoError() << "\n";
  //
}
void ofApp::draw() {
  ofClear(0);
  ofSetColor(255, 0, 0);
  ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

  drawBars(100, buffer, 100.0f);
  drawBars(500, outBuffer, 100.0f);
}

void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args) {
}

void ofApp::keyPressed(int key){ }
void ofApp::keyReleased(int key){ }
void ofApp::mouseMoved(int x, int y ){ }
void ofApp::mouseDragged(int x, int y, int button){ }
void ofApp::mousePressed(int x, int y, int button){ }
void ofApp::mouseReleased(int x, int y, int button){ }
void ofApp::mouseEntered(int x, int y){ }
void ofApp::mouseExited(int x, int y){ }
void ofApp::windowResized(int w, int h){ }
void ofApp::gotMessage(ofMessage msg){ }
void ofApp::dragEvent(ofDragInfo dragInfo){ }
