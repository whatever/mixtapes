#include "ofApp.h"

#include <chrono>

unsigned int now() {
   using namespace std::chrono;
   return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void ofApp::setup(){

  std::cout << "XYZ\n";
  std::cout << "\n\n\n";

  speed = 1.0f;
  smoothedVol = 3.0f;
  start = now();

  setupMicrophone();


  std::cout << "\n\n";

  string newRoot = "../Resources";
  ofEnableDataPath();
  ofSetDataPathRoot(newRoot);

  auto videoPath = ofFilePath::getCurrentWorkingDirectory();
  videoPath = ofFilePath::join(videoPath, "sea.mp4");
  videoPath = ofFilePath::getAbsolutePath(videoPath);
  std::cout << "Video path = " << videoPath << "\n";

  player.load(videoPath);
  player.play();
}

void ofApp::exit(ofEventArgs &args) {
  inStream.stop();
  inStream.close();
}

void ofApp::setupMicrophone() {

  int bufferSize = 256;
  left.assign(bufferSize, 0.0);
  right.assign(bufferSize, 0.0);
  volHistory.assign(400, 0.0);

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

void ofApp::update() {
  player.update();
  update(getElapsedMillis());
}

void ofApp::update(unsigned int t) {
  float u = ((float) getElapsedMillis()) / 1000.0f / 1.0f;
  float ts = ((float) getElapsedMillis()) / 1000.0f * 33.0f;
  box.resetTransform();
  box.setScale(smoothedVol);
  box.rotateDeg(+1.0f * ts, ofVec3f(1.0f, 0.0f, 0.0f));
  box.rotateDeg(+5.0f * ts, ofVec3f(0.0f, 1.0f, 0.0f));
  box.rotateDeg(-3.0f * ts, ofVec3f(0.0f, 0.0f, 1.0f));

  speed = 0.8f + (5.0f*smoothedVol - 0.1f - 0.3f);
  // speed = std::max(0.0f, speed);
  player.setSpeed(speed);
}

void ofApp::draw() {
  ofClear(0);
  ofSetColor(255);

  player.draw(0, 0, ofGetWidth(), ofGetHeight());

  cam.setGlobalPosition({
    0,
    0,
    cam.getImagePlaneDistance(ofGetCurrentViewport())
  });

  // Begin
  cam.begin();

    box.draw();

  // End
  cam.end();


  // Draw footer
  ofSetColor(1);
  int h = 50;
  int w = ofGetViewportWidth();
  int x = 0;
  int y = ofGetViewportHeight()-h;
  ofDrawRectangle(x, y, w, h);

  ofSetColor(255);
  ofDrawBitmapString("vol = " + to_string(speed), x+10, y+h-30);
  ofDrawBitmapString("elp = " + to_string(getElapsedMillis()), x+10, y+h-10);
}

unsigned int ofApp::getElapsedMillis() {
  return now() - start;
}

void ofApp::audioIn(ofSoundBuffer & input){

	float curVol = 0.0;

  float v = 0.0f;
  for (size_t i=0; i < input.size(); i++) {
    v += input[i]*input[i];
  }

  v /= (float) input.size();
  v = sqrt(v);
  smoothedVol = 0.1f*smoothedVol + 0.9f*v;

}

void ofApp::keyPressed(int key) { }
void ofApp::keyReleased(int key) { }
void ofApp::mouseMoved(int x, int y ){ }
void ofApp::mouseDragged(int x, int y, int button){ }
void ofApp::mousePressed(int x, int y, int button){ }
void ofApp::mouseReleased(int x, int y, int button){ }
void ofApp::mouseEntered(int x, int y){ }
void ofApp::mouseExited(int x, int y){ }
void ofApp::windowResized(int w, int h){ }
void ofApp::gotMessage(ofMessage msg){ }
void ofApp::dragEvent(ofDragInfo dragInfo){ }
