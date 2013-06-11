#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
    
    // uncomment this line for antialiasing
    //window.setGlutDisplayString("rgba double samples>=4");
    
	ofSetupOpenGL(&window, 1280, 610, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}