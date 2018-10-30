//
// Created by dixonnj on 10/25/2018.
//
#include <iostream>
#include "ImageCapturer.h"
#include <ctime>
#include <ratio>
#include <chrono>

#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

ImageCapturer::ImageCapturer(std::string threadName, Camera *referencedCamera, ImageTransmitter *trans, int width, int height):PeriodicTask("ImageCapturerPeriodicTask") {
    cout << "Image Capturer started creating" << endl;
    myCamera = referencedCamera;
    myTrans = trans;
    imageHeight = height;
    imageWidth = width;
    size = new Size(width, height);
    cout << "Image Capturer started end" << endl;

}

ImageCapturer::~ImageCapturer() {


}

void ImageCapturer::run() {
    cout << "Image Capturer started run" << endl;

    std::chrono::high_resolution_clock::time_point timeBeforePic;
    std::chrono::high_resolution_clock::time_point timeAfterPic;
    std::chrono::high_resolution_clock::time_point timeAfterResize;
    std::chrono::high_resolution_clock::time_point timeAfterTransmit;

    //  1. Obtain the time since the epoch from the system clock in ms.
    timeBeforePic =  std::chrono::high_resolution_clock::now();

    //  2. Take the picture from the camera.
    Mat* picture = myCamera->takePicture();
    //  3. If the image is not null,
    if(picture == NULL){
        return;
    }
    //  3a. Obtain the time since the epoch from the system clock in ms.
    timeAfterPic =  std::chrono::high_resolution_clock::now();

    //  3b. Resize the image according to the desired size, if a resize needs to occur.
    cout << "Size height is: " << size->height << " size width: " << size->width <<endl;

    if(picture->rows != size->height || picture->cols != size->width){
	Mat* newPic = new Mat();
        cout << "We about to resize" << endl;
        resize(*picture, *newPic, *size);
        cout << "We resized" <<endl;
	picture = newPic;
    }
    cout << "We made it past resize" << endl;


    //  4. Obtain the time since the epoch from the system clock in ms.
    timeAfterResize =  std::chrono::high_resolution_clock::now();

    //  5. Stream the image to the remote device.
    myTrans->streamImage(picture);

    //  6. Delete the image that was taken.
    picture = nullptr;

    //  7. Obtain the time since the epoch from the system clock in ms.
    timeAfterTransmit =  std::chrono::high_resolution_clock::now();

    //  8. Print out to the console in ms the amount of time it took to grab the picture, resize the picture, and transmit the picture in ms.
    std::chrono::duration<double, std::milli> grabPicTimespan = timeAfterPic - timeBeforePic;
    std::chrono::duration<double, std::milli> resizePicTimespan = timeAfterResize - timeAfterPic;
    std::chrono::duration<double, std::milli> transmitPicTimespan = timeAfterTransmit - timeAfterResize;

    std::cout << "Time to grab picture: " << grabPicTimespan.count() << "ms" << std::endl;
    std::cout << "Time to resize picture: " << resizePicTimespan.count() << "ms" << std::endl;
    std::cout << "Time to transmit picture: " << transmitPicTimespan.count() << "ms" << std::endl;

    cout << "Image Capturer ended run" << endl;

}

