/**
 * OpenCV video streaming over TCP/IP
 * Client: Receives video from server and display it
 * by Steve Tuenkam
 */

#include "opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <chrono>
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
    
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();


    Transmitter rec(argc, argv);

    Mat img;
    img = Mat::zeros(480 , 640, CV_8UC3);
    vector<unsigned char> vb;
    vector<int> param{CV_IMWRITE_JPEG_QUALITY,80};    
    
    while(1){        
        try{
            cap >> img;
            imencode(".jpg",img,vb,param);
            rec.sendBytes(vb);
            int bytesRec;
            rec.recvInt(bytesRec);
            if (bytesRec == vb.size()){
                start = std::chrono::high_resolution_clock::now();
                std::cout << "Received confirmation of bytes: " << bytesRec << std::endl;
            }
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            if(duration.count() > 3){
                std::cout << "Lost Connection!" << std::endl;
                
                int tries = 3;
                auto startRetry = std::chrono::high_resolution_clock::now();            
                auto endRetry = std::chrono::high_resolution_clock::now();
                rec.closeSocket();
                rec.~Transmitter();
                while(tries > 0){                
                    endRetry = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> durationRetry = endRetry - startRetry;
                    if(durationRetry.count() > 10) {
                        std::cout << "Retrying Connection" << std::endl;
                        rec = Transmitter(argc, argv);
                        startRetry = std::chrono::high_resolution_clock::now();
                        tries--;
                    }
                }
            }
        }
        catch(cv::Exception ex){
            std::cout << "encoding error " << ex.msg << std::endl;
            continue;
        }
    }

    return 0;
}
