#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "Particle.h"
#include "LeavingParticle.h"

class testApp : public ofBaseApp{
	
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
	
	void getPotentials();	//fills up the potentialPoints vector
	void populate(int num);	//not being used
	void addParticle();
	void removeParticles(int num);
    
    int masterTimer;
    int outTimer;
    int particleNum;
    bool bHalf;
    int imageNum;
    string images[3];
	
	//text
	ofImage textScreen;
	unsigned char * textPix;



	
	
	//particles
	vector <ofPoint> potentialPoints;	//vector of all of the black pixel locations
	vector <Particle> particles;
	vector <LeavingParticle> leaving;			//the particles currently flying away
	
	
    
};

#endif	

