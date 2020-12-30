#include "ofApp.h"

#include <chrono>

unsigned int now() {
   using namespace std::chrono;
   return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void ofApp::setup(){

  smoothedVol = 3.0f;
  start = now();

  setupMicrophone();



  std::cout << "\n\n";
  // std::cout << "CWD = " << ofFilePath::getCurrentWorkingDirectory() << "\n";
  auto videoPath = ofFilePath::getAbsolutePath("../../sea.mp4");
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
    std::cout << i << " " << device.name << "\n";
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
  float ts = ((float) getElapsedMillis()) / 1000.0f * 33.0f;
  box.resetTransform();
  box.setScale(20*smoothedVol);
  box.rotateDeg(+1.0f * ts, ofVec3f(1.0f, 0.0f, 0.0f));
  box.rotateDeg(+5.0f * ts, ofVec3f(0.0f, 1.0f, 0.0f));
  box.rotateDeg(-3.0f * ts, ofVec3f(0.0f, 0.0f, 1.0f));
}

void ofApp::draw() {
  ofClear(0);

  int h = player.getHeight();
  int w = player.getWidth();
  player.draw(0, 0, ofGetWidth(), ofGetHeight());

  cam.setGlobalPosition({
    0,
    0,
    cam.getImagePlaneDistance(ofGetCurrentViewport())
  });

  // Begin
  cam.begin();

    box.draw();

    ofDrawBitmapString(to_string(smoothedVol), 100, 50);
    ofDrawBitmapString(to_string(getElapsedMillis()), 100, 100);

  // End
  cam.end();

}

unsigned int ofApp::getElapsedMillis() {
  return now() - start;
}

void ofApp::audioIn(ofSoundBuffer & input){

	float curVol = 0.0;

	for (size_t i = 0; i < input.getNumFrames(); i++){
		left[i]		= input[i*2+0]*0.5f;
		right[i]	= input[i*2+1]*0.5f;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
	}

	curVol /= (float) (2*input.getNumFrames());
	curVol = std::min(1.0f, sqrt(curVol));

  if (input.getNumFrames() == 0) {
    std::cout << "Num frames is zero!\n";
  }

  smoothedVol = 0.7f*smoothedVol + 0.3f*curVol;
  smoothedVol = std::min(1.00f, smoothedVol);
  smoothedVol = std::max(0.01f, smoothedVol);
  smoothedVol = 0.05f;
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
