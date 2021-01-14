#include "ofApp.h"

#define SAMPLE_RATE 44100
#define FPS 30

//--------------------------------------------------------------
void ofApp::setup(){

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
  recorder.setAudioBitrate("192k");

  // Handler
  ofAddListener(recorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
  recorder.setup("../../../../recorded.mov", 100, 100, FPS, SAMPLE_RATE, 1);
  recorder.start();
}

void ofApp::audioIn(ofSoundBuffer & input) {
  {
    buffer = input.getBuffer();
    recorder.addAudioSamples(&input[0], buffer.size(), input.getNumChannels());
  }

  {
    float *sound = ofSoundGetSpectrum(512);
    spectrum = std::vector<float>(512);
    for (int i=0; i < 512; i++) {
      spectrum[i] = sound[i];
    }
  }
}

void ofApp::drawBars(float y_pos, std::vector<float> &ys, float scale) {

  if (ys.empty()) {
    return;
  }

  float w = ofGetWidth();
  float h = 100;
  int width = (ofGetWidth()-100) / buffer.size();

  for (int i=0; i < ys.size(); i++) {
    float x = i*width;
    float y = y_pos;
    float h = ys[i];
    ofSetColor(255);
    ofDrawRectangle(x, y, width-1, h*scale);
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
}

void ofApp::exit(ofEventArgs &args) {
  ofRemoveListener(recorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
  recorder.close();
}

//--------------------------------------------------------------
void ofApp::update(){
  ofSoundUpdate();


  fbo.begin();
    ofClear(255, 0, 0, 255);
    ofSetColor(255, 255, 255);
  fbo.end();

  ofPixels pixels;
  fbo.getTexture().readToPixels(pixels);
  recorder.addFrame(pixels);

  if (audioPlayer.isLoaded()) {
    int bands = 512;
    // float *sound = ofSoundGetSpectrum(bands);
    float sound[512];

    for (int i=0; i < 32; i++) {
      sound[i] = 0.2;
    }
  }

  // std::cout << recorder.hasAudioError() << ", " << recorder.hasVideoError() << "\n";
}
void ofApp::draw(){
  ofClear(0);
  ofSetColor(255, 0, 0);
  ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

  drawBars(100, buffer, 100.0f);
  drawBars(300, spectrum, 100.0f);
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
