#include "ofMain.h"
#include "ofxMultiKinectV2.h"


// dan buzzo - added updates to render color on point cloud
//========================================================================
class ofApp : public ofBaseApp{
    ofxMultiKinectV2 kinect0;
    
    ofEasyCam ecam;
    ofVboMesh mesh;
    
    ofPixels colorImage;
public:
    
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
        
        kinect0.open(true, true, 0);
        // Note :
        // Default OpenCL device might not be optimal.
        // e.g. Intel HD Graphics will be chosen instead of GeForce.
        // To avoid it, specify OpenCL device index manually like following.
        // kinect1.open(true, true, 0, 2); // GeForce on MacBookPro Retina
        
        kinect0.start();
        
        mesh.setUsage(GL_DYNAMIC_DRAW);
        mesh.setMode(OF_PRIMITIVE_POINTS);
        
        ecam.setAutoDistance(false);
        ecam.setDistance(200);
        
        
    }
    
    void update() {
        kinect0.update();
        if (kinect0.isFrameNew()) {
            mesh.clear();
            
            colorImage = kinect0.getColorPixelsRef();
            
            {
                int step = 1;
                int h = kinect0.getDepthPixelsRef().getHeight();
                int w = kinect0.getDepthPixelsRef().getWidth();
                float cW = colorImage.getWidth();
                float cH = colorImage.getHeight();
                //cout << w << " width " << "scale " << cW << "height " << h <<  "scale :" << cH << endl;
                for(int y = 0; y < h; y += step) {
                    for(int x = 0; x < w; x += step) {
                        float dist = kinect0.getDistanceAt(x, y);
                        if(dist > 50 && dist < 5000) {
                            ofVec3f pt = kinect0.getWorldCoordinateAt(x, y, dist);
                            
                            ofColor c;
                            int colX = x * 3.75;
                            int colY = y * 2.55;
                           c =  colorImage.getColor(colX, colY);
                           // cout << c << " " << colX << " " << colY << endl;
                            float h = ofMap(dist, 50, 200, 0, 255, true);
                            //c.setHsb(h, 255, 255);
                            mesh.addColor(c);
                            mesh.addVertex(pt);
                        }
                    }
                }

            }
        }
    }
    
    void draw()
    {
        ofClear(0);
        
        if (mesh.getVertices().size()) {
            ofPushStyle();
            glPointSize(2);
            ecam.begin();
            ofDrawAxis(100);
            ofPushMatrix();
            ofTranslate(0, 0, -100);
            mesh.draw();
            ofPopMatrix();
            ecam.end();
            ofPopStyle();
        }
        
        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
        ofDrawBitmapStringHighlight("Device Count : " + ofToString(ofxMultiKinectV2::getDeviceCount()), 10, 40);
    }
};

//#include "ofAppGLFWWindow.h"
//========================================================================
int main( ){
    ofSetupOpenGL(1920,1080,OF_WINDOW);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
