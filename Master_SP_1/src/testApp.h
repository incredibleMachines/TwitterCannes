#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxDOF.h"


class testApp : public ofBaseApp {	
	
public:
	void setup();
	void update();
	void draw();
	
    void loadImage();
    void loadDir();
    void loadCams();
    void setupGL();
    
    ofImage pic;
	unsigned char * imagePixels;
    int imgCount;
    vector<string> images;

	
    vector <Particle> particles;
    vector <Particle::particlePosition> particlePos;
    void loadParticles();
    void populatePixels();
    int particleCount;
	
	ofEasyCam camera;
	ofxDOF dof;
    ofLight light;
    ofMaterial material;
    
    class camPoint{
    public:
        ofPoint pos;
        ofPoint lookAt;
        int pause;              //Pause duration
        string posMotion;       //linear, ease, speed
        string posType;         //Line, Curve
        string lookAtMotion;
        string lookAtType;
        
        float rate;
        bool reached;
        
    };
    
    ofPoint camPos;
    int camCount;
    vector<camPoint> camPoints;
    
    void mouseDragged(int x, int y, int button);

};

