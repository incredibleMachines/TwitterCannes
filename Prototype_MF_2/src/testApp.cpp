#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
	//set up animation info
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	
    imageNum=1;
    images[0]="twitter-bird.png";
    images[1]="tweet.png";
    images[2]="conan.png";
	images[3]="chanel900x700.jpg";
	images[4]="chanel900x700.png";
	
	
	
	//get the text pixels
	textScreen.loadImage("tweet.png");
	textScreen.setImageType(OF_IMAGE_COLOR);
	//textScreen.resize(90, 70);
    
	
	
	ofBackground(255, 255, 255);	//white background
	
	textPix=textScreen.getPixels();
	
	getPotentials();
	
	particleNum=0;
	populate(0);
    masterTimer=ofGetElapsedTimeMillis();
    bHalf=false;

    
    
	
}


//--------------------------------------------------------------
void testApp::update(){
	//update the particles
	for (int i=particles.size()-1; i>=0; i--)
		particles[i].update();
	
	for (int i=leaving.size()-1; i>=0; i--){
		leaving[i].update();
		if (leaving[i].killMe)
			leaving.erase(leaving.begin()+i);
	}
    
	
	//add or remove particles based on the mouse location
//	float prc= ofMap(mouseX,0,ofGetWidth(),0,1);
    
    if(ofGetElapsedTimeMillis()-masterTimer<3000){
        particleNum+=200;
    }
    
    else if(ofGetElapsedTimeMillis()-masterTimer<6000){

    }
    
    else if(particleNum>0){
        particleNum-=100;
    }
    else{
        if (bHalf==false){
            bHalf=true;
            outTimer=ofGetElapsedTimeMillis();
            particleNum=0;
        }
        else if(ofGetElapsedTimeMillis()-outTimer>4000){
            bHalf=false;
            masterTimer=ofGetElapsedTimeMillis();
        }
    }
	
	//are there too few?
	while (particles.size()<particleNum)
		addParticle();
	//are there too many
	if (particles.size()>particleNum)
		removeParticles(particles.size()-particleNum);
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	//draw the particles
	for (int i=0; i<particles.size(); i++)
		particles[i].draw();
	for (int i=0; i<leaving.size(); i++)
		leaving[i].draw();
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
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
//goes through and finds all of the black pixels, stores them in a (very large) vector
void testApp::getPotentials(){
	for (int x=0; x<textScreen.width; x++){
		for (int y=0; y<textScreen.height; y++){
			//check if this was a black pixel
			// index for grayscale image
			//int index = y*textScreen.width+x;

			// index for color image
			int index = y*3*textScreen.width+x*3;
		
			if (textPix[index] < 2){
//			if (ofRandom(0, 1) < 0.025) {
				//printf("Pixel at %i is %i\n", index, textPix[index]);
				ofPoint newPoint(x,y);
				potentialPoints.push_back(newPoint);
			}
		}
	}
	
	cout << "Potential poits count: " << potentialPoints.size() << endl;
}

//--------------------------------------------------------------
//add a particle at a random spot from the vector of possible locations
void testApp::addParticle(){
	//for the first few frames, just have them appear, not fly in
	bool normalStart=true;
	if (ofGetFrameNum()<30)	normalStart=false;
	
	int index = ofRandom(0,potentialPoints.size()-1);
	ofPoint p = potentialPoints[index];
	ofColor tempColor;

	tempColor.r=textPix[(int)p.y*3*textScreen.width+(int)p.x*3];
	tempColor.g=textPix[(int)p.y*3*textScreen.width+(int)p.x*3+1];
	tempColor.b=textPix[(int)p.y*3*textScreen.width+(int)p.x*3+2];
	
	//cout << textPix[50] << endl;
	//textPix[p.y*textScreen.width+p.x];
	
	Particle newParticle;
	newParticle.setup(potentialPoints[index],
					  tempColor,
					  //ofColor::fromHsb(ofRandom(255), 255, 255),
					  textPix, textScreen.width, normalStart);
	particles.push_back(newParticle);
}

//removes num particles
void testApp::removeParticles(int num){
	//create leaving particles for all of the particles we're about to kill
	for (int i=0; i<num; i++){
		LeavingParticle newLeaver;
		newLeaver.setup(particles[i].loc);
		leaving.push_back(newLeaver);
	}
	
	if (particles.size()>1)	//make sure there is a particle to remove
		particles.erase(particles.begin(),particles.begin()+num);
}

//--------------------------------------------------------------
//clears the particles and adds num
//this function isn't being used
void testApp::populate(int num){
	//first kill all existing particles
//	particles.clear();
//	
//	for (int i=0; i<num; i++){
//		Particle newParticle;
//		newParticle.setup(potentialPoints[ofRandom(0,potentialPoints.size()-1)], textPix, textScreen.width, false);
//		particles.push_back(newParticle);
//	}
	
	
}
