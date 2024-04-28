#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
 {
    cv::Mat image= cv::imread(argv[1]);
    if (!image.data) {
        std::cout << "Image file not found\n";
        return 1;
    }

    int hue_low = 170;
    int hue_high = 179;

    int sat_low = 150; 
    int sat_high = 255;

    int val_low = 60;
    int val_high = 255;

    int x_old = -1; 
    int y_old = -1;

    //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros( image.size(), CV_8UC3 );;

    // Convert the contour.jpg from BGR to HSV
    Mat imgHSV; 
    cvtColor(image, imgHSV, COLOR_BGR2HSV); 
 
    // Make sure the image is in the threshold
    Mat imgThresholded;
    inRange(imgHSV, Scalar(hue_low, sat_low, val_low), Scalar(hue_high, sat_high, val_high), imgThresholded); 
      
    //morphological opening (removes small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

    //morphological closing (removes small holes from the foreground)
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //Calculate the moments of the thresholded image
    Moments oMoments = moments(imgThresholded);

    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;

   // if the area is super small, no objects - noisy!
    if (dArea > 10000)
    {
        // calculate the position of the red object
        int posX = dM10 / dArea;
        int posY = dM01 / dArea;        
        
        x_old = posX;
        y_old = posY;
    }

    printf("x coord: %d, y coord: %d ",x_old, y_old);
    cv::imwrite("output_red.png", imgThresholded);

   return 0;
}