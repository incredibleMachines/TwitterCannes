#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // interval for demo only
    // should be event driven
    updateInterval = 30;
    lastUpdated = -updateInterval;
    
    // setup our get queries
    // these are the options for query
    // limit: how many tweets to return
    // category: Celebrities, Executive+Tweets, Speaker+Quotes
    // starred: true/false
    urls.push_back("limit=10");
    urls.push_back("limit=10&starred=true");
    urls.push_back("limit=10&category=Speaker+Quotes");
    urls.push_back("limit=10&category=Celebrities&starred=true");

    // to cycle through different URLs
    urlCounter = 0;
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    // using a timer demo only
    if (ofGetElapsedTimef() - lastUpdated > updateInterval) {
        fetchTweets();
        
        lastUpdated = ofGetElapsedTimef();
    }
    
}


//--------------------------------------------------------------
void testApp::fetchTweets(){
    
	string url = "http://127.0.0.1:9000/?" + urls[urlCounter];
    cout << "\nFetching url: " + url << endl;

    parsed = json.open(url);
    
    if (parsed) {
        
        for (int i=0; i < json.size(); i++) {
            
            // Grab current data out of results
            ofxJSONElement data = json[i];
            
            string user_name = data["user_name"].asString();
            string user_screen_name = data["user_screen_name"].asString();
            string user_image = data["user_image"].asString();
            
            string text = data["text"].asString();
            string media_url = data["media_url"].asString();
            
            // Can load ofImages here
            if (user_image != "") {
                cout << "Adding image: " + user_image << endl;
                ofImage userImage;
                userImage.loadImage(user_image);
                images.push_back(userImage);
            }
            if (media_url != "") {
                cout << "Adding image: " + media_url << endl;
                ofImage mediaImage;
                mediaImage.loadImage(media_url);
                images.push_back(mediaImage);
            }
            
            cout << "@"+user_screen_name+" "+text << endl;
        }
        
    } else {
        cout  << "Failed to parse JSON" << endl;
    }
    
    // magic
    // (increase counter and wrap around to 0 once we hit the end)
    urlCounter = ++urlCounter % urls.size();
    
}

//--------------------------------------------------------------
void testApp::draw(){

    int x, y = 0;
    
    for (int i = 0; i < images.size(); i++) {
        ofImage image = images[i];
        image.resize(50, 50);

        x++;
        if (x > (ofGetWidth() / 50)) {
            x = 0;
            y++;
        }
        image.draw(x*50, y*50);
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}