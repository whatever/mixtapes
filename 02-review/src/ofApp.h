#pragma once

#undef TARGET_OS_IOS
#undef TARGET_OS_IPHONE
#define TARGET_OS_MAC 1

#include "ofMain.h"
#include "ofxVideoRecorder.h"
#include "ofSoundPlayerExtended.h"

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
    // void audioOut(ofSoundBuffer &outBuffer);

    // void audioIn(float *input, int bufferSize, int nChannels);
    void setupMicrophone();

    // Draw stuff
    void drawBars(float, std::vector<float>&, float);

  protected:


    ofSoundPlayerExtended audioPlayer;

    ofFbo fbo;
    ofxVideoRecorder recorder;
    ofSoundStream outSoundStream;
    ofSoundStream inStream;

    ofSoundBuffer lastBuffer;

    std::vector<float> buffer;
    std::vector<float> spectrum;
    std::vector<float> outBuffer;

    std::mutex audioMutex;
};
