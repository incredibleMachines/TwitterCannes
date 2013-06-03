#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxDOF.h"
#include "ofx3DModelLoader.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"



class testApp : public ofBaseApp {
	
public:
	void setup();
	void update();
	void draw();
    
    void setupGL();
    void loadCamKeyframes();
    void loadParticleKeyframes();
    void loadImage();
    void loadDir();
    
    void initParticles();
    
    void drawHashtag(int x, int y, int z);
    void drawHashtag(ofPoint pos);
    void loadHashtagGUI();
    void guiEvent(ofxUIEventArgs &e);
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void keyReleased(int key);
    
    vector<string> images;
    ofImage pic;
	unsigned char * imagePixels;
    int imgCount;

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
    
    
    ofxBulletWorldRigid           world;
    ofxBulletBox                  ground;
    ofxBulletBox                  groundLow;    
    ofxBulletBox                  background;
    vector<ofxBulletBox*> shapes;
    vector<ofxBulletBox*> tempShapes;
    
    ofxBulletBox* box;
    vector<ofTexture> face;
    btBoxShape*					boxShape;
    
    ofImage whiteImg;
    ofTexture white;
    
    bool bDrawPhysics;
    bool bSwitchPhysics;
    bool bStart;
    bool bNewParticleKey=false;
    
    bool bCamChange;
    
    class hashtag{
    public:
        ofPoint pos;
        ofPoint rotation;
        ofPoint scale;
        bool active;
    };
    class hashletter{
    public:
        ofPoint pos;
        bool active;
        string key;
    };
    vector<hashletter> hashletters;
    hashtag hash;
    ofx3DModelLoader tempMesh[12];
    
    ofxXmlSettings settings;
    ofxXmlSettings camXML;
    ofxXmlSettings particleXML;
    
    
    float count;
    
    ofxUICanvas *hashtagGUI;
    
    bool bSingle=false;


};

