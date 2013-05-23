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
    ofVec2f pos;
    ofVec2f pixelPos;

    ofVec2f start;
	ofVec2f target;


	int imageW;
	int imageH;
    
	float r;
    float g;
    float b;

    float percentage;
    
	void setup(ofVec2f pixel, int red, int green, int blue, int imageWidth, int imageHeight);
	void flipTarget();

    void update();
    void draw();
	
	bool targetReached;
};


