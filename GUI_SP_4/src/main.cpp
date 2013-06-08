#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
    
    // uncomment this line for antialiasing
    window.setGlutDisplayString("rgba double samples>=4");
    
	ofSetupOpenGL(&window, 1920, 1080, OF_FULLSCREEN);
	ofRunApp(new testApp()); // start the app
}