#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

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
}

//--------------------------------------------------------------
void ofApp::update(){
  if (player.isLoaded()) {
  }
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
