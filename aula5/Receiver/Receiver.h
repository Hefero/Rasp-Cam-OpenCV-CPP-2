#ifndef RECEIVER_H
#define RECEIVER_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>
#include <vector>
using namespace std;
using namespace cv;

using namespace cv;

class Receiver
{
private:    
    int sokt;
    char* serverIP;
    int  serverPort;
    struct  sockaddr_in serverAddr;
    socklen_t           addrLen = sizeof(struct sockaddr_in);
    int key;    

public:
    std::vector<unsigned char> buffer;
    Receiver(int argc, char** argv);
    ~Receiver();
    bool closeSocket();
    int sendInt(int value);
    int sendMat(Mat img);
    int sendBytes(vector<unsigned char> compressed);
    int sendString(string value);
    int recvString(string& storage);
    int recvInt(int& value);
    int recvMat(Mat& mat);
    int recvBytes(vector<unsigned char>& compressed);
};

#endif