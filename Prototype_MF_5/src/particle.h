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
	bool direction;
    
	void setup(ofPoint pixel, ofImage image, float imageWidth, float imageHeight, float i, float total);
	void flipTarget();

    void update();
    void draw();
	
	bool targetReached;
	int rotationType;
	
	float myIndex;
	float offset;
};


