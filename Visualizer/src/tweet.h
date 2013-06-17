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

class db{
public:
    string user_image;
    string user_name;
    string user_screen_name;
    string text;
    string media_url;
};

class Tweet {
    
public:
    
    void setup(ofPoint _hashMin, ofPoint _hashMax, ofxBulletWorldRigid* _world, Alphabet* _gotham);
    void update();
    void updateMulti();
    void draw();
    void drawImg();
    void drawLetters();
    void tweetToKinematic();
    void imgToKinematic();
    void userToKinematic();
    void tweetToPhysics();
    void imgToPhysics();
    void userToPhysics();
    void updateTweet();
    void updateImg();
    void updateUser();
    
    class TweetImage {
    public:
        vector<ofxBulletBox*> shapes;
        vector <Particle> particles;
        string location;
        vector<ofTexture> face;
        bool bNewKey;
        bool bFinished;
        int width, height;
    };
    
    class TweetUser{
    public:
        ofTexture profile;
        vector<ofxBulletCustomShape*> letters;
        vector<Particle> particles;
        bool bNewKey;
        bool bFinished;
        ofxBulletBox* shape;
        string username;
        string handle;
        string profileImg;
    };
    
    class Animation{
    public:
        string path;
        int speed;
        int delay;
        
    };
    
    void loadMulti(vector<db> items);
    void loadTweet(db item);
    void loadImage(string _image);
    void loadUser(string _username, string _handle, string _profileimage);
    
    bool bFinished;
    TweetImage image;
    TweetUser user;
    vector<ofxBulletCustomShape* > letters;
    vector<Particle> particles;
    vector <Particle::Keyframe> tweetKeyframes;
    vector <Particle::Keyframe> imageKeyframes;
    vector <Particle::Keyframe> userKeyframes;
    int tweetKeyframe, imageKeyframe, userKeyframe;
    string ID;
    string text;
    
    Alphabet* gotham;
    
    Animation tweetIn, tweetOut, imgIn, imgOut, userIn, userOut;
    bool bNewKey;
    
    btBoxShape*					boxShape;
            btBoxShape* profileBox;
    ofxBulletWorldRigid* world;
    
    ofPoint imageScale;
    ofPoint imagePos;
    ofPoint tweetScale;
    ofPoint tweetPos;
    ofPoint userPos;
    ofPoint userScale;
    ofPoint handleScale;
    ofPoint profileScale;
    void loadParticleKeyframes(Animation anim, int which,bool In);
    ofTexture white;
    ofImage pic;
    
    void destroy();
    
    bool bImage;
    
    void switchKey(Particle::Keyframe key, int which);
    
    ofPoint hashMin, hashMax;
    
    string tweetType, userType, imageType;
    
    int animationCount;
    
    int masterDelay;
    
    vector<db> multis;
    
    void drawMultiImg();
    
    ofPoint mUserPos;
    bool bMulti;
    
    int tweetTimer, imgTimer, userTimer;
    void goTweets();
    void goUser();
    void goImg();
    bool tweetStart, imgStart, userStart;
    
    
    ofxBulletBox* tweetBlock;
    btBoxShape* tweetBox;
    ofPoint tweetBoxPos;

};


