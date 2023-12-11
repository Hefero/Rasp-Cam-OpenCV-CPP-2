/**
 * OpenCV video streaming over TCP/IP
 * Client: Receives video from server and display it
 * by Steve Tuenkam
 */

#include "opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include "Transmitter/Transmitter.cpp"

using namespace cv;

int main(int argc, char** argv)
{
    if (argc < 3) {
           std::cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << std::endl;
    }
    
    Transmitter rec(argc, argv);
    vector<unsigned char> vc;
    vc.assign(100,123);
    
    while(1){
        rec.sendBytes(vc);
        int bytesRec;
        //rec.recvInt(bytesRec);
        //std::cout << "Receveid confirmation of bytes: " << bytesRec << std::endl;
    }

    return 0;
}
