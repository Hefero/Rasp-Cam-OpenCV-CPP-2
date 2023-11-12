#include "botao.h"

Botao::Botao(Point _p, Point _q, string _st0, string _st1, bool _ligado)
{ 
    p=_p; q=_q; st0=_st0; 
    if (_st1!="") st1=_st1; else st1=_st0;
    ligado=!_ligado; primeiraVez=true; 
}
void Botao::testaDesenha(Mat_<Vec3b>& a, Mouse mouse)
{ 
    if (primeiraVez || (p.x<=mouse.x && mouse.x<=q.x && p.y<=mouse.y && mouse.y<=q.y)) {

        
        if (ligado) {            
            ligado=false;
            rectangle(a, p,q, Scalar(192,192,192), CV_FILLED);
            line(a, p, Point(p.x,q.y), Scalar(255,255,255), 0);
            line(a, p, Point(q.x,p.y), Scalar(255,255,255), 0);
            line(a, q, Point(p.x,q.y), Scalar(0,0,0), 0);
            line(a, q, Point(q.x,p.y), Scalar(0,0,0), 0); 
            putText(a, st0, Point(p.x+5,(p.y+q.y)/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
        } else {
            ligado=true;
            rectangle(a, p,q, Scalar(128,255,255), CV_FILLED);
            line(a, p, Point(p.x,q.y), Scalar(0,0,0), 0);
            line(a, p, Point(q.x,p.y), Scalar(0,0,0), 0);
            line(a, q, Point(p.x,q.y), Scalar(255,255,255), 0);
            line(a, q, Point(q.x,p.y), Scalar(255,255,255), 0);
            putText(a, st1, Point(p.x+5,(p.y+q.y)/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
        }
        primeiraVez=false;
    }
    if (mouse.up) {
        ligado=false;
        rectangle(a, p,q, Scalar(192,192,192), CV_FILLED);
        line(a, p, Point(p.x,q.y), Scalar(255,255,255), 0);
        line(a, p, Point(q.x,p.y), Scalar(255,255,255), 0);
        line(a, q, Point(p.x,q.y), Scalar(0,0,0), 0);
        line(a, q, Point(q.x,p.y), Scalar(0,0,0), 0); 
        putText(a, st0, Point(p.x+5,(p.y+q.y)/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
    }
}