#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    void exit(ofEventArgs &args);

		void setupRecorder();
    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
    void audioIn(ofSoundBuffer & input);
    // void audioIn(float *input, int bufferSize, int nChannels);
    void setupMicrophone();

  protected:
    ofSoundPlayer audioPlayer;
    ofFbo fbo;
    ofxVideoRecorder recorder;
    ofSoundStream soundStreamOutput;
    ofSoundStream inStream;
};
