/*
 * Camera.cpp
 *
 *  Created on: Oct 25, 2018
 *      Author: se3910
 */

#include "Camera.h"
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define FRAME_RATE (30)

Camera::Camera(string threadName, int width, int height): RunnableClass(threadName) {
    cout << "Start Making Camera" << endl;
    cam_height = height;
    cam_width = width;
    capture = new VideoCapture(0);
    capture->set(CV_CAP_PROP_FRAME_WIDTH,cam_width);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT,cam_height);
    capture->set(CV_CAP_PROP_FPS, FRAME_RATE);
    cout << "End Making Camera" << endl;
}

void Camera::run(){
   // cout << " Start Camera run" << endl;
    //If we are unable to connect then exit
    if(!capture->isOpened()){
        cout << "Failed to connect to the camera." << endl;
        exit(-1);
    }
    cout << "Connection to cam successful" << endl;
    lastFrame = new Mat();
    // While thread is to keep running
    //// Grab the next image from the camera
    //// Retrieve the image into the last frame
    while(keepGoing){
	//cout << "In the loop" << endl;
        capture->grab();
        mtx.lock();
	//cout << "In the mtx lock" << endl;
        capture->retrieve(*lastFrame);
	//cout << "Last frame rows: " << lastFrame->rows << "cols: " << lastFrame->cols << endl;
        mtx.unlock();
    }
    cout << " End Camera run" << endl;
}

void Camera::shutdown(){
    // Set keepGoing to false
    cout << "Camera shutdown" << endl;

    keepGoing = false;
}

Mat* Camera::takePicture(){
    cout << " Start Camera Take Pic" << endl;

   
    // If the last frame was empty, return NULL
    mtx.lock();

    if(lastFrame == NULL || lastFrame->cols <=0){
        cout << "Last frame is empty!" << endl;
        mtx.unlock();
        return NULL;
    }else{
        //cout << " last frame is not empty" << endl;
	Mat *imageCopy = new Mat();
        //Instantiate a new matrix
        // Copy the data from the last frame into the return frame
        lastFrame->copyTo(*imageCopy);
	//cout << "imageCopy rows: " << imageCopy->rows << "imageCopy cols: " << imageCopy->cols <<endl;
        mtx.unlock();
        // Return the frame
        return imageCopy;
        // This method will need to be properly synchronized

        // Return matrix of the picture that was last grabbed from the camera
    }

}

Camera::~Camera() {
    delete lastFrame;
    delete capture;
}

