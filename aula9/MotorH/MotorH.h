#ifndef MOTORH_H
#define MOTORH_H

#include <string>
#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include "opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <chrono>
#include <arpa/inet.h>
#include <unistd.h>



class motorH
{
    public:
        string lastCommand;
        motorH();
        void motorDir(int command, int duty);
        void motorEsq(int command, int duty);
        void moveForward();
        void moveBackwards();
        void turnLeft();
        void turnRight();
        void stop();
        void hardStop();
        void execute(string& command, motorH& motor);
        void debug();
        bool moving = false;
        std::chrono::steady_clock::time_point beginL = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point endL = std::chrono::steady_clock::now(); 
        int getDurationL() { return std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();}
        bool did = false;

};
#endif