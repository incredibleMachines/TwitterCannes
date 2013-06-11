//
//  tweet.h
//  twitterCannes
//
//  Created by IncredibleMachines on 6/10/13.
//
//

#include "ofxBullet.h"
#include "alphabet.h"
#include "particle.h"
#include "ofUTF8.h"
//#include "ofUTF16.h"
#include "ofUnicode.h"
#include "ofTextConverter.h"
#include "ofxJSONElement.h"

class Tweet {
    
public:
    
    void update(ofxBulletWorldRigid* world);
    void draw();
    void tweetToKinematic();
    void imgToKinematic();
    void userToKinematic();
    void tweetToPhysics(ofxBulletWorldRigid world);
    void imgToPhysics();
    void userToPhysics();
    void updateTweet();
    void updateImg();
    void updateUser();
    
    class tweetImage {
    public:
        vector<ofxBulletBox*> shapes;
        vector <Particle> particles;
        string location;
        vector<ofTexture> face;
        bool bNewKey;
        
    };
    
    class tweetUser{
    public:
        ofTexture profileImgTex;
        vector<ofxBulletCustomShape*> letters;
        vector<Particle> particles;
        bool bNewKey;
    };
    
    void loadTweet(string _text, string _ID, string _img, string _username, string _handle, string _profileimage, ofxBulletWorldRigid* _world);
    tweetImage loadImage(string _image, ofxBulletWorldRigid* _world);
    tweetUser loadUser(string _username, string _handle, string _profileimage, ofxBulletWorldRigid* _world, Alphabet* _gotham);
    
    tweetImage image;
    tweetUser user;
    vector<ofxBulletCustomShape* > letters;
    vector<Particle> particles;
    vector <Particle::keyframe> tweetKeyframes;
    vector <Particle::keyframe> imageKeyframes;
    vector <Particle::keyframe> userKeyframes;
    int keyframe, imageKeyframe, userKeyframe;
    string ID;
    string text;
    
    Alphabet gotham;
    
        string tweetIn;
        string tweetOut;
        string imgIn;
        string imgOut;
        string userIn;
        string userOut;
    bool bNewKey;
    
    btBoxShape*					boxShape;
    ofxBulletWorldRigid* world;
    
    ofPoint boxScale;
    ofPoint imagePos;
    ofPoint tweetScale;
    ofPoint tweetPos;
    ofPoint userPos;
    ofPoint userScale;
    ofPoint handleScale;
    void loadParticleKeyframes(string filePath, int which);
    ofTexture white;
};


