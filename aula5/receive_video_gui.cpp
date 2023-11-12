
#include "Mygui/Mygui.cpp"
#include <opencv2/opencv.hpp>
#include "cekeikon.h"
#include <chrono>
#include "Receiver/Receiver.cpp"

Mygui gui;

void sendCommand(Receiver& rec, Mygui& gui);
void onMouseGui(int event, int x, int y, int flags, void* userdata)
{ 
    Mouse* mouse=(Mouse*)userdata;
    if (event==EVENT_LBUTTONDOWN) {
        (*mouse).clicou=true; (*mouse).x=x; (*mouse).y=y;  (*mouse).up=false;
    }
    if (event==EVENT_LBUTTONUP) {
        (*mouse).clicou=true; (*mouse).x=x; (*mouse).y=y; (*mouse).up=true;
    }
    
}

int main(int argc, char** argv)
{
Receiver rec(argc, argv);

Mat img = Mat::zeros(480 , 640, CV_8UC3);
vector<unsigned char> compressed;
Mat concatImg = Mat::zeros(480+480 , 640, CV_8UC3);

namedWindow("janela");
setMouseCallback("janela", onMouseGui, &gui.mouse);
int key = 0;

auto start = std::chrono::high_resolution_clock::now();
auto end = std::chrono::high_resolution_clock::now();

    
 while (key != 'q') {
    try{
        gui.guiLoop();
        sendCommand(rec, gui);
        if (rec.recvBytes(compressed) > 0){
            img = imdecode(compressed,1);
            start = std::chrono::high_resolution_clock::now();
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        if(duration.count() > 3){
            std::cout << "Lost Connection!" << std::endl;
            
            int tries = 3;
            auto startRetry = std::chrono::high_resolution_clock::now();
            auto endRetry = std::chrono::high_resolution_clock::now();
            rec.closeSocket();
            rec.~Receiver();
            while(tries > 0){                
                endRetry = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> durationRetry = endRetry - startRetry;
                if(durationRetry.count() > 10) {
                    std::cout << "Retrying Connection" << std::endl;
                    rec = Receiver(argc, argv);
                    startRetry = std::chrono::high_resolution_clock::now();
                    tries--;
                }
            }
            
        }        
        concatImg = grudaH(gui.a,img);        
        imshow("janela",concatImg);        
        
    }
    catch(cv::Exception ex){
            continue;
    }
    if (key = cv::waitKey(1) >= 0) break;
 }
}

void sendCommand(Receiver& rec, Mygui& gui)
{
    if (gui.mouse.up){
        rec.sendString("stop");
    }
    else {        
        if(gui.b1.getState()){
            rec.sendString("b1");
        }
        
        if(gui.b2.getState()){
            rec.sendString("b2");
        }
        
        if(gui.b3.getState()){
            rec.sendString("b3");        
        }
        if(gui.b4.getState()){
            rec.sendString("b4");        
        }
        if(gui.b5.getState()){
            rec.sendString("b5");        
        }
        if(gui.b6.getState()){
            rec.sendString("b6");        
        }
        if(gui.b7.getState()){
            rec.sendString("b7");        
        }
        if(gui.b8.getState()){
            rec.sendString("b8");        
        }
        if(gui.b9.getState()){
            rec.sendString("b9");        
        }
    }
    
}
