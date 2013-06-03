#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxDOF.h"
#include "ofx3DModelLoader.h"
#include "ofxXmlSettings.h"



class testApp : public ofBaseApp {
	
public:
	void setup();
	void update();
	void draw();
    
    void loadCamKeyframes();
    void setupGL();
    
    void loadImage();
    void loadDir();
    void initParticles();
    void loadParticleKeyframes();
    
    void drawHashtag(int x, int y, int z);
    void drawHashtag(ofPoint pos);
    void loadTextMeshes();
    void keyPressed(int key);
    
    void mousePressed(int x, int y, int button);
    
    ofImage pic;
	unsigned char * imagePixels;
    int imgCount;
    vector<string> images;
	
    vector <Particle> particles;
    vector <Particle::keyframe> particleKeyframes;
    int particleKeyframe;
	
	ofEasyCam camera;
	ofxDOF dof;
    ofLight light;
    ofLight light2;
    ofMaterial material;
    
    class camPoint{
    public:
        ofPoint pos;
        ofPoint lookAt;
        int pause;
        int posRate=0;
        int posPath=0;
        int lookAtRate=0;
        int lookAtPath=0;
        
        float rate;
        bool reached;
        
    };
    
    ofPoint camPos;
    int camKeyframe;
    vector<camPoint> camKeyframes;
    
    void keyReleased(int key);
    
    ofxBulletWorldRigid           world;
    ofxBulletBox                  ground;
    ofxBulletBox                  groundLow;    
    ofxBulletBox                  background;
    vector<ofxBulletBox*> shapes;
    
    ofxBulletBox* box;
    vector<ofTexture> face;
    btBoxShape*					boxShape;
    
    ofx3DModelLoader hashtag;
    ofx3DModelLoader C_upper;
	ofx3DModelLoader A_lower;
    ofx3DModelLoader N_lower;
    ofx3DModelLoader E_lower;
    ofx3DModelLoader S_lower;
    ofx3DModelLoader L_upper;
    ofx3DModelLoader I_lower;
    ofx3DModelLoader O_lower;
    
    ofImage whiteImg;
    ofTexture white;
    
    bool bDrawPhysics;
    bool bSwitchPhysics;
    bool bStart;
    
    bool bCamChange;
    
    ofPoint C, a, n1, n2, e, s1, l, i, o, n3, s2, hash;
    
    bool bUp=false, bDown=false, bLeft=false, bRight=false;
    
    ofxXmlSettings settings;
    
    ofPoint pos;
    
    float count;
    
    
    
};

