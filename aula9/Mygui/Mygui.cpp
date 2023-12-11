#include "Mygui.h"

// Highgui constructor
Mygui::Mygui()
{
    
}

void Mygui::desenhaFlechas(){
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

void Mygui::guiLoop(){
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
            desenhaFlechas();
            mouse.clicou=false;
        }
}