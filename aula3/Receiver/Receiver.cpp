#include "Receiver.h"


void Receiver::waitConnection() 
{
    int bytes = 0;
    int recInt; 
    if (bytes = recvInt(recInt) > 0) {
        buffer.resize(recInt);
        int result = recvBytes(buffer, recInt);
        if (result == recInt) {                
            std::cout << "received bytes = " << (int)buffer[0] << " size:" << result << std::endl;
            sendInt(recInt);              
        } else {
        // Handle error
        }
    }
}

Receiver::Receiver(int argc, char** argv)
{
    if (argc < 3) {
           std::cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << std::endl;
    }

    serverIP   = argv[1];
    serverPort = atoi(argv[2]);

    if ((sokt = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket() failed" << std::endl;
    }

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    if (connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {
        std::cerr << "connect() failed!" << std::endl;
    }
    
}

int Receiver::sendInt(int value){
    return send(sokt, &value, sizeof(value), MSG_WAITALL); 
}
int Receiver::sendMat(Mat img){
    return send(sokt, img.data, img.total() * img.elemSize(), MSG_WAITALL); 
}
int Receiver::sendBytes(vector<unsigned char> compressed){
    sendInt(compressed.size());
    return send(sokt, compressed.data(), compressed.size(), MSG_WAITALL); 
}
int Receiver::recvInt(int& storage){
    return recv(sokt, &storage, sizeof storage, MSG_WAITALL);
}

int Receiver::recvMat(Mat& storage){
    return recv(sokt, storage.data, storage.total() * storage.elemSize(), MSG_WAITALL); 
}

int Receiver::recvBytes(vector<unsigned char>& storage, int size){
    return recv(sokt, storage.data(), size, MSG_WAITALL); 
}
// Transmitter constructor
Receiver::~Receiver()
{
    close(sokt);
}