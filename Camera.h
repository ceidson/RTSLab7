/*
 * Camera.h
 *
 *  Created on: Oct 25, 2018
 *      Author: se3910
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "RunnableClass.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
using namespace cv;

class Camera: public RunnableClass {
public:
	Camera(string threadName, int width,int height);
	virtual ~Camera();
	void run();
	void shutdown();
	Mat* takePicture();
private:
	VideoCapture* capture;
	Mat* lastFrame;
	mutex mtx;
};

#endif /* CAMERA_H_ */
