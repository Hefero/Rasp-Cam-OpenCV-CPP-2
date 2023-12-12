
#include "cekeikon.h"

int main(){

    VideoCapture vid_capture("capturado.avi");

    int ex = static_cast<int>(vid_capture.get(CV_CAP_PROP_FOURCC));
    VideoWriter outputVideo;
    Size S = Size((int) vid_capture.get(CV_CAP_PROP_FRAME_WIDTH),    //Acquire input size
                (int) vid_capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    outputVideo.open("video.avi" , ex, vid_capture.get(CV_CAP_PROP_FPS),S, true);


    namedWindow("janela");    
    while (vid_capture.isOpened()) {
        Mat readImg;
        bool isSuccess = vid_capture.read(readImg);
        if (!isSuccess) break;
        imshow("janela",readImg);
        outputVideo << readImg;
       if (int key = cv::waitKey(1) >= 0) break;
    }
}