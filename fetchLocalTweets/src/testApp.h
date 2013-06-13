#pragma once

#include "ofMain.h"
#include "ofxJSONElement.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void formatTimestamp();
    void loadMostRecentTweetTimestamp();

    bool tweetExists(string id);
    void fetchTweets();
    
    ofxJSONElement json;
    bool parsed;

    vector <string> urls;
    int urlCounter;

    float lastUpdated, updateInterval;
    
};
