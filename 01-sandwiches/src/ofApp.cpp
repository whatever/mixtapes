#include "ofApp.h"

#include <chrono>

unsigned int now() {
   using namespace std::chrono;
   return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void ofApp::setup(){
  start = now();
}

void ofApp::update() {
  update(getElapsedMillis());
}

void ofApp::update(unsigned int t) {
  float ts = ((float) getElapsedMillis()) / 1000.0f * 33.0f;
  box.resetTransform();
  box.rotate(+1.0f * ts, ofVec3f(1.0f, 0.0f, 0.0f));
  box.rotate(+5.0f * ts, ofVec3f(0.0f, 1.0f, 0.0f));
  box.rotate(-3.0f * ts, ofVec3f(0.0f, 0.0f, 1.0f));
}

void ofApp::draw(){
  ofClear(0);

  cam.setGlobalPosition({
    0,
    0,
    cam.getImagePlaneDistance(ofGetCurrentViewport())
  });

  cam.begin();

    box.draw();
    ofDrawBitmapString(to_string(getElapsedMillis()), 100, 100);

  cam.end();
}

unsigned int ofApp::getElapsedMillis() {
  return now() - start;
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
