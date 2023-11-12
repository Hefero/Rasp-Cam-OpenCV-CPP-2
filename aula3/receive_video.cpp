/**
 * OpenCV video streaming over TCP/IP
 * Client: Receives video from server and display it
 * by Steve Tuenkam
 */

#include "opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include "Receiver/Receiver.cpp"

using namespace cv;

int main(int argc, char** argv)
{
    if (argc < 3) {
           std::cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << std::endl;
    }
    
    Mat img;
    img = Mat::zeros(480 , 640, CV_8UC3);    
    int imgSize = img.total() * img.elemSize();
    uchar *iptr = img.data;
    int bytes = 0;
    int key;
    
    //make img continuos
    if ( ! img.isContinuous() ) { 
          img = img.clone();
    }
    std::cout << "Image Size:" << imgSize << std::endl;

    namedWindow("CV Video Client",1);

    Receiver rec(argc, argv);
    
    while(key != 'q'){
        
        rec.waitConnection();        
        img = imdecode(rec.buffer,1);

        cv::imshow("CV Video Client", img);
        if (key = cv::waitKey(10) >= 0) break;
    }

    return 0;
}
