
#include "Mygui/Mygui.cpp"
#include <opencv2/opencv.hpp>
#include "cekeikon.h"
#include <chrono>
#include "Receiver/Receiver.cpp"

Mygui gui;

CascadeClassifier cascade;

void detectAndDisplay( Mat& frame, std::vector<Rect>& faces );
void sendCommand(Receiver& rec, Mygui& gui);
void sendFollow(Receiver& rec, Mat& frame, std::vector<Rect>& faces);
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
void gammaCorrection(const Mat &src, Mat &dst, const float gamma)
{
    float invGamma = 1 / gamma;

    Mat table(1, 256, CV_8U);
    uchar *p = table.ptr();
    for (int i = 0; i < 256; ++i) {
        p[i] = (uchar) (pow(i / 255.0, invGamma) * 255);
    }

    LUT(src, table, dst);
}

int main(int argc, char** argv)
{
MNIST mnist(28, true, true);
mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
TimePoint t1=timePoint();
flann::Index ind(mnist.ax,flann::KDTreeIndexParams(4));
TimePoint t2=timePoint();
vector<int> indices(1); vector<float> dists(1);



Receiver rec(argc, argv);

Mat img = Mat::zeros(480 , 640, CV_8UC3);
std::vector<Rect> faces;
vector<unsigned char> compressed;
Mat concatImg = Mat::zeros(480+480 , 640, CV_8UC3);
cascade.load("haar.xml");

namedWindow("janela");
setMouseCallback("janela", onMouseGui, &gui.mouse);
int key = 0;

auto start = std::chrono::high_resolution_clock::now();
auto end = std::chrono::high_resolution_clock::now();

rec.sendString("Keep Alive");
 while (key != 'q') {
    try{
        gui.guiLoop();
        
        if (rec.recvBytes(compressed) > 0){
            img = imdecode(compressed,1);
            Mat imgCopy;
            img.copyTo(imgCopy);
            //detectAndDisplay(img, faces);
            //rec.sendString("Keep Alive");
            

            //Mat grayImg = imread("1199.png", IMREAD_GRAYSCALE);
            if (faces.size() > 0){
                try {
                    int padding = 27;
                    Mat cropped_image = imgCopy(Range(faces[0].y-padding,faces[0].y+faces[0].height+padding),Range(faces[0].x-padding,faces[0].x+faces[0].width+padding));
                    Mat gray;
                    cvtColor(cropped_image, gray, COLOR_BGR2GRAY);

                    int down_width = 28;
                    int down_height = 28;
                    Mat resize_down;
                    
                    // resize down
                    resize(cropped_image, resize_down, Size(down_width, down_height), INTER_LINEAR);

                    Mat grayImg;
                    cv::cvtColor(resize_down, grayImg, cv::COLOR_BGR2GRAY);

                    //Mat new_image = Mat::zeros( grayImg.size(), grayImg.type() );
                    //double alpha = 1.0; /*< Simple contrast control */
                    //int beta = 0;       /*< Simple brightness control */
                    //for( int y = 0; y < grayImg.rows; y++ ) {
                    //    for( int x = 0; x < grayImg.cols; x++ ) {
                    //        for( int c = 0; c < grayImg.channels(); c++ ) {
                    //            new_image.at<Vec3b>(y,x)[c] =
                    //                saturate_cast<uchar>( alpha*grayImg.at<Vec3b>(y,x)[c] + beta );
                    //        }
                    //    }
                    //}
                    Mat eroded;
                    int erosion_size = 1;
                    Mat element = getStructuringElement( 0,
                    Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                    Point( erosion_size, erosion_size ) );
                    erode( gray, eroded, element );

                    Mat gammaImg;
                    gammaCorrection(eroded, gammaImg, 0.75);

                    //gammaImg.copyTo(img);

                    Mat cImg = gammaImg.reshape(1,1);
                    Mat tmp;
                    cImg.convertTo(tmp,CV_32FC1);
                    ind.knnSearch(tmp,indices,dists,1,flann::SearchParams(32));
                    std::cout << mnist.ay(indices[0]) << std::endl;
                }
                catch(exception ex){

                }
            }

            sendFollow(rec, img, faces);
            //concatImg = grudaH(gui.a,img);
            imshow("janela",img);
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            start = std::chrono::high_resolution_clock::now();
            double fps = 1.0/duration.count();
            std::cout << "Estimated FPS: " << fps << std::endl;

        }
    }
    catch(cv::Exception ex){
            continue;
    }
    if (key = cv::waitKey(1) >= 0) break;
 }
}

void detectAndDisplay( Mat& frame, std::vector<Rect>& faces )
{
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces    
    Size minSize=Size(14,14);
    Size maxSize=Size(600,600);
    cascade.detectMultiScale( frame_gray, faces, 1.1, 3, 0, minSize, maxSize);
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        Mat faceROI = frame_gray( faces[i] );
        //-- In each face, detect eyes
        cv::putText(frame, //target image
            to_string(faces[i].width), //text
            cv::Point(faces[i].x, faces[i].y), //top-left position
            cv::FONT_HERSHEY_DUPLEX,
            1.0,
            CV_RGB(118, 185, 0), //font color
            2);

    }
    //-- Show what you got
    //imshow( "Capture - Face detection", frame );
}

void sendFollow(Receiver& rec, Mat& frame, std::vector<Rect>& faces)
{
    if(faces.size() > 0){
        if (faces[0].width < 100 && faces[0].width > 30){
            int Xrect = faces[0].x+faces[0].width/2;
            int Xcenter = 320;
            int epsilon = 70;
            std::cout << "Tamanho "<< faces[0].width << std::endl;
            if (abs(Xcenter - Xrect) <= epsilon){ // está ao centro
                //std::cout << "Centro "<< Xrect << std::endl;
                rec.sendString("b2");
            }
            else{
                if (Xrect >= (Xcenter + epsilon)){ // está a direita
                //std::cout << "Direita " << Xrect << std::endl;
                
                rec.sendString("b7");
                }
                if (Xrect <= (Xcenter - epsilon)){ // está a esquerda
                //std::cout << "Esquerda " << Xrect << std::endl;
                rec.sendString("b1");
                }
            }
        }
        else {
            rec.sendString("stop");
        }
    }
    else {
            rec.sendString("stop");
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