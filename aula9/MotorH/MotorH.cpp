#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <chrono>
#include <arpa/inet.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "MotorH.h"


#define velDir 100
#define velEsq 60
#define LEFT_TURN_DISTANCE 1
#define RIGHT_TURN_DISTANCE 1


motorH::motorH()

{
   wiringPiSetup();
   softPwmCreate(22, 0, 100);
   softPwmCreate(23, 0, 100);
   softPwmCreate(24, 0, 100);
   softPwmCreate(25, 0, 100);   
}

void motorH::moveForward()
{
    moving = true;
    motorDir(1, velDir); 
    motorEsq(1, velEsq);
}
void motorH::moveBackwards()
{
    moving = true;
    motorDir(-1, velDir); 
    motorEsq(-1, velEsq);
}

void motorH::stop(void)
{
    moving = false;
    motorDir(0, 0); // inicia os motores em ponto morto
    motorEsq(0, 0); // inicia os motores em ponto morto
   
}

void motorH::hardStop(void)
{
    moving = false;
    motorDir(2, 0); // trava os motores
    motorEsq(2, 0); // trava os motores    
}

void motorH::turnLeft(void)
{
    moving = true;
    motorDir(1, velDir);
    motorEsq(-1, velEsq);
}

void motorH::turnRight(void)
{
    moving = true;
    motorDir(-1, velDir);
    motorEsq(1, velEsq);
}   

void motorH::motorDir(int command, int duty)
{
    switch(command) 
    {
    case 0: // ponto morto
        softPwmWrite(22, 0);
        softPwmWrite(23, 0);
        break;
    case 1: // move forward
        softPwmWrite(22, duty);
        softPwmWrite(23, 0);
        break;
    case -1: // move backwards
        softPwmWrite(22, 0);
        softPwmWrite(23, duty);
        break;
    case 2: // hard break
        softPwmWrite(22, 100);
        softPwmWrite(23, 100);
        break;
    }
}

void motorH::motorEsq(int command, int duty)
{
    switch(command) 
    {
    case 0: // ponto morto
        softPwmWrite(24, 0);
        softPwmWrite(25, 0);
        break;
    case 1: // move forward
        softPwmWrite(24, 0);
        softPwmWrite(25, duty);
        break;
    case -1: // move backwards
        softPwmWrite(24, duty);
        softPwmWrite(25, 0);
        break;
    case 2: // hard break
        softPwmWrite(24, 100);
        softPwmWrite(25, 100);
        break;
    }
}

void motorH::execute(string& command, motorH& motor)
{
    int delayTime = 40;
    if (strcmp(command.data(),lastCommand.data()) != 0){
        lastCommand = command;
        beginL = std::chrono::steady_clock::now();
    }
    if (getDurationL() <= delayTime){        
        endL = std::chrono::steady_clock::now();
        if(command.size() > 1){            
            char stop[4096] = "stop";    
            if( strcmp(command.data(),stop) == 0){            
                std::cout << command << std::endl;
                motorH::stop();
            }
            char b1[4096] = "b1";    
            if( strcmp(command.data(),b1) == 0 ){
                motorDir(1, velDir);
                motorEsq(1, velEsq/2);
                std::cout << command << std::endl;
            }
            char b2[4096] = "b2";
            if( strcmp(command.data(),b2) == 0 ){
                moveForward();
                std::cout << command << std::endl;
            }
            char b3[4096] = "b3";    
            if( strcmp(command.data(),b3) == 0 ){
                turnLeft();
                std::cout << command << std::endl;
            }
            char b4[4096] = "b4";    
            if( strcmp(command.data(),b4) == 0 ){
                hardStop();
                std::cout << command << std::endl;
            }
            char b5[4096] = "b5";    
            if( strcmp(command.data(),b5) == 0 ){
                motorDir(-1, velDir);
                motorEsq(-1,velEsq/2);
            }
            char b6[4096] = "b6";    
            if( strcmp(command.data(),b6) == 0 ){
                moveBackwards();
                std::cout << command << std::endl;
            }
            char b7[4096] = "b7";    
            if( strcmp(command.data(),b7) == 0 ){
                motorDir(1, velDir/2);
                motorEsq(1, velEsq);            
                std::cout << command << std::endl;
            }
            char b8[4096] = "b8";    
            if( strcmp(command.data(),b8) == 0 ){
                turnRight();
                std::cout << command << std::endl;
            }
            char b9[4096] = "b9";    
            if( strcmp(command.data(),b9) == 0 ){
                motorDir(-1, velDir/2);
                motorEsq(-1, velEsq); 
                std::cout << command << std::endl;
            }
        }
    }
    else{        
        stop();
        if (getDurationL() > delayTime*2){
            beginL = std::chrono::steady_clock::now();
            endL = std::chrono::steady_clock::now();
            
        }
        else{
            endL = std::chrono::steady_clock::now();
        }        
    }
}

void motorH::debug(void)  // debug function
{
    //_serial.printf("\n\resq:%d dir%d dist:%dmm",pulsoEsq,pulsoDir,getSonicDistance());
}
