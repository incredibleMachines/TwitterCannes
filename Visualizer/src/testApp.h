#pragma once

#include "stdio.h"

#include "ofMain.h"
#include "ofxDOF.h"
#include "ofxAssimpModelLoader.h"
#include "ofxXmlSettings.h"
#include "ofxBullet.h"
#include "shadowMapLight.h"
#include "tweet.h"
#include "ofxSQLite.h"


class testApp : public ofBaseApp {
	
public:
    
    class Hashletter{
    public:
        ofPoint pos;
        ofPoint rot;
        ofPoint scale;
        bool active;
        string key;
        ofPoint size;
        ofRectangle checkbox;
    };
    
    vector<db> list;
    
	void setup();
	void update();
	void draw();
    
    void setupGL();
    void loadMasterKeyframe();

    void loadHashtag();
    void drawGUI();
    void loadSQL();
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void keyReleased(int key);
    
    void setupLights();
    void drawObjects();
    
    vector<Tweet::Tweet> tweets;
    Tweet::Tweet tweet;
    int tweetCount;
    
	ofEasyCam camera;
	ofxDOF dof;
    ofMaterial material;
    
    ofxBulletWorldRigid   world;
    ofxBulletBox          background;
    
    ofxBulletBox* box;

    ofImage whiteImg;
    ofTexture white;
    
    Alphabet gotham;
    
    vector<Hashletter> hashletters;
    vector<ofMesh> hashMeshes;
    ofxAssimpModelLoader hashModel;
    vector<ofxBulletCustomShape*> hashCollision;
    bool bSingle=false;
    
    btBoxShape*					boxShape;
    
    ofxXmlSettings settings;
    
    int listCount;
    
    bool bGUI;
    int camState;
    
    void updateCollision(int i);
    
    bool bDebug=false;

    bool bRot;
    bool bScale;
        
    ofShader shader;
    ShadowMapLight shadowLightLeft, shadowLightRight;
    ofMesh mesh;
    
    float   lightAngle;
    bool    bDrawDepth;
    bool    bDrawLight;
    bool    bShadowsOn;
    
    ofxSQLite* sqlite;


};

