/*
 * main.cpp
 * This is the main file for the Camera Capture program.
 *      Author: W. Schilling
 */

using namespace std;
#include "ImageTransmitter.h"
#include "Camera.h"
#include "ImageCapturer.h"
#include <chrono>
#include <iostream>
#include <mutex>
#include "RunnableClass.h"
#include <sys/syscall.h>
#include <unistd.h>


using namespace std;

/**
 * This is the main program.  It will instantiate a network manager and robot controller as well as a Command Queue.
 * It will then block until the user enters a message on the console.
 * This will then cause it to shutdown the other classes and wait for their threads to terminate.
 */
int main(int argc, char* argv[]) {
	// These are the image sizes for the camera (c) and the transmitted image (t), both height (h) and width (w).
	int cw, ch, tw, th;

	if (argc != 7)
	{
		printf("Usage: %s ip port cameraWidth cameraHeight TransmitWidth transmitHeight");
		exit(0);
	}

	cout << "Main thread id is : " << syscall(SYS_gettid) << "\n";

	// Convert the parameters into integers.
	cw = atoi(argv[3]);
	ch = atoi(argv[4]);
	tw = atoi(argv[5]);
	th = atoi(argv[6]);


	// Instantiate a camera.
	Camera* myCamera = new Camera("Camera", cw, ch);

	// Figure out the port to use.
	int port = atoi(argv[2]);
	ImageTransmitter* it = new ImageTransmitter(argv[1], port);
	myCamera->start(10);

	// Start capturing and streaming.
	ImageCapturer *is = new ImageCapturer("Image Stream", myCamera, it, tw, th);
	is->setTaskPeriod(100);
	is->start(8);

	string msg;
	cin >> msg;

	while (msg.compare("QUIT")!=0)
	{
		cout << "Looping thread id is : " << syscall(SYS_gettid) << "\n";
		if (msg.compare("P")==0)
		{
			RunnableClass::printThreads();
		}
		else if (msg.compare("R")==0)
		{
			RunnableClass::resetAllThreadInformation();
		}

		cin >> msg;
	}

	myCamera->shutdown();

	delete myCamera;
	delete it;
	delete is;
}
