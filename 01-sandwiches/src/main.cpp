#include "ofMain.h"
#include "ofApp.h"

int main() {
  float s = 0.3f;
  int w = 1080 * s;
  int h = 1920 * s;
  ofSetupOpenGL(w, h, OF_WINDOW);
  ofRunApp(new ofApp());
}
