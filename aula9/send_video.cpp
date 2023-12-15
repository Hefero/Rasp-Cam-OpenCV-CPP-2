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
#include "MotorH/MotorH.cpp"
#include <wiringPi.h>
#include <softPwm.h>

using namespace cv;

int capDev = 0;
VideoCapture cap(capDev); // open the default camera
motorH motor;

int main(int argc, char** argv)
{
    if (argc < 3) {
           std::cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << std::endl;
    }
    //motor.stop();

    Transmitter rec(argc, argv);

    Mat img;
    img = Mat::zeros(480 , 640, CV_8UC3);
    vector<unsigned char> vb;
    vector<int> param{CV_IMWRITE_JPEG_QUALITY,100};
    std::chrono::steady_clock::time_point beginL = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point endL = std::chrono::steady_clock::now(); 
    int durationL = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();    
    while(1){        
        try{
            cap >> img;
            imencode(".jpg",img,vb,param);
            rec.sendBytes(vb);
            int bytesRec;
            
            string command;
            int bytesCommand = rec.recvString(command);
            bool did = false;
            if(command.size() > 1){                                
                if(!did){
                    did = true;
                    motor.execute(command, motor);
                    beginL = std::chrono::steady_clock::now();
                    endL = std::chrono::steady_clock::now();
                }
                else{                    
                    durationL = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                    if (durationL > 100 ){
                        did = false;                        
                    }
                    motor.stop();
                    endL = std::chrono::steady_clock::now();
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
