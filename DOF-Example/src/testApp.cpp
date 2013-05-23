#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
//    glEnable(GL_DEPTH_TEST);
    
    //lights
    ofSetSmoothLighting(true);
    light.setDiffuseColor(ofColor(255,100,100));
    light.setSpecularColor(ofColor(255,200,150));
    light.setPointLight();
    light.setPosition(0, 0, 1600);
    
    light2.setPointLight();
    light2.setPosition(500, 0, -3000);
    
    //material
    material.setShininess(128);
    material.setSpecularColor(ofColor(255,255,100));
    material.setEmissiveColor(ofColor(255,0,0,30));
    
    //dof
    dof.setup(1024,768);
    dof.setBlurAmount(0.5);
    dof.setFocalRange(100);

    //camera
    camDist = 1600;
    cam.setDistance(camDist);
    cam.setFov(100);
    
    toggleBlend = true;
    
    ofSetCircleResolution(60);
    // randomly add a point on a sphere
	int   num = 800;
	radius = 500;
	for(int i = 0; i<num; i++ ) {
		
//		float theta1 = TWO_PI/num * i;
//		float theta2 = TWO_PI/num * i * 3;
		
        float theta1 = ofRandom(TWO_PI);
		float theta2 = ofRandom(TWO_PI);
		ofVec3f p;
		p.x = cos( theta1 ) * cos( theta2 );
		p.y = sin( theta1 );
		p.z = cos( theta1 ) * sin( theta2 );
		p *= radius;
		
        points.push_back(p);
        
        float tempSpeed = ofRandom(2);
        speed.push_back(tempSpeed);
        
        ofColor tempColor(ofRandom(100,255),ofRandom(100,255),0,100);
        color.push_back(tempColor);
        
	}
    

    
    
    


}

//--------------------------------------------------------------
void testApp::update(){
    
    	dof.setFocalDistance(ofMap(mouseY,0,ofGetHeight(),200, 700));

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackgroundGradient( ofColor(50), ofColor(0), OF_GRADIENT_CIRCULAR);
    ofEnableLighting();

    
    if (toggleBlend) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofEnablePointSprites();
        ofRect(0, 0, 10, 10);
    }else{
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }
    
//    glDepthMask(GL_TRUE);
    
    dof.begin();
    cam.begin(dof.getDimensions());
    light.enable();
    light2.enable();
    material.begin();
    
    ofSetLineWidth(4);
    ofNoFill();
    ofSetColor(255, 30);
    ofSphere(0, 0, radius);
    ofFill();
    
    ofRect(points[0].x, points[0].y, points[0].z, 10, 10);    
    
    float rad = 100;
    for (int i=0; i<points.size(); i++) {
        float ang = ofGetElapsedTimef() *speed[i];
        
        ofVec3f mid=points[i];
        mid.normalize();
        mid *= radius + abs(sin(ang)*rad);
       
        ofSetColor(255, 50);

        ofLine(points[i], mid);
        
        ofSetColor(color[i],255);

        ofSphere(mid, 10);
    }
    

    
    material.end();
    ofDisableLighting();
    cam.end();
    dof.end();
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(dof.getFocalDistance()), 50,100);
    
    if(ofGetKeyPressed('f')){
		dof.drawFocusAssist(0, 0);
	}
	else{
		dof.getFbo().draw(0, 0);
	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
        toggleBlend = !toggleBlend;

    
    if(key == OF_KEY_UP) {
		camDist -= 100;
	}
	if(key == OF_KEY_DOWN) {
		camDist += 100;
	}
	cam.setDistance(camDist);

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