//
//  database.h
//  SQLiteJSONTest
//
//  Created by Incredible Machines on 6/12/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxSQLite.h"
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

class Database{

    public:
        void setup();
        void update();
        void formatTimestamp();
        void loadMostRecentTweetTimestamp();
        bool tweetExists(string id);
        void fetchTweets();
        db returnDbObj();
    
        ofxSQLite *sqlite;
        ofxJSONElement json;
        bool parsed;
        
        float lastUpdated, updateInterval;
        string since;
    
};


