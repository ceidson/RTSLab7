//
// Created by eidsoncd on 10/29/2018.
//

#include "ImageTransmitter.h"
#include "time_util.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>


using namespace std;
using namespace cv;

ImageTransmitter::ImageTransmitter(char *machineName, int port) {
    destinationMachineName = machineName;
    myPort = port;
}

ImageTransmitter::~ImageTransmitter() {

}

int ImageTransmitter::streamImage(Mat *image) {
    if(countNonZero(lastFrame) > 1 && destinationMachineName != NULL){
        // Increment the image count
        imageCount++;
        // Initialize the socket sockfd to be a DGRAM
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        // If there is an error, abort with error message
        if(sockfd < 0){
            cout << "There was an error opening socket" << endl;
            exit(-1);
        }
        // Get host by name and set up server instance
        server = gethostbyname(destinationMachineName);
        // If the server is NULL, exit
        if(server == NULL){
            cout << "Server was NULL" << endl;
            exit(-1);
        }
        // Set up the rest of the UDP parameters
        // Zero out the structure.
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        // Now copy the appropriate data over.
        memcpy((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);

        serv_addr.sin_port = htons(myPort);
        // Obtain the image rows, columns, and message size (which is 3 * columns + 24)
        Size s = image.size();
        rows = s.height;
        cols = s.wdith;
        size = 3 * cols + 24;
        // Allocate a buffer of message size length on the heap
        char* msg = new char[size];
        // Obtain the current timestamp in ms using the time_util library
        int startTime = current_timestamp();
        //  Iterate (using `index' over the even rows of the image, creating a message to send which consists of the following general structure:
        int index = 0;
        for(index = 0; index < rows; index +=2){
            int32_t  *hdrPtr = (int32_t*) msg;
            // Integer 0: The start time for the transmission
            hdrPtr[0] = htonl(startTime);
            // Integer 1: The current timestamp for the current portion of the image
            hdrPtr[1] = htonl(current_timestamp());
            // Integer 2: The count of the image
            hdrPtr[2] = htonl(imageCount);
            // Integer 3: The number of rows in the image.
            hdrPtr[3] = htonl(rows);
            // Integer 4: The number of columns in the image
            hdrPtr[4] = htonl(columns);
            // Integer 5: The index being sent An array of columns * 3 bytes, representing the pixels in the current row.
            hdrPtr[5] = htonl(index);

            memcpy(&hdrPtr[6], &image->data[(index * columns * 3)], columns * 3);
            serverlen = sizeof(serv_addr);

            int lres = sendto(sockfd, msg, size, 0, (struct sockaddr *) &serv_addr, serverlen);

            if(lres < 0){
                cout << "c " << lres <<endl;
                cout << "Transmit" << endl;
            }
        }

        for(index = 1; index < rows; index +=2){
            int32_t  *hdrPtr = (int32_t*) msg;
            // Integer 0: The start time for the transmission
            hdrPtr[0] = htonl(startTime);
            // Integer 1: The current timestamp for the current portion of the image
            hdrPtr[1] = htonl(current_timestamp());
            // Integer 2: The count of the image
            hdrPtr[2] = htonl(imageCount);
            // Integer 3: The number of rows in the image.
            hdrPtr[3] = htonl(rows);
            // Integer 4: The number of columns in the image
            hdrPtr[4] = htonl(columns);
            // Integer 5: The index being sent An array of columns * 3 bytes, representing the pixels in the current row.
            hdrPtr[5] = htonl(index);

            memcpy(&hdrPtr[6], &image->data[(index * columns * 3)], columns * 3);
            serverlen = sizeof(serv_addr);

            int lres = sendto(sockfd, msg, size, 0, (struct sockaddr *) &serv_addr, serverlen);

            if(lres < 0){
                cout << "c " << lres <<endl;
                cout << "Transmit" << endl;
            }
        }

        free(msg);
        close(sockfd);

        // Send to message to the destination
    }else{

    }
}
