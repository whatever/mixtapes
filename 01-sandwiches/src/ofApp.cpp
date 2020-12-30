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

  inStream.printDeviceList();

  for (int i=0; i < devices.size(); i++) {
    auto device = devices[i];
    std::cout << i << " " << device.name << "\n";
  }

  assert(devices.size() > 0);

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
  update(getElapsedMillis());
}

void ofApp::update(unsigned int t) {
  float ts = ((float) getElapsedMillis()) / 1000.0f * 33.0f;
  box.resetTransform();
  box.setScale(100*smoothedVol);
  box.rotateDeg(+1.0f * ts, ofVec3f(1.0f, 0.0f, 0.0f));
  box.rotateDeg(+5.0f * ts, ofVec3f(0.0f, 1.0f, 0.0f));
  box.rotateDeg(-3.0f * ts, ofVec3f(0.0f, 0.0f, 1.0f));
}

void ofApp::draw() {
  ofClear(0);

  // auto devices = soundStream.getMatchingDevices("in");
  // auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE)

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

	// samples are "interleaved"
	int numCounted = 0;

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (size_t i = 0; i < input.getNumFrames(); i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}

	// this is how we get the mean of rms :)
	curVol /= (float) numCounted;

	// this is how we get the root of rms :)
	curVol = sqrt( curVol );

	// smoothedVol *= 0.93;
  smoothedVol = 0.8f*smoothedVol + 0.2f*curVol;
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
