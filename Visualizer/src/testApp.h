#pragma once

#include "stdio.h"
#include "assert.h"

#include "ofMain.h"
#include "ofxDOF.h"
#include "ofxAssimpModelLoader.h"
#include "ofxXmlSettings.h"
#include "ofxBullet.h"
#include "shadowMapLight.h"
#include "tweet.h"


class testApp : public ofBaseApp {
	
public:
	void setup();
	void update();
	void draw();
    
    void setupGL();
    void loadMasterKeyframe();
    void loadContentKeyframes();
    void loadParticleKeyframes();


    void loadDir();
    
    void initImgParticles();
    void initTweetParticles();
    
    void loadHashtag();
    void drawGUI();
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void keyReleased(int key);
    
    void setupLights();
    void drawObjects();
        
       
    vector<Tweet::Tweet> tweets;
    
    Tweet::Tweet tweet;
    int tweetCount;
    
    vector<vector <Particle::keyframe> > particleKeyframes;
    int particleKeyframe;
	
	ofEasyCam camera;
	ofxDOF dof;
    ofMaterial material;
    
    ofxBulletWorldRigid   world;
    ofxBulletBox          background;
    
    ofxBulletBox* box;

    ofImage whiteImg;
    ofTexture white;
    
    class hashletter{
    public:
        ofPoint pos;
        ofPoint rot;
        ofPoint scale;
        bool active;
        string key;
        ofPoint size;
        ofRectangle checkbox;
    };
    
    class content{
    public:
        ofPoint pos;
    };
    
    content image;
    
    vector<hashletter> hashletters;
    ofxAssimpModelLoader hashModel[14];
    vector<ofxBulletCustomShape*> hashCollision;
    bool bSingle=false;
    
    btBoxShape*					boxShape;
    
    ofxXmlSettings settings;
    
    float count;
    
    ofPoint meshCollisionScale;
    ofPoint meshDrawScale;
    
    bool bGUI;
    
    int camState;
    
    bool bCamSwitch;
    
    ofPoint meshOffset;

    ofPoint boxScale;
    
    void updateCollision(int i);
    
    bool bDebug=false;

    bool bRot;
    bool bScale;
    
    
    ofxAssimpModelLoader collisionPlane;
    ofTexture multiply;
    
    
    Alphabet gotham;
    
    ofPoint tweetScale;
    ofPoint tweetPos;
    ofPoint userScale;
    ofPoint userPos;
    ofPoint handleScale;
    int bTweet;
    
    
    void tweetToKinematic();
    void tweetToPhysics();
    void imgToPhysics();
    void imgToKinematic();
    void updateImg();
    void updateTweet();
    
    ofShader shader;
    ShadowMapLight shadowLight;
    ofxAssimpModelLoader model;
    ofMesh mesh;
    
    float   lightAngle;
    bool    bDrawDepth;
    bool    bDrawLight;
    
    ofTrueTypeFont gui;


};

