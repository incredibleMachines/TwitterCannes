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
    ofVec2f pixelpos;
    ofVec2f start;
    ofVec2f end;
    ofVec2f startvector;
    ofVec2f endvector;
    float r;
    float g;
    float b;
    float percentage;
    void setup(ofVec2f pixel, int red, int green, int blue);
    void update();
    void draw();
};


