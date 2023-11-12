#include "Transmitter.h"

Transmitter::Transmitter(int argc, char** argv)
{
    
    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
    if ( (argc > 1) && (strcmp(argv[1],"-h") == 0) ) {
          std::cerr << "usage: ./cv_video_srv [port] [capture device]\n" <<
                       "port           : socket port (4097 default)\n" <<
                       "capture device : (0 default)\n" << std::endl;

          exit(1);
    }

    if (argc == 2) port = atoi(argv[1]);

    localSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (localSocket == -1){
         perror("socket() call failed!!");
    }    

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons( port );

    int option = 1;    
    setsockopt(localSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
         perror("Can't bind() socket");
         //exit(1);
    }

    //Listening
    listen(localSocket , 3);

    std::cout <<  "Waiting for connections...\n"
              <<  "Server Port:" << port << std::endl;

    //accept connection from an incoming client
    //while(1){
    //if (remoteSocket < 0) {
    //    perror("accept failed!");
    //    exit(1);
    //}

     remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
     setsockopt(remoteSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
      //std::cout << remoteSocket<< "32"<< std::endl;
    if (remoteSocket < 0) {
        perror("accept failed!");
        //exit(1);
    } 
    std::cout << "Connection accepted" << std::endl;
    //pthread_create(&thread_id,NULL,display,&remoteSocket);

     //pthread_join(thread_id,NULL);

    //}
    
}

int Transmitter::sendInt(int value){
    return send(remoteSocket, &value, sizeof(value), MSG_WAITALL); 
}
int Transmitter::sendMat(Mat img){
    return send(remoteSocket, img.data, img.total() * img.elemSize(), MSG_WAITALL); 
}
int Transmitter::sendBytes(vector<unsigned char> compressed){
    sendInt(compressed.size());
    return send(remoteSocket, compressed.data(), compressed.size(), MSG_WAITALL); 
}
int Transmitter::recvInt(int& storage){
    return recv(remoteSocket, &storage, sizeof storage, MSG_WAITALL);
}

int Transmitter::recvMat(Mat& storage){
    return recv(remoteSocket, storage.data, storage.total() * storage.elemSize(), MSG_WAITALL); 
}

int Transmitter::recvBytes(vector<unsigned char>& storage, int size){
    return recv(remoteSocket, storage.data(), size, MSG_WAITALL); 
}

bool Transmitter::closeSocket(){
    int option = 1;    
    setsockopt(localSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    setsockopt(remoteSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    close(localSocket);
    close(remoteSocket);
    localSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (localSocket == -1){
         perror("socket() call failed!!");
    }    

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons( port );

    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
         perror("Can't bind() socket");
         //exit(1);
    }

    //Listening
    listen(localSocket , 3);

    std::cout <<  "Waiting for connections...\n"
              <<  "Server Port:" << port << std::endl;

    //accept connection from an incoming client
    //while(1){
    //if (remoteSocket < 0) {
    //    perror("accept failed!");
    //    exit(1);
    //}

     remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);  
      //std::cout << remoteSocket<< "32"<< std::endl;
    if (remoteSocket < 0) {
        perror("accept failed!");
        //exit(1);
    }     
    return true;
}
// Transmitter constructor
Transmitter::~Transmitter()
{
    close(localSocket);
    close(remoteSocket);
}