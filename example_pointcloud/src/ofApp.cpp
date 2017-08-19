#include "ofMain.h"
#include "ofxMultiKinectV2.h"


// dan buzzo - added updates to render color on point cloud
//========================================================================
class ofApp : public ofBaseApp{
    ofxMultiKinectV2 kinect0;
    
    ofEasyCam ecam;
    ofVboMesh mesh;
    
    ofPixels colorImage;
    
    float irXres = 512;
    float irYres = 424;
    float colorXres = 1920;
    float colorYres = 1080;
    
    float irXdeg = 70.6;
    float irYdeg = 60;
    float colorXdeg = 84.1;
    float colorYdeg = 53.8;
    
    
public:
    
    bool paintMesh;
    bool myColor;
    
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
        
        paintMesh = true;
        myColor = true;
        
    }
    
    void update() {
        kinect0.update();
        if (kinect0.isFrameNew()) {
            mesh.clear();
            
            colorImage = kinect0.getColorPixelsRef();
            int step = 1;
            int h = kinect0.getDepthPixelsRef().getHeight();
            int w = kinect0.getDepthPixelsRef().getWidth();
            float cW = colorImage.getWidth();
            float cH = colorImage.getHeight();
            //cout << w << " width " << "scale " << cW << "height " << h <<  "scale :" << cH << endl;
            //   for(int y = 0; y < h; y += step) {
            //     for(int x = 0; x < w; x += step) {
            
            int startH = irXres * ((irXdeg - colorXdeg) / 2)/irXdeg;
            int endH = irXres - startH;
            
            int startW = 0;
            int endW = kinect0.getDepthPixelsRef().getWidth();;
            
            int startColorW = colorXres * (((colorXdeg - irXdeg)/2)/colorXdeg);
            int endColorW = colorXres - startColorW;
          //  float dist;

            
            for(int y = startH; y < endH; y += step) {
                for(int x = startW; x < endW; x += step) {
                    
                  //dist =  kinect0.getDistanceAt(x, y);
                   float dist = kinect0.getDistanceAt(x, y);
                    
                    if(dist > 1 && dist < 5000) {
                        ofVec3f pt = kinect0.getWorldCoordinateAt(x, y, dist);
                        
                        ofColor c;
                        if (paintMesh){
                            int colX = x * 3.75;
                            int colY = y * 2.55;
                            if (myColor) {
                                c =  colorImage.getColor(colX, colY);
                            } else {
                                
                                c = kinect0.getColorAt(startColorW + (x / irXres) * (endColorW - startColorW), (y / irYres) * colorYres);
                            }
                        } else {
                            // cout << c << " " << colX << " " << colY << endl;
                            float h = ofMap(dist, 50, 200, 0, 255, true);
                            c.setHsb(h, 255, 255);
                        }
                        mesh.addColor(c);
                        mesh.addVertex(pt);
                    }
                }
            }
            
        }
    }
    
    void draw() {
        ofClear(0);
        
        if (mesh.getVertices().size()) {
            ofPushStyle();
            glPointSize(1);
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
        ofDrawBitmapStringHighlight("using my measure : " + to_string(myColor) , 10, 60);

    }
    
    void keyPressed(int key) {
        if (key == 'p') {
            paintMesh =  !paintMesh;
            
        }
        if (key == 'c') {
            myColor =  !myColor;
            
        }
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

