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
    int category;
    int starred;
};

class Tweet {
    
public:
    
    void update();
    void draw();
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
        
    };
    
    class TweetUser{
    public:
        ofTexture profileImgTex;
        vector<ofxBulletCustomShape*> letters;
        vector<Particle> particles;
        bool bNewKey;
        bool bFinished;
    };
    
    class Animation{
    public:
        string path;
        int speed;
        int delay;
        
    };
    
    void loadTweet(db item, ofxBulletWorldRigid* _world, Alphabet* _gotham);
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
    
    Animation tweetIn;
    Animation tweetOut;
    Animation imgIn;
    Animation imgOut;
    Animation userIn;
    Animation userOut;
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
    void loadParticleKeyframes(Animation anim, int which);
    ofTexture white;
    
    void destroy();
    
    bool bImage;
    
    void switchKey(Particle::Keyframe key, int which);
};


