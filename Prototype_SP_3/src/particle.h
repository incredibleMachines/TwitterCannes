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
    ofPoint pixelPos;
	ofPoint prevPos;

    ofPoint start;
	ofPoint target;

	float imageW;
	float imageH;
	ofImage image;

    float percentage;
	float percentageAdder;
	bool bDirection;
    
	void setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight);
	void flipTarget();

    void update();
    void draw();
	
	bool targetReached;
	
	float myIndex;
	float offset;
    
    ofPoint end;
    ofPoint startvector;
    ofPoint endvector;

    ofTexture face;



};

