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
#include <wiringPi.h>
#include <softPwm.h>

using std::string;



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
};
#endif