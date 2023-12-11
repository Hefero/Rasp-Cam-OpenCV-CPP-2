
#include "Mygui/Mygui.cpp"
#include <opencv2/opencv.hpp>
#include "cekeikon.h"
#include <chrono>
#include "Receiver/Receiver.cpp"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

Mygui gui;

CascadeClassifier cascade;

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

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
MNIST mnist(14, true, true);
mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
TimePoint t1=timePoint();
flann::Index ind(mnist.ax,flann::KDTreeIndexParams(32));
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
            detectAndDisplay(img, faces);
            //rec.sendString("Keep Alive");
            

            //Mat grayImg = imread("1199.png", IMREAD_GRAYSCALE);
            if (faces.size() > 0){
                try {
                    //int areaImg = 10000/faces[0].width*faces[0].height;
                    int padding = 0;
                    //int padding = areaImg;
                    Mat cropped_image = imgCopy(Range(faces[0].y-padding,faces[0].y+faces[0].height+padding),Range(faces[0].x-padding,faces[0].x+faces[0].width+padding));
                    Mat gray;
                    cvtColor(cropped_image, gray, COLOR_BGR2GRAY);


                    //erode
                    Mat eroded;
                    int erosion_size = 1;
                    Mat element = getStructuringElement( 0,
                    Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                    Point( erosion_size, erosion_size ) );
                    erode( gray, eroded, element );
                    
                    //gammacorrection
                    Mat gammaImg;
                    gammaCorrection(eroded, gammaImg, 0.8);
                    Mat imageContrastHigh2;
                    
                    //contrast
                    gammaImg.convertTo(imageContrastHigh2, -1, 2, 0.1);

                    //erode
                    element = getStructuringElement( 0,
                    Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                    Point( erosion_size, erosion_size ) );
                    erode( imageContrastHigh2, eroded, element );


                    //gammacorrection
                    gammaCorrection(eroded, gammaImg, 0.8);
                    
                    for (int i = 0; i <5; i++){
                        //contrast
                        gammaImg.convertTo(imageContrastHigh2, -1, 1.5, 0.1);

                        //gammacorrection
                        gammaCorrection(imageContrastHigh2, gammaImg, 0.6);
                    
                    }

                    //findcountours

                    cv::Mat mask;
                    cv::threshold(imageContrastHigh2, mask, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

                    std::vector<std::vector<cv::Point>> contours;
                    std::vector<cv::Vec4i> hierarchy;
                    cv::findContours(mask,contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

                    int biggestContourIdx0 = -1;
                    int biggestContourIdx1 = -1;
                    int biggestContourIdx2 = -1;
                    float biggestContourArea = 0;
                    cv::Mat drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                    for( int i = 0; i< contours.size(); i++ )
                    {
                        cv::Scalar color = cv::Scalar(0, 100, 0);
                        drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, cv::Point() );
                        float ctArea= cv::contourArea(contours[i]);                        
                        if(ctArea > biggestContourArea)
                        {
                            biggestContourArea = ctArea;
                            biggestContourIdx2 = biggestContourIdx1;
                            biggestContourIdx1 = biggestContourIdx0;
                            biggestContourIdx0 = i;
                        }
                    }

                    if(biggestContourIdx2 > 0 && cv::contourArea(contours[biggestContourIdx2]) > 4){
                        //std::cout << "number framed" << std::endl;
                        cv::RotatedRect boundingBox = cv::minAreaRect(contours[biggestContourIdx2]);
                        // one thing to remark: this will compute the OUTER boundary box, so maybe you have to erode/dilate if you want something between the ragged lines

                        // draw the rotated rect
                        cv::Point2f corners[4];
                        boundingBox.points(corners);
                        //cv::line(gammaImg, corners[0], corners[1], cv::Scalar(255,255,255));
                        //cv::line(gammaImg, corners[1], corners[2], cv::Scalar(255,255,255));
                        //cv::line(gammaImg, corners[2], corners[3], cv::Scalar(255,255,255));
                        //cv::line(gammaImg, corners[3], corners[0], cv::Scalar(255,255,255));

                        //Mat cropped_image_final = gammaImg(Range(corners[2].x-padding,corners[2].y+padding),Range(corners[0].x-padding,corners[0].y+padding));
                        //Mat cropped_image = imgCopy(Range(faces[0].y-padding,faces[0].y+faces[0].height+padding),Range(faces[0].x-padding,faces[0].x+faces[0].width+padding));
                        //std::cout << "faces: faces[0].y " << faces[0].y << " faces[0].x" << faces[0].x << std::endl;
                        //std::cout << "corners: corners[0].y " << corners[0].y << " corners[0].x" << corners[0].x << std::endl;
                        //std::cout << "corners: corners[1].y " << corners[1].y << " corners[1].x" << corners[1].x << std::endl;
                        //std::cout << "corners: corners[2].y " << corners[2].y << " corners[2].x" << corners[2].x << std::endl;
                        //std::cout << "corners: corners[3].y " << corners[3].y << " corners[3].x" << corners[3].x << std::endl;
                        //std::cout << "number framed" << std::endl;
                        //Mat cropped_image_final = gammaImg(Range(corners[3].y,corners[0].y),Range(corners[3].x,corners[1].x));
                        //cropped_image_final.copyTo(img);

                        int maxX = corners[0].x, maxY = corners[0].y, minX = corners[0].x, minY = corners[0].y;
                        for (int i = 0; i <= 3; i++) {
                            if (corners[i].x >= maxX){
                                maxX = corners[i].x;
                            }
                            if (corners[i].y >= maxY){
                                maxY = corners[i].y;
                            }
                            if (corners[i].x <= minX){
                                minX = corners[i].x;
                            }
                            if (corners[i].y <= minY){
                                minY = corners[i].y;
                            }
                        }

                        int startX=corners[1].x,startY=corners[1].y,width=corners[2].x-corners[1].x,height=corners[3].y-corners[1].y;
                        std::cout << "startx: " << minX << " starty: " << minY << " width: " << maxX-minX << " height: " << maxY-minY << std::endl;
                        Mat ROI(gammaImg, Rect(startX,startY,width,height));


                        

                        Mat croppedImage;          // Copy the data into new matrix


                        ROI.copyTo(croppedImage);                
                        imwrite("tess.png",ROI);
                        croppedImage.copyTo(img);
                        Mat cImg = croppedImage.reshape(1,1);
                        Mat tmp;
                        cImg.convertTo(tmp,CV_32FC1);
                        ind.knnSearch(tmp,indices,dists,1,flann::SearchParams(512));
                        std::cout << mnist.ay(indices[0]) << std::endl;
                    }

                  
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