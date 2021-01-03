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

    void audioIn(ofSoundBuffer & input);

    // ...
    void exit(ofEventArgs &args);

		void update(unsigned int t);
		void setupMicrophone();
		void setupRecorder();

    unsigned int getElapsedMillis();



    ofMesh videoMesh(ofTexture tex);
    void drawFooter();

  protected:

    size_t frameWidth, frameHeight;

    // ...
    float speed;
    unsigned int start;
    float smoothedVol;

    ofEasyCam cam;
    ofBoxPrimitive box;
    ofSoundStream inStream;
    ofVideoPlayer player;

    ofVideoPlayer bgs[4];

    ofFbo fbo;
    ofPixels pixels;

    // Recorder stuff
    ofxVideoRecorder recorder;
    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);

    ofShader shader;
};
