#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>
#include <vector>
#include <cekeikon.h>
class Transmitter
{
private:    
    int             localSocket,
                    remoteSocket,
                    port = 4097;                            

    struct  sockaddr_in localAddr,
                        remoteAddr;
    pthread_t thread_id;


    int addrLen = sizeof(struct sockaddr_in);

public:
    std::vector<unsigned char> buffer;
    Transmitter(int argc, char** argv);
    ~Transmitter();
    int sendInt(int value);
    int sendMat(Mat img);
    bool closeSocket();
    int sendBytes(vector<unsigned char> compressed);
    int sendString(string value);
    int recvString(string& storage);
    int recvInt(int& value);
    int recvMat(Mat& mat);
    int recvBytes(vector<unsigned char>& compressed);
};

#endif