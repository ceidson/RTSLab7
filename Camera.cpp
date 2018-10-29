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
    cam_height = height;
    cam_width = width;
    capture = new VideoCapture(0);
    capture->set(CV_CAP_PROP_FRAME_WIDTH,cam_width);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT,cam_height);
    capture->set(CV_CAP_PROP_FPS, FRAME_RATE);
}

void Camera::run(){
    //If we are unable to connect then exit
    if(!capture->isOpened()){
        cout << "Failed to connect to the camera." << endl;
        exit(-1);
    }
    lastFrame = new Mat();
    // While thread is to keep running
    //// Grab the next image from the camera
    //// Retrieve the image into the last frame
    while(keepGoing){
        capture->grab();
        capture->retrieve(lastFrame);
    }

}

void Camera::shutdown(){
    // Set keepGoing to false
    keepGoing = false;
}

Mat* Camera::takePicture(){
    Mat *imageCopy = new Mat();
    // If the last frame was empty, return NULL
    if(countNonZero(lastFrame) < 1){
        cout << "Last frame is empty!" << endl;
        return NULL;
    }else{
        mtx.lock();
        //Instantiate a new matrix
        // Copy the data from the last frame into the return frame
        lastFrame->copyTo(*imageCopy);
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

