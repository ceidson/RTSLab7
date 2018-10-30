//
// Created by dixonnj on 10/25/2018.
//

#include "PeriodicTask.h"
#include "ImageTransmitter.h"
#include "Camera.h"


#ifndef C_IMAGECAPTURER_H

class ImageCapturer : PeriodicTask{
public:
    ImageCapturer(std::string threadName, Camera* referencedCamera, ImageTransmitter* trans, int width, int height):PeriodicTask(threadName);
    virtual ~ImageCapturer();
    void run();
private:
    Camera* myCamera;
    ImageTransmitter* myTrans;
    int imageWidth;
    int imageHeight;
    Size* size;
};

#define C_IMAGECAPTURER_H

#endif //C_IMAGECAPTURER_H
