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
    string recv;
    vector<unsigned char> vb;
    vb.assign(1000,345);
    while(1){
        rec.sendString("b1");
        //rec.sendInt(594631);
        //rec.sendBytes(vb);
    }

    return 0;
}
