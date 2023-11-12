#ifndef MYGUI_H
#define MYGUI_H

#include <opencv2/opencv.hpp>
#include "cekeikon.h"
#include "botao/botao.cpp"

using namespace cv;

class Mygui
{
public:

    Botao b1 = Botao(Point(70, 60),Point(160,160),"","",false);
    Botao b3 = Botao(Point(70,180),Point(160,280),"","",false);
    Botao b5 = Botao(Point(70,300),Point(160,410),"","",false);

    Botao b2 = Botao(Point(270, 60),Point(360,160),"","",false); 
    Botao b4 = Botao(Point(270,180),Point(360,280),"","",false); 
    Botao b6 = Botao(Point(270,300),Point(360,410),"","",false);
                
    Botao b7 = Botao(Point(470, 60),Point(560,160),"","",false);
    Botao b8 = Botao(Point(470,180),Point(560,280),"","",false);
    Botao b9 = Botao(Point(470,300),Point(560,410),"","",false); 
    int capDev = 0;    
    Mouse mouse;
    Mat_<Vec3b> a = Mat_<Vec3b>(480,640,Vec3b(192,192,192));

    Mygui();    
    void onMouse(int event, int x, int y, int flags, void* userdata);
    void desenhaFlechas();
    void guiLoop();
};

#endif