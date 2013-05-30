#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxDOF.h"
#include "ofx3DModelLoader.h"



class testApp : public ofBaseApp {
	
public:
	void setup();
	void update();
	void draw();
	
    void loadImage();
    void loadDir();
    void loadCamKeyframes();
    void setupGL();
    
    	void mousePressed(int x, int y, int button);
    
    ofImage pic;
	unsigned char * imagePixels;
    int imgCount;
    vector<string> images;
    
	
    vector <Particle> particles;
    vector <Particle::keyframe> particleKeyframes;
    void loadParticleKeyframes();
    void createParticles();
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
    
    void mouseDragged(int x, int y, int button);
    
        ofxBulletWorldRigid			world;
        ofxBulletBox                  ground;
            ofxBulletBox                  groundLow;

    vector<ofxBulletBox*> shapes;
    ofxBulletBox* box;
    vector<ofTexture> face;
    
    btBoxShape*					boxShape;
    
    ofx3DModelLoader hashtag;
    ofMesh C_upper;
	ofMesh A_lower;
    ofMesh N_lower;
    ofMesh E_lower;
    ofMesh S_lower;
    ofMesh L_upper;
    ofMesh I_lower;
    ofMesh O_lower;
    
    ofVbo letter;
    
    ofImage whiteImg;
    ofTexture white;
    ofMaterial whiteMat;
    
    int zPos;
    bool bZ;
    
    float drawPhysics;
    float bSwitchPhysics;
    
    
};

