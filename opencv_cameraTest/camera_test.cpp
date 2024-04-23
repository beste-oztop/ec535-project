#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	    VideoCapture capture(0);
	    if (!capture.isOpened()) {
	        return -1;
	    }
	    capture.set(CAP_PROP_FRAME_WIDTH, 640);
	    capture.set(CAP_PROP_FRAME_HEIGHT, 480);
	    capture.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));

	    Mat frame;
	    Mat edges;
	    while (true) {
	    	capture >> frame;
	        if (!frame.empty()) {
	         break;
		}
	     cvtColor(frame, edges, COLOR_BGR2GRAY);									        }
				    imwrite("frame.png", frame);
				        return 0;
}

