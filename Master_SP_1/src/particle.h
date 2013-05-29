//
//  particle.h
//  scott_particles
//
//  Created by IncredibleMachines on 5/20/13.
//
//

#include "ofMain.h"

#include "Quad.h"
#include "Quart.h"
#include "Quint.h"
#include "Sine.h"
#include "Linear.h"
#include "Expo.h"
#include "Elastic.h"
#include "Cubic.h"
#include "Circ.h"
#include "Bounce.h"
#include "Back.h"

#define PARTICLE_PATH_LINE 0
#define PARTICLE_PATH_CURVE 1

#define PARTICLE_INTERPOLATE_LINEAR 0
#define PARTICLE_INTERPOLATE_BACK_EASE_IN 1
#define PARTICLE_INTERPOLATE_BACK_EASE_OUT 2
#define PARTICLE_INTERPOLATE_BACK_EASE_IN_OUT 3
#define PARTICLE_INTERPOLATE_BOUNCE_EASE_IN 4
#define PARTICLE_INTERPOLATE_BOUNCE_EASE_OUT 5
#define PARTICLE_INTERPOLATE_BOUNCE_EASE_IN_OUT 6
#define PARTICLE_INTERPOLATE_CIRC_EASE_IN 7
#define PARTICLE_INTERPOLATE_CIRC_EASE_OUT 8
#define PARTICLE_INTERPOLATE_CIRC_EASE_IN_OUT 9
#define PARTICLE_INTERPOLATE_CUBIC_EASE_IN 10
#define PARTICLE_INTERPOLATE_CUBIC_EASE_OUT 11
#define PARTICLE_INTERPOLATE_CUBIC_EASE_IN_OUT 12
#define PARTICLE_INTERPOLATE_ELASTIC_EASE_IN 13
#define PARTICLE_INTERPOLATE_ELASTIC_EASE_OUT 14
#define PARTICLE_INTERPOLATE_ELASTIC_EASE_IN_OUT 15
#define PARTICLE_INTERPOLATE_EXPO_EASE_IN 16
#define PARTICLE_INTERPOLATE_EXPO_EASE_OUT 17
#define PARTICLE_INTERPOLATE_EXPO_EASE_IN_OUT 18
#define PARTICLE_INTERPOLATE_QUAD_EASE_IN 19
#define PARTICLE_INTERPOLATE_QUAD_EASE_OUT 20
#define PARTICLE_INTERPOLATE_QUAD_EASE_IN_OUT 21
#define PARTICLE_INTERPOLATE_QUART_EASE_IN 22
#define PARTICLE_INTERPOLATE_QUART_EASE_OUT 23
#define PARTICLE_INTERPOLATE_QUART_EASE_IN_OUT 24
#define PARTICLE_INTERPOLATE_QUINT_EASE_IN 25
#define PARTICLE_INTERPOLATE_QUINT_EASE_OUT 26
#define PARTICLE_INTERPOLATE_QUINT_EASE_IN_OUT 27
#define PARTICLE_INTERPOLATE_SINE_EASE_IN 28
#define PARTICLE_INTERPOLATE_SINE_EASE_OUT 29
#define PARTICLE_INTERPOLATE_SINE_EASE_IN_OUT 30

#define PARTICLE_POS_ABS 0
#define PARTICLE_POS_RELATIVE 1
#define PARTICLE_POS_PIXEL 2

class Particle{
public:
    
    class keyframe{
    public:
        
        ofPoint type=ofPoint(0,0,0);
        
        ofPoint pos;
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
    
    void setup(keyframe start, keyframe pixel, ofImage image, float imageWidth, float imageHeight, float myIndex);
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
    
    float myIndex;
    
    int startTime;
    int playhead;
    
};


