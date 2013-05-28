//
//  particle.h
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "ofMain.h"

#define PARTICLE_PATH_LINE 0
#define PARTICLE_PATH_CURVE 1
#define PARTICLE_INTERPOLATE_LINEAR 0
#define PARTICLE_INTERPOLATE_EASE 1
#define PARTICLE_INTERPOLATE_SPEED_UP 2
#define PARTICLE_POS_ABS 0
#define PARTICLE_POS_RELATIVE 1
#define PARTICLE_POS_PIXEL 2

class Particle{
public:
    
    class keyframe{
    public:
        
        ofPoint pos;
        ofPoint type=ofPoint(0,0,0);
        ofPoint posMin;
        ofPoint posMax;
        ofPoint rotation;
        
        int length;
        
        int duration=1000;
        int durationMin=0;
        int durationMax=0;
        
        int path=0;
        int interpolation=0;
        bool destroy=false;
        
    };
    
    void setup(keyframe start, keyframe pixel, ofImage image, float imageWidth, float imageHeight);
    void update();
    void draw();
    
    void goToPosition(keyframe goTo);
    void goToPixels();
    
    keyframe current;
    keyframe start;
    keyframe target;
    keyframe pixel;

    ofPoint move;
    bool targetReached;
    bool bInit;

	float imageW;
	float imageH;
	ofImage image;
    ofTexture face;
    
    int timer;
    
};


