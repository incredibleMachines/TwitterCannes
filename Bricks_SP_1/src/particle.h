//
//  particle.h
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "ofMain.h"

class Particle{
public:
    ofPoint pos;
    ofPoint prevPos;
    ofPoint target;
    ofPoint start;
    
    ofPoint pixelPos;    
    class pointRange{
    public:
        ofPoint pos;
        ofPoint min;
        ofPoint max;
        
    };
    pointRange startPos;
    
    ofPoint move;

	float imageW;
	float imageH;
	ofImage image;

    float rate;
	bool bStart;
    
	void setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight);

    void update();
    void draw();
	
	bool targetReached;
	
	float myIndex;
	float offset;

    ofTexture face;

    void setVector();
    
    int rateMin;
    int rateMax;
    
    
    

};


