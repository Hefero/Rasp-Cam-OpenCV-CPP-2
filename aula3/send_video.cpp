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

int capDev = 0;
VideoCapture cap(capDev); // open the default camera

int main(int argc, char** argv)
{
    if (argc < 3) {
           std::cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << std::endl;
    }
    
    Transmitter rec(argc, argv);

    Mat img, imgGray;
    img = Mat::zeros(480 , 640, CV_8UC3);
     //make it continuous
    if (!img.isContinuous()) {
        img = img.clone();
    }
    int imgSize = img.total() * img.elemSize();
    vector<unsigned char> vb;
    vector<int> param{CV_IMWRITE_JPEG_QUALITY,80};    
    
    while(1){
        cap >> img;
        imencode(".jpg",img,vb,param);
        rec.sendBytes(vb);
        int bytesRec;
        rec.recvInt(bytesRec);
        std::cout << "Received confirmation of bytes: " << bytesRec << std::endl;
    }

    return 0;
}
