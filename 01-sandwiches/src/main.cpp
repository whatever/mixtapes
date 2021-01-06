#include "ofMain.h"
#include "ofApp.h"

int main() {
  float s = 0.3f;
  int w = 1080 / 4;
  int h = 1920 / 4;

  ofGLFWWindowSettings settings;
  settings.setGLVersion(3, 2);
  settings.setSize(w, h);

  ofCreateWindow(settings);
  ofRunApp(new ofApp());
}
