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
    
    float rate;
    int rateMin;
    int rateMax;
    ofPoint move;
    bool bStart;
    bool targetReached;

	float imageW;
	float imageH;
	ofImage image;
    ofTexture face;
    
    ofPoint easePoint;
    float bEase;
	bool bStartEase;

	void setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight);
    void update();
    void draw();
    
    void setEase();
    void setVector();
};


