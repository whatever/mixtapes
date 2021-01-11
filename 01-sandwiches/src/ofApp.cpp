#include <chrono>

#include "ofApp.h"

#define FPS 60

unsigned int now() {
   using namespace std::chrono;
   return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void ofApp::setup(){

  // Set data root
  string newRoot = "../Resources";
  ofEnableDataPath();
  ofSetDataPathRoot(newRoot);

  // Framerate
  ofSetFrameRate(FPS);

  // WxH
  frameWidth = ofGetWidth();
  frameHeight = ofGetHeight();
  fbo.allocate(frameWidth, frameHeight, GL_RGB);

  // ...
  setupRecorder();

  auto renderer = ofGetGLRenderer();
  std::cout << "01 Mixtape; "
    << "OpenGL = " << renderer->getGLVersionMajor() << "." << renderer->getGLVersionMinor()
    << "\n\n\n\n";

  speed = 0.0f;
  smoothedVol = 3.0f;
  start = now();

  setupMicrophone();

  std::cout << "\n\n";

  std::cout << "Loading shader...\n";

  if (shader.load("video.vert", "video.frag")) {
    std::cout << "Shader successfully loaded!\n";
  }

  std::cout << "Finished loading shader...\n";

  auto videoPath = ofFilePath::getCurrentWorkingDirectory();
  videoPath = ofFilePath::join(videoPath, "sea.mp4");
  videoPath = ofFilePath::getAbsolutePath(videoPath);
  std::cout << "Video path = " << videoPath << "\n";

  player.load(videoPath);
  player.play();

  for (int i=0; i < 4; i++) {
    bgs[i].load(videoPath);
    bgs[i].play();
  }
}

void ofApp::setupRecorder() {
  recorder.setVideoCodec("mpeg4");
  recorder.setVideoBitrate("1800k");
  ofAddListener(recorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
  recorder.setup("../../../../recorded.mov", frameWidth, frameHeight, FPS);
  recorder.start();
}

void ofApp::exit(ofEventArgs &args) {
  ofRemoveListener(recorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
  recorder.close();
  inStream.stop();
  inStream.close();
}

void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args) {
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

void ofApp::update() {
  player.update();

  for (int i=0; i < 4; i++) {
    bgs[i].update();
  }

  update(getElapsedMillis());
  ring.update(getElapsedMillis());

  if (recorder.isInitialized()) {
    recorder.addFrame(pixels);
  }
}

void ofApp::update(unsigned int t) {
  float ts = ((float) getElapsedMillis()) / 1000.0f * 33.0f;
  box.resetTransform();
  // box.setScale(smoothedVol);
  box.setScale(1.0f);
  box.rotateDeg(+1.0f * ts, ofVec3f(1.0f, 0.0f, 0.0f));
  box.rotateDeg(+5.0f * ts, ofVec3f(0.0f, 1.0f, 0.0f));
  box.rotateDeg(-3.0f * ts, ofVec3f(0.0f, 0.0f, 1.0f));

  speed = 0.3f + 4.0f*smoothedVol;
  player.setSpeed(speed/2.0f);
  bgs[0].setSpeed(0.45);
}

void ofApp::drawFooter() {
  ofDisableDepthTest();

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

void ofApp::draw() {
  ofClear(0);
  ofSetColor(255);

  cam.setGlobalPosition({
    0,
    0,
    cam.getImagePlaneDistance(ofGetCurrentViewport())
  });


  fbo.begin();

    // Clear
    ofDisableDepthTest();

    // Draw video in background
    ofClear(0, 0, 0, 255);
    ofSetColor(255, 255, 255);
    player.draw(0, 0, frameWidth, frameHeight);

  fbo.end();

  ofTexture tex = fbo.getTexture();
  ofMesh mesh = videoMesh(tex);

  // XXX: Recursive Video Editing
  // XXX: Other software cannot make videos with infinit layers
  // XXX: So many videos with infinite layers...

  ofFbo t;
  t.allocate(frameWidth, frameHeight, GL_RGB);
  t.begin();

  { // ...
    ofClear(255, 255, 255, 255);
    ofSetColor(255, 255, 255);
    shader.begin();
    shader.setUniformTexture("tex0", fbo.getTexture(), 0);
    shader.setUniformTexture("tex1", bgs[0].getTexture(), 1);
    shader.setUniform1f("alpha", smoothedVol);
    float t = ((float) getElapsedMillis()) / 1000.0f;
    shader.setUniform1f("t", t);
    mesh.draw();
    shader.end();
  }

  { // Draw 3D stuff
    ofEnableDepthTest();
    cam.begin();
    ring.draw();
    cam.end();
  }

  { // Draw Footer
    drawFooter();
  }

  t.end();

  t.draw(0, 0, frameWidth, frameHeight);
  t.readToPixels(pixels);
}

ofMesh ofApp::videoMesh(ofTexture tex) {
  ofMesh mesh;

  float h = frameHeight;
  float w = frameWidth;

  auto nw = tex.getCoordFromPercent(0.0f, 0.0f);
  auto ne = tex.getCoordFromPercent(1.0f, 0.0f);
  auto se = tex.getCoordFromPercent(1.0f, 1.0f);
  auto sw = tex.getCoordFromPercent(0.0f, 1.0f);

  nw = glm::vec2(0, 0);
  ne = glm::vec2(w, 0);
  se = glm::vec2(w, h);
  sw = glm::vec2(0, h);

  // 0
  mesh.addVertex(ofPoint(0.0f, 0.0f));
  mesh.addTexCoord(nw);

  // 1
  mesh.addVertex(ofPoint(w, 0.0f));
  mesh.addTexCoord(ne);

  // 3
  mesh.addVertex(ofPoint(0.0f, h));
  mesh.addTexCoord(sw);

  // 1
  mesh.addVertex(ofPoint(w, 0.0f));
  mesh.addTexCoord(ne);

  // 3
  mesh.addVertex(ofPoint(0.0f, h));
  mesh.addTexCoord(sw);

  // 2
  mesh.addVertex(ofPoint(w, h));
  mesh.addTexCoord(se);
  return mesh;
}

unsigned int ofApp::getElapsedMillis() {
  return now() - start;
}

void ofApp::audioIn(ofSoundBuffer & input){

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
