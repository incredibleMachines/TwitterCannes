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
#define PARTICLE_RATE_LINEAR 0
#define PARTICLE_RATE_EASE 1
#define PARTICLE_RATE_SPEED 2
#define PARTICLE_POS_ABS 0
#define PARTICLE_POS_RELATIVE 1
#define PARTICLE_POS_PIXEL 2

class Particle{
public:
    
    class particlePosition{
    public:
        
        ofPoint pos;
        ofPoint type=ofPoint(0,0,0);
        ofPoint posMin;
        ofPoint posMax;
        
        float rate=1;
        float rateMin;
        float rateMax;
        
        int path=0;
        int startRate=0;
        float startRange;
        int endRate=0;
        float endRange;
        bool destroy=false;
        
    };
    
    void setup(particlePosition start, particlePosition pixel, ofImage image, float imageWidth, float imageHeight);
    void update();
    void draw();
    
    void goToPosition(particlePosition goTo);
    void initPosition(particlePosition goTo);
    void goToPixels();
    
    particlePosition current;
    particlePosition start;
    particlePosition target;
    particlePosition pixel;

    ofPoint move;
    bool targetReached;
    bool bInit;

	float imageW;
	float imageH;
	ofImage image;
    ofTexture face;
    
};


