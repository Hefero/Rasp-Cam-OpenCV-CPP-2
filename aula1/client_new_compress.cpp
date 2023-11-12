/**
 * OpenCV video streaming over TCP/IP
 * Client: Receives video from server and display it
 * by Steve Tuenkam
 */

#include "opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>

using namespace cv;
int recvInt(int socket, int& value);
int recvMat(int socket, Mat& mat);
int recvBytes(int socket, vector<unsigned char>& compressed, int size);
int main(int argc, char** argv)
{

    //--------------------------------------------------------
    //networking stuff: socket , connect
    //--------------------------------------------------------
    int         sokt;
    char*       serverIP;
    int         serverPort;

    if (argc < 3) {
           std::cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << std::endl;
    }

    serverIP   = argv[1];
    serverPort = atoi(argv[2]);

    struct  sockaddr_in serverAddr;
    socklen_t           addrLen = sizeof(struct sockaddr_in);

    if ((sokt = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket() failed" << std::endl;
    }

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    if (connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {
        std::cerr << "connect() failed!" << std::endl;
    }



    //----------------------------------------------------------
    //OpenCV Code
    //----------------------------------------------------------

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
    int recInt;    

    while (key != 'q') {        
        
        // recvInt        
        if (bytes = recvInt(sokt, recInt) > 0) {
            std::vector<unsigned char> buffer(recInt);
            int result = recvBytes(sokt, buffer, recInt);
            if (result == recInt) {                
                std::cout << "received bytes = " << (int)buffer[0] << " size:" << result << std::endl;
                img = imdecode(buffer,1);            
            } else {
            // Handle error
            }
        }        
        cv::imshow("CV Video Client", img);

        if (key = cv::waitKey(10) >= 0) break;
    }   

    close(sokt);

    return 0;
}

int recvInt(int socket, int& storage){
    return recv(socket, &storage, sizeof storage, MSG_WAITALL);
}

int recvMat(int socket, Mat& storage){
    return recv(socket, storage.data, storage.total() * storage.elemSize(), MSG_WAITALL); 
}

int recvBytes(int socket, vector<unsigned char>& storage, int size){
    return recv(socket, storage.data(), size, MSG_WAITALL); 
}