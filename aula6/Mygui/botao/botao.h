#ifndef BOTAO_H
#define BOTAO_H

#include <opencv2/opencv.hpp>
#include "cekeikon.h"
using namespace cv;


struct Mouse {
    bool clicou=true;
    int x,y;
    bool up=true;
};

class Botao
{
 public:
    Point p,q;
    string st0,st1 = "";
    bool ligado,primeiraVez = true;
    Botao(Point _p, Point _q, string _st0, string _st1="", bool _ligado = true);
    void testaDesenha(Mat_<Vec3b>& a, Mouse mouse);
    bool getState() const { return ligado; }
};



#endif