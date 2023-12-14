//Lampada08.cpp
//Cria classe Mouse.
#include <opencv2/opencv.hpp>
#include "cekeikon.h"
using namespace cv;


int capDev = 0;
VideoCapture cap(capDev); // open the default camera




struct Mouse {
 bool clicou=true;
 int x,y;
};
void onMouse(int event, int x, int y, int flags, void* userdata)
{ Mouse* mouse=(Mouse*)userdata;
 if (event==EVENT_LBUTTONDOWN || event==EVENT_LBUTTONUP) {
 (*mouse).clicou=true; (*mouse).x=x; (*mouse).y=y;
 } 
}

void desenhaFlechas(Mat_<Vec3b>& a){
    flecha(a, 140, 140, 90, 90, COR(255,0,0),5);
    //Botao b1(Point(70, 60),Point(160,160),"");

    flecha(a, 230, 140, 230, 90, COR(255,0,0),5);
    //Botao b3(Point(70,180),Point(160,280),"");

    flecha(a, 320, 140, 380, 90, COR(255,0,0),5);
    //Botao b5(Point(70,300),Point(160,410),"");
    
    flecha(a, 135, 315, 75, 315, COR(255,0,0),5);
    //Botao b2(Point(270, 60),Point(360,160),""); 

    //flecha(a, 140, 140, 90, 90, COR(255,0,0),5);
    //Botao b4(Point(270,180),Point(360,280),""); 

    flecha(a, 315, 315, 390, 315, COR(255,0,0),5);
    //Botao b6(Point(270,300),Point(360,410),"");

    flecha(a, 140, 490, 90, 540, COR(255,0,0),5);
    //Botao b7(Point(470, 60),Point(560,160),"");

    flecha(a, 230, 490, 230, 540, COR(255,0,0),5);
    //Botao b8(Point(470,180),Point(560,280),"");

    flecha(a, 320, 490, 380, 540, COR(255,0,0),5);
    //Botao b9(Point(470,300),Point(560,410),"");
     
}
class Botao {
 public:
 Point p,q;
 string st0,st1;
 bool ligado,primeiraVez;
 Botao(Point _p, Point _q, string _st0, string _st1="", bool _ligado=false);
 void testaDesenha(Mat_<Vec3b>& a, Mouse mouse);
};
Botao::Botao(Point _p, Point _q, string _st0, string _st1, bool _ligado)
{ p=_p; q=_q; st0=_st0; 
 if (_st1!="") st1=_st1; else st1=_st0;
 ligado=!_ligado; primeiraVez=true; 
}
void Botao::testaDesenha(Mat_<Vec3b>& a, Mouse mouse)
{ if (primeiraVez || (p.x<=mouse.x && mouse.x<=q.x && p.y<=mouse.y && mouse.y<=q.y)) {
 if (ligado) {
    ligado=false;
 rectangle(a, p,q, Scalar(192,192,192), CV_FILLED);
 line(a, p, Point(p.x,q.y), Scalar(255,255,255), 0);
 line(a, p, Point(q.x,p.y), Scalar(255,255,255), 0);
 line(a, q, Point(p.x,q.y), Scalar(0,0,0), 0);
 line(a, q, Point(q.x,p.y), Scalar(0,0,0), 0); 
 putText(a, st0, Point(p.x+5,(p.y+q.y)/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
 } else { 
    ligado = true;
 rectangle(a, p,q, Scalar(128,255,255), CV_FILLED);
 line(a, p, Point(p.x,q.y), Scalar(0,0,0), 0);
 line(a, p, Point(q.x,p.y), Scalar(0,0,0), 0);
 line(a, q, Point(p.x,q.y), Scalar(255,255,255), 0);
 line(a, q, Point(q.x,p.y), Scalar(255,255,255), 0);
 putText(a, st1, Point(p.x+5,(p.y+q.y)/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
 }
 primeiraVez=false;
 }
}
int main()
{ printf("Sai do programa apertando uma tecla.\n");
 printf("Liga e desliga os botoes com click.\n");
 Mat_<Vec3b> a(480,640,Vec3b(192,192,192));
 namedWindow("janela");
 Mouse mouse;
 setMouseCallback("janela", onMouse, &mouse);
 imshow("janela",a);
 //⇐⇑⇒⇓⇖⇗⇘⇙
 Botao b1(Point(70, 60),Point(160,160),"");
 Botao b3(Point(70,180),Point(160,280),"");
 Botao b5(Point(70,300),Point(160,410),"");

 Botao b2(Point(270, 60),Point(360,160),""); 
 Botao b4(Point(270,180),Point(360,280),"",""); 
 Botao b6(Point(270,300),Point(360,410),"");


 Botao b7(Point(470, 60),Point(560,160),"");
 Botao b8(Point(470,180),Point(560,280),"");
 Botao b9(Point(470,300),Point(560,410),"");

 Mat img, concatImg;
img = Mat::zeros(480 , 640, CV_8UC3);
concatImg = Mat::zeros(480+480 , 640, CV_8UC3);
    //make it continuous
if (!img.isContinuous()) {
    img = img.clone();
}
 
 
    
 while (waitKey(30)<0) {
    cap >> img;
    
    concatImg = grudaH(a,img);        
    imshow("janela",concatImg);
    if (mouse.clicou) {
        b1.testaDesenha(a,mouse);
        b2.testaDesenha(a,mouse);
        b3.testaDesenha(a,mouse);
        b4.testaDesenha(a,mouse);
        b5.testaDesenha(a,mouse);
        b6.testaDesenha(a,mouse);
        b7.testaDesenha(a,mouse);
        b8.testaDesenha(a,mouse);
        b9.testaDesenha(a,mouse);
        desenhaFlechas(a);
        mouse.clicou=false;
    }
 }
}