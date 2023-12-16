
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
#include <bits/stdc++.h> 

Mygui gui;
bool mode = false;
CascadeClassifier cascade;

bool changedMode = false;
auto debouncerS = std::chrono::high_resolution_clock::now();
auto debouncerE = std::chrono::high_resolution_clock::now();
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

void detectAndDisplay( Mat& frame, std::vector<Rect>& faces, int detected );
void sendCommand(Receiver& rec, Mygui& gui);
void sendFollow(Receiver& rec, Mat& frame, std::vector<Rect>& faces);
void onMouseGui(int event, int x, int y, int flags, void* userdata)
{ 
    Mouse* mouse=(Mouse*)userdata;
    if (event==EVENT_LBUTTONDOWN) {
        (*mouse).clicou=true; (*mouse).x=x; (*mouse).y=y;  (*mouse).up=false;
    }
    if (event==EVENT_LBUTTONUP) {
        //if (!(270<=x && x<=360 && 180<=y && y<=280)){ // botao do meio toggle
            (*mouse).clicou=true; (*mouse).x=x; (*mouse).y=y; (*mouse).up=true;
        //}        
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
int mostFrequent(int* arr, int n) 
{ 
    // code here     
    int maxcount = 0; 
    int element_having_max_freq; 
    for (int i = 0; i < n; i++) { 
        int count = 0; 
        for (int j = 0; j < n; j++) { 
            if (arr[i] == arr[j]) 
                count++; 
        } 
  
        if (count > maxcount) { 
            maxcount = count; 
            element_having_max_freq = arr[i]; 
        } 
    }
    return element_having_max_freq; 
} 


int outputNumber[5];
int output_index = 0;
int mostFreq = -1;
vector<unsigned char> compressed;
Mat img = Mat::zeros(480 , 640, CV_8UC3);
int main(int argc, char** argv)
{
MNIST mnist(28, true, true);
mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
TimePoint t1=timePoint();
flann::Index ind(mnist.ax,flann::KDTreeIndexParams(4));
TimePoint t2=timePoint();
vector<int> indices(1); vector<float> dists(1);

int counter = 0;


VideoCapture vid_capture("capturado.avi");
int ex = static_cast<int>(vid_capture.get(CV_CAP_PROP_FOURCC));

VideoWriter outputVideo;
Size S = Size(640+240,    //Acquire input size
              480);
outputVideo.open("receive.avi" , ex, vid_capture.get(CV_CAP_PROP_FPS),S, true);

Receiver rec(argc, argv);


std::vector<Rect> faces;


//mats
Mat concatImg = Mat::zeros(480, 640+240, CV_8UC3);
Mat concatImgF = Mat::zeros(480, 640+640+240, CV_8UC3);

Mat concatImg1 = Mat::zeros(480, 240, CV_8UC3);
Mat concatImgZ = Mat::zeros(480, 240, CV_8UC3);
Mat ImgSize = Mat::zeros(240, 240, CV_8UC3);
Mat resize_upM = Mat::zeros(240, 240, CV_8UC3);
Mat resize_upE = Mat::zeros(240, 240, CV_8UC3);
Mat resize_up = Mat::zeros(240, 240, CV_8UC3);


Mat resized_down = Mat::zeros(28, 28, CV_8UC3);
cv::Mat mask;
Mat eroded;

cascade.load("haar.xml");

namedWindow("janela");
setMouseCallback("janela", onMouseGui, &gui.mouse);
int key = 0;

//namedWindow("mask");
//namedWindow("inner");

auto start = std::chrono::high_resolution_clock::now();
auto end = std::chrono::high_resolution_clock::now();


rec.sendString("Keep Alive");
gui.guiLoop();

Mat guiAuto, guiManual;
gui.a.copyTo(guiManual);
gui.a.copyTo(guiAuto);



cv::putText(guiManual, //target image
    "Mode: Manual", //text
    cv::Point(10, 25), //top-left position
    cv::FONT_HERSHEY_DUPLEX,
    1.0,
    CV_RGB(118, 185, 0), //font color
    2);
cv::putText(guiAuto, //target image
    "Mode: Automatic", //text
    cv::Point(10, 25), //top-left position
    cv::FONT_HERSHEY_DUPLEX,
    1.0,
    CV_RGB(118, 185, 0), //font color
    2);

 while (key != 'q') {
    try{
        if(gui.b4.getState()){
            if (changedMode == false){
                changedMode = true;
                std::cout << "Changing mode" << std::endl;
                mode = !mode;
            }
        }
        else{
            changedMode = false;
            if (!mode){
                gui.a = guiManual;
            }
            else{
                gui.a = guiAuto;
            }
        }
        gui.guiLoop();
        
        if (rec.recvBytes(compressed) > 0){
            img = imdecode(compressed,1);
            Mat imgCopy;
            img.copyTo(imgCopy);
            if (mode == true) {
                detectAndDisplay(img, faces, mostFreq);
                //rec.sendString("Keep Alive");
                

                //Mat grayImg = imread("1199.png", IMREAD_GRAYSCALE);
                if (faces.size() > 0){
                    try {
                        //int areaImg = 10000/faces[0].width*faces[0].height;
                        int padding = 0;
                        //int padding = areaImg;
                        
                        //Mat cropped_image = imgCopy(Range(faces[0].y-padding,faces[0].y+faces[0].height+padding),Range(faces[0].x-padding,faces[0].x+faces[0].width+padding));
                        int startX1=faces[0].x,startY1=faces[0].y,width1=faces[0].width,height1=faces[0].height;
                        //std::cout << "startx: " << minX << " starty: " << minY << " width: " << maxX-minX << " height: " << maxY-minY << std::endl;
                        Mat cropped_image(imgCopy, Rect(startX1,startY1,width1,height1));
                        

                        Mat gray;
                        cvtColor(cropped_image, gray, COLOR_BGR2GRAY);

                        //erode                    
                        int erosion_size = 1.0;
                        Mat element = getStructuringElement( 0,
                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                        Point( erosion_size, erosion_size ) );
                        erode( gray, eroded, element );


                        //gammacorrection                    
                        Mat imageContrastHigh;                    
                        for (int i = 0; i <10; i++){
                            //contrast
                            eroded.convertTo(imageContrastHigh, -1, 1.5, 0.1);
                            //gammacorrection
                            gammaCorrection(imageContrastHigh, eroded, 0.6);                    
                        }

                        //findcountours
                        //Mat resize_upE;
                        //resize(eroded, resize_upE, ImgSize.size(), INTER_LINEAR);
                        //imshow("cropped",resize_upE);
                        
                        cv::threshold(eroded, mask, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
                        Mat resize_upM;
                        try {
                            resize(eroded, resize_upM, ImgSize.size(), INTER_LINEAR);                    
                        }
                        catch (exception ex){

                        }
                        //imshow("mask",resize_upM);

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

                        if(biggestContourIdx1 > 0 && cv::contourArea(contours[biggestContourIdx1]) > 100){
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
                            //std::cout << "startx: " << minX << " starty: " << minY << " width: " << maxX-minX << " height: " << maxY-minY << std::endl;
                            Mat ROI(eroded, Rect(startX,startY,width,height));        
                            Mat croppedImage;          // Copy the data into new matrix
                            ROI.copyTo(croppedImage);
                            //croppedImage.copyTo(img);
                            int down_width = 28;
                            int down_height = 28;                        
                            //resize down
                            try{
                                resize(croppedImage, resized_down, Size(down_width, down_height), INTER_LINEAR);                                
                                resize(croppedImage, resize_up, ImgSize.size(), INTER_LINEAR);
                            }
                            catch(exception ex){

                            }


                            
                            //imwrite("tess.png",resized_down);
                            vconcat(resize_upM,resize_up,concatImg1);
                            //imshow("inner",concatImg1);

                            Mat colorDig = Mat::zeros(480, 240, CV_8UC3);                        
                            cvtColor(concatImg1, colorDig, COLOR_GRAY2BGR);

                            hconcat(img,colorDig,concatImg);
                            hconcat(gui.a,concatImg,concatImgF);
                            imshow("janela",concatImgF);
                            Mat cImg = resized_down.reshape(1,1);
                            Mat tmp;    
                            cImg.convertTo(tmp,CV_32FC3);
                            ind.knnSearch(tmp,indices,dists,1,flann::SearchParams(32));                        
                            

                            outputNumber[output_index] = int(mnist.ay(indices[0]));
                            output_index++;
                            if(output_index == 5){
                                output_index = 0;
                                int n = sizeof(outputNumber) / sizeof(outputNumber[0]);
                                mostFreq = mostFrequent(outputNumber, n);
                                std::cout << mostFreq << std::endl;
                            }
                        }                  
                    }
                    catch(exception ex){

                    }
                    sendFollow(rec, img, faces);                               
                }
                else {
                    output_index = 0;
                    mostFreq = -1;
                    hconcat(img,concatImgZ,concatImg);
                    hconcat(gui.a,concatImg,concatImgF);
                    imshow("janela",concatImgF);
                    rec.sendString("stop");                    
                }                
            }

            else{
                output_index = 0;
                mostFreq = -1;
                hconcat(img,concatImgZ,concatImg);
                hconcat(gui.a,concatImg,concatImgF);
                imshow("janela",concatImgF);
                sendCommand(rec, gui);
            }
                
            
            Mat H;
            //hconcat(img,V,H);            
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            start = std::chrono::high_resolution_clock::now();
            double fps = 1.0/duration.count();
            //std::cout << "Estimated FPS: " << fps << std::endl;
            outputVideo.write(concatImg);
        }
    }
    catch(cv::Exception ex){
            continue;
            rec.sendString("alive");
            rec.recvBytes(compressed);
    }
    if (key = cv::waitKey(1) >= 0) break;
 }
}

void detectAndDisplay( Mat& frame, std::vector<Rect>& faces, int detected )
{
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces    
    Size minSize=Size(14,14);
    Size maxSize=Size(480,480);
    cascade.detectMultiScale( frame_gray, faces, 1.09, 3, 0, minSize, maxSize);
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        Mat faceROI = frame_gray( faces[i] );
        //-- In each face, detect eyes
        if (detected >= 0){
        cv::putText(frame, //target image
            to_string(detected), //text
            cv::Point(faces[i].x, faces[i].y), //top-left position
            cv::FONT_HERSHEY_DUPLEX,
            1.0,
            CV_RGB(255, 0, 0), //font color
            2);
        }

    }
    //-- Show what you got
    //imshow( "Capture - Face detection", frame );
}

void sendFollow(Receiver& rec, Mat& frame, std::vector<Rect>& faces)
{
    if(faces.size() > 0){
        if (faces[0].width < 100 && faces[0].width > 20){
            int Xrect = faces[0].x+faces[0].width/2;
            int Xcenter = 320;
            int epsilon = 70;
            //std::cout << "Tamanho "<< faces[0].width << std::endl;
            if (abs(Xcenter - Xrect) <= epsilon){ // está ao centro
                //std::cout << "Centro "<< Xrect << std::endl;
                rec.sendString("b2");
            }
            else{
                if (Xrect >= (Xcenter + epsilon)){ // está a direita
                //std::cout << "Direita " << Xrect << std::endl;
                
                rec.sendString("b8");
                }
                if (Xrect <= (Xcenter - epsilon)){ // está a esquerda
                //std::cout << "Esquerda " << Xrect << std::endl;
                rec.sendString("b3");
                }
            }
        }
        if (faces[0].width <= 20){
            rec.sendString("stop");
        }
        if (faces[0].width >= 100){ //placa suficientemente grande para reconhecimento correto
            std::chrono::steady_clock::time_point beginL = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point endL = std::chrono::steady_clock::now();
            int duration = std::chrono::duration_cast<std::chrono::microseconds>(endL - beginL).count();
            std::cout << "digito: " << mostFreq << std::endl;
            rec.sendString("stop");
            
            switch (mostFreq)
            {
            case 0: // Pare o carrinho.                
                std::cout << "digito 0" << std::endl;
                rec.sendString("stop");
                break;
            case 1: // Pare o carrinho.
                std::cout << "digito 1" << std::endl;
                rec.sendString("stop");
                break;
            case 2: //Vire 180 graus à esquerda imediatamente.
                std::cout << "digito 2" << std::endl;
                rec.sendString("b3");
                while (duration < 1000 ){ // 2 segundos
                    rec.recvBytes(compressed);
                    rec.sendString("b3");
                    endL = std::chrono::steady_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                }                
                break;
            case 3: //Vire 180 graus à direita imediatamente.
                std::cout << "digito 3" << std::endl;
                rec.sendString("b8");
                while (duration < 1000 ){ // 2 segundos
                    rec.recvBytes(compressed);
                    rec.sendString("b8");
                    endL = std::chrono::steady_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                }                
                break;
            case 4: //Passe por baixo da placa e continue em frente.
                std::cout << "digito 4" << std::endl;
                //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - beginL).count()  << "[µs]" << std::endl;
                rec.sendString("b2");
                while (duration < 1500 ){ // 2 segundos
                    rec.recvBytes(compressed);
                    rec.sendString("b2");
                    endL = std::chrono::steady_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                }                
                break;
            case 5: //Passe por baixo da placa e continue em frente.
                std::cout << "digito 5" << std::endl;
                //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - beginL).count()  << "[µs]" << std::endl;
                rec.sendString("b2");
                while (duration < 1500 ){ // 2 segundos
                    rec.recvBytes(compressed);
                    rec.sendString("b2");
                    endL = std::chrono::steady_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                }                
                break;
            case 6: //Vire 90 graus à esquerda imediatamente.
                std::cout << "digito 6" << std::endl;
                //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - beginL).count()  << "[µs]" << std::endl;
                rec.sendString("b3");
                while (duration < 1000 ){ // 2 segundos
                    rec.recvBytes(compressed);
                    rec.sendString("b3");
                    endL = std::chrono::steady_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                }                
                break;
            case 7: //Vire 90 graus à esquerda imediatamente.
                std::cout << "digito 7" << std::endl;
                //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - beginL).count()  << "[µs]" << std::endl;
                rec.sendString("b3");
                while (duration < 1000 ){ // 2 segundos
                    rec.recvBytes(compressed);
                    rec.sendString("b3");
                    endL = std::chrono::steady_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                }                
                break;
            case 8: //Vire 90 graus à direita imediatamente.
                std::cout << "digito 8" << std::endl;
                //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - beginL).count()  << "[µs]" << std::endl;
                rec.sendString("b8");
                while (duration < 10000 ){ // 2 segundos
                    //rec.recvBytes(compressed);
                    //rec.sendString("stop");
                    rec.recvBytes(compressed);
                    rec.sendString("b8");
                    endL = std::chrono::steady_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                }                
                break;
            case 9: //Vire 90 graus à direita imediatamente.
               std::cout << "digito 9" << std::endl;
                //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - beginL).count()  << "[µs]" << std::endl;
                rec.sendString("b8");
                while (duration < 1000 ){ // 2 segundos
                    rec.recvBytes(compressed);
                    rec.sendString("b8");
                    endL = std::chrono::steady_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endL - beginL).count();
                }                
                break;          
            default: //digito nao reconhecido
                std::cout << "digito nao reconhecido" << std::endl;
                rec.sendString("stop");
            }
        }
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
            rec.sendString("keep"); //botao do meio 
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