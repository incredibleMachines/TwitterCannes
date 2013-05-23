#pragma once

#include "ofMain.h"
#include "ofxDOF.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    bool toggleBlend;
    
    vector<ofVec3f>points;
    vector<float> speed;
    
    float radius;
    
    ofEasyCam cam;
    ofLight light, light2;
    ofMaterial material;
    
    float camDist;
    
    ofxDOF dof;
    vector<ofColor> color;
    
		
};
