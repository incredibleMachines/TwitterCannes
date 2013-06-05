#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxDOF.h"
#include "ofx3DModelLoader.h"
#include "ofxAssimpModelLoader.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "ofxBullet.h"



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
    
    void drawHashtag();
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
	
	ofEasyCam camera,camera2;
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
    
    ofxBulletBox* box;
    vector<ofTexture> face;
    btBoxShape*					boxShape;
    
    ofImage whiteImg;
    ofTexture white;
    ofImage blackImg;
    ofTexture black;
    
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
        ofPoint rot;
        bool active;
        string key;
        ofPoint size;
    };
    
    class content{
    public:
        ofPoint pos;
    };
    
    content image;
    
    vector<hashletter> hashCollisionPos;
    vector<hashletter> hashMeshPos;
    hashtag hash;
//    ofx3DModelLoader tempMesh[12];
    ofxAssimpModelLoader hashModel[12];
        ofxAssimpModelLoader testModel;
    vector<ofxBulletCustomShape*> hashCollision;
    ofTexture hashTex[12];
    ofMesh hashMesh[12];
    
    ofxXmlSettings settings;
    ofxXmlSettings camXML;
    ofxXmlSettings particleXML;
    
    
    float count;
    
    ofxUICanvas *hashtagGUI;
    
    bool bSingle=false;
    
    ofPoint meshCollisionScale;
    ofPoint meshDrawScale;
    
    bool bGUI;
    
    int camState;
    
    bool bCamSwitch;
    
    ofPoint meshOffset;

    
    ofPoint boxScale;
    
};

