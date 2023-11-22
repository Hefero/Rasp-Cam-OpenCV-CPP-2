#include "Receiver.h"

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

bool Receiver::closeSocket(){
     
    int option = 1;    
    setsockopt(sokt, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    close(sokt);
    return true;
}
int Receiver::sendInt(int value){
    return send(sokt, &value, sizeof(value), MSG_WAITALL); 
}
int Receiver::sendMat(Mat img){
    return send(sokt, img.data, img.total() * img.elemSize(), MSG_WAITALL); 
}
int Receiver::sendBytes(vector<unsigned char> compressed){
    sendInt(594631);
    sendInt(compressed.size());
    return send(sokt, compressed.data(), compressed.size(), MSG_WAITALL); 
}
int Receiver::sendString(string dataToSend){
    sendInt(866685);    
    sendInt(dataToSend.size());
    return send(sokt,dataToSend.c_str(),dataToSend.size(),MSG_WAITALL); // Send the string 
                                                            
}

int Receiver::recvString(string& storage){
    int bytes = 0;
    int recInt = 0;
    int bytesSize = 0;
    int stringSize = 0;
    if (bytes = recvInt(recInt) > 0) {
        //std::cout << "received int = " << recInt << std::endl;
        switch(recInt) 
        {
         case 866685: // string
            if (int recString = recvInt(stringSize) > 0) {
                string command;                
                int result = 0;
                const unsigned int MAX_BUF_LENGTH = 4096;
                std::vector<char> buffer(MAX_BUF_LENGTH);
                string receivedString;
                int bytesReceived = 0;
                do {
                    bytesReceived = recv(sokt, &buffer[0], buffer.size(), 0);
                    // append string from buffer.
                    if ( bytesReceived == -1 ) { 
                        // error 
                    } else {
                        receivedString.append( buffer.cbegin(), buffer.cend() );
                    }
                } while ( bytesReceived == MAX_BUF_LENGTH );
                storage = receivedString;
                result = bytesReceived;
                if (result == stringSize) {                
                    //std::cout << "received command: " << storage << std::endl;
                    //sendInt(stringSize); //send confirmation of received                    
                    return result;       
                } else {
                    return -1;
                }
            }
            break;
        default:
            break;
        }
    }
    return recInt;
}
int Receiver::recvInt(int& storage){
    return recv(sokt, &storage, sizeof storage, MSG_WAITALL);
}

int Receiver::recvMat(Mat& storage){
    return recv(sokt, storage.data, storage.total() * storage.elemSize(), MSG_WAITALL); 
}


int Receiver::recvBytes(vector<unsigned char>& storage){
    int bytes = 0;
    int recInt = 0;
    int bytesSize = 0;
    int stringSize = 0;
    if (bytes = recvInt(recInt) > 0) {
        //std::cout << "received int = " << recInt << std::endl;
        switch(recInt) 
        {
        case 594631: // bytes
            if (int recBytes = recvInt(bytesSize) > 0) {
                storage.resize(bytesSize);
                int result = recv(sokt, storage.data(), bytesSize, MSG_WAITALL);
                if (result == bytesSize) {                
                    std::cout << "received number of bytes: " << result << std::endl;
                    //sendInt(bytesSize); //send confirmation of received
                    return result;       
                } else {
                    return -1;
                }
            }
            break;
        default:
            break;
        }
    }
    return recInt;
}
// Transmitter constructor
Receiver::~Receiver()
{
    close(sokt);
}