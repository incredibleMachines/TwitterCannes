#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	img.loadImage("chanel.jpg");
//	img.loadImage("derbty.jpg");
//	img.loadImage("derby2.jpg");
//	img.loadImage("image.jpg");
//	img.loadImage("test.jpg");
	
//	img.loadImage("celebs/1.jpg");
//	img.loadImage("celebs/2.jpg");
//	img.loadImage("celebs/3.jpg");
//	img.loadImage("celebs/4.jpg");
//	img.loadImage("celebs/5.jpg");
	
	//	finder.setup("haarcascade_frontalface_default.xml");
	//	finder.findHaarObjects(img);
	//	cout << finder.blobs.size() << endl;
	//
	//	finder.setup("haarcascade_frontalface_alt_tree.xml");
	//	finder.findHaarObjects(img);
	//	cout << finder.blobs.size() << endl;
	
	finder.setup("haarcascade_frontalface_alt.xml");
	finder.setNeighbors(3);
	
//	finder.findHaarObjects(img);
//	finder.findHaarObjects(img, 100, 100);
	finder.findHaarObjects(img, img.width*0.1, img.width*0.1);
	
	cout << finder.blobs.size() << endl;
	
	//	finder.setup("haarcascade_frontalface_alt2.xml");
	//	finder.findHaarObjects(img);
	//	cout << finder.blobs.size() << endl;
	
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	img.draw(0, 0, img.width/2, img.height/2);
	ofNoFill();
	for(int i = 0; i < finder.blobs.size(); i++) {
		ofRectangle cur = finder.blobs[i].boundingRect;
		ofRect(cur.x/2, cur.y/2, cur.width/2, cur.height/2);
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