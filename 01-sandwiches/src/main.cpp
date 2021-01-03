#include "ofMain.h"
#include "ofApp.h"

int main() {
  float s = 0.3f;
  int w = 1080 * s;
  int h = 1920 * s;

  ofGLFWWindowSettings settings;
  settings.setGLVersion(3, 2);
  settings.setSize(w, h);

  ofCreateWindow(settings);
  ofRunApp(new ofApp());
}
