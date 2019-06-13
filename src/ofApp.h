#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxPDSP.h"

class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void onCollision(ofxBulletCollisionData& cdata);
    
        ofxBulletWorldRigid     world;
        ofxBulletBox            ground;
    
        ofxBulletSphere *       sphere;
        bool                    bColliding;
        vector <ofxBulletBox*>  bounds;
        ofxBulletCustomShape*   boundsShape;
        ofMaterial              boundsMat;
        float                   boundsWidth;
    bool bDropBox;
    
        pdsp::Engine            engine;
        pdsp::VAOscillator      osc;
        pdsp::LFO               lfo;
        pdsp::Amp               amp;
        pdsp::AHR              env;
        pdsp::TriggerControl    gate_ctrl;
        pdsp::ValueControl      pitch_ctrl;
    
        ofCamera camera;
    
    private:
    
};
