//
// Created by eidsoncd on 10/29/2018.
//

#ifndef CREPO_IMAGETRANSMITTER_H
#define CREPO_IMAGETRANSMITTER_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

using namespace std;
using namespace cv;

class ImageTransmitter {
private:
    int myPort = 5000;
    int sockfd = 0;
    char* destinationMachineName = NULL;
    int imageCount = 0;
    struct hostnet* server;
    int serverlen;
    struct sockaddr_in serv_addr;
public:
    ImageTransmitter(char* machineName, int port);
    virtual ~ImageTransmitter();
    int streamImage(Mat* image);
};
#endif //CREPO_IMAGETRANSMITTER_H
