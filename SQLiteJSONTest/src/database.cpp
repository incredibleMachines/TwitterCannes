//
//  database.cpp
//  SQLiteJSONTest
//
//  Created by Incredible Machines on 6/12/13.
//
//

#include "database.h"

void Database::setup(){
    
    updateInterval = 10;
    lastUpdated = -updateInterval;
    
    sqlite = new ofxSQLite("twitterCannesLions.db");
    
    loadMostRecentTweetTimestamp();
    
}

//--------------------------------------------------------------
void Database::update(){
    
    if (ofGetElapsedTimef() - lastUpdated > updateInterval) {
        
        string url = "http://ec2-23-23-188-178.compute-1.amazonaws.com:9000/?since="+since;
        parsed = json.open(url);
        
        cout << "Fetching since: " << since << endl;
        
        if (parsed) {
            
            for(int i=0; i < json.size(); i++) {
                
                // Grab current data out of all results
                ofxJSONElement data = json[i];
                string id = data["id"].asString();
                cout << "Got tweet with id: " << id << endl;
                
                // Set up file paths
                string user_id = data["user_id"].asString();
                string user_image = data["user_image"].asString();
                string media_url = data["media_url"].asString();
                
                string user_image_path, media_url_path;
                
                //cout << "From: " << user_image << endl;
                //cout << "To: " << user_image_path << endl;
                if(user_image != "" ) {
                    ofFile userFile(user_image);
                    string user_ext = userFile.getExtension();
                    user_image_path = "user_images/" + user_id + "." + user_ext;
                    
                    ofSaveURLAsync(user_image, user_image_path);
                } else {
                    user_image_path = "";
                }
                
                if(media_url != "" ) {
                    ofFile mediaFile(media_url);
                    string media_ext = mediaFile.getExtension();
                    media_url_path = "media_images/"+ id +"."+media_ext;
                    
                    ofSaveURLAsync(media_url, media_url_path);
                } else {
                    media_url_path = "";
                }
                
                // check if we should be updating or inserting
                if (tweetExists(id)) {
                    
                    cout << "Updating " << id << endl;
                    
                    // update
                    sqlite->update("tweets")
                    .use("approved_at", data["approved_at"].asString())
                    .use("created_at", data["created_at"].asString())
                    .use("category", data["category"].asString())
                    .use("media_url", media_url_path)
                    .use("starred", data["starred"].asInt())
                    .use("text", data["text"].asString())
                    .use("user_id", data["user_id"].asString())
                    .use("user_image", user_image_path)
                    .use("user_name", data["user_name"].asString())
                    .use("user_screen_name", data["user_screen_name"].asString())
                    .where("id", id)
                    .execute();
                    
                } else {
                    
                    cout << "Inserting " << id << endl;
                    
                    // insert
                    sqlite->insert("tweets")
                    .use("id", id)
                    .use("approved_at", data["approved_at"].asString())
                    .use("created_at", data["created_at"].asString())
                    .use("category", data["category"].asString())
                    .use("media_url", media_url_path)
                    .use("starred", data["starred"].asInt())
                    .use("text", data["text"].asString())
                    .use("user_id", data["user_id"].asString())
                    .use("user_image", user_image_path)
                    .use("user_name", data["user_name"].asString())
                    .use("user_screen_name", data["user_screen_name"].asString())
                    .execute();
                    
                }
                
                
                since = data["approved_at"].asString();
                formatTimestamp();
            }
            
        } else {
            cout  << "Failed to parse JSON" << endl;
            
        }
        
        lastUpdated = ofGetElapsedTimef();
    }
    
}

//--------------------------------------------------------------
void Database::formatTimestamp() {
    ofStringReplace(since, " ", "+");
    ofStringReplace(since, ":", "%3A");
}

//--------------------------------------------------------------
void Database::loadMostRecentTweetTimestamp() {
    
	ofxSQLiteSelect most_recent_sel = sqlite->select("approved_at")
    .from("tweets")
    .order("approved_at", "DESC")
    .limit(1)
    .execute().begin();
    
	while (most_recent_sel.hasNext()) {
        since = most_recent_sel.getString();
		most_recent_sel.next();
	}
    
    if (since == "") {
        since = "2013-01-01 00:00:00";
    }
    
    formatTimestamp();
    
}

//--------------------------------------------------------------
bool Database::tweetExists(string id) {
    
	ofxSQLiteSelect sel = sqlite->select("count(id)")
    .from("tweets")
    .where("id", id)
    .execute().begin();
    
    int count = sel.getInt();
    
    if (count > 0)
        return true;
    else
        return false;
}

//--------------------------------------------------------------
void Database::fetchTweets(){
    
	ofxSQLiteSelect sel = sqlite->select("id, approved_at, created_at, user_id, user_image, user_name, user_screen_name, text, media_url, category, starred")
    .from("tweets")
    .execute().begin();
    
	while (sel.hasNext()) {
        
        string id = sel.getString();
        string approved_at = sel.getString();
        string created_at = sel.getString();
        string user_id = sel.getString();
        string user_image = sel.getString();
        string user_name = sel.getString();
        string user_screen_name = sel.getString();
        string text = sel.getString();
        string media_url = sel.getString();
        int category = sel.getInt();
        int starred = sel.getInt();
        
		sel.next();
	}
    
}
