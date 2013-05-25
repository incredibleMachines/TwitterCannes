#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxDOF.h"


class testApp : public ofBaseApp {	
	
public:
	void setup();
	void update();
	void draw();
	
	void mouseDragged(int x, int y, int button);
    void loadImage();
    void loadDir();
    void loadCams();
    void populatePixels();
    void setupGL();
    
	ofImage pic;
	unsigned char * imagePixels;
	
    vector <Particle> particles;
	
	ofEasyCam camera;
	ofxDOF dof;
    ofLight light;
    ofMaterial material;
    
    int imgCount;    
    vector<string> images;
 
    class camPoint{
    public:
        ofPoint pos;
        ofPoint lookAt;
        float rate;
        bool reached;
        
    };
    
    ofPoint camPos;
    int camCount;
    vector<camPoint> camPoints;
    
    int animateCount;

};

