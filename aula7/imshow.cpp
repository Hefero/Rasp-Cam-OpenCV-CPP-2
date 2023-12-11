#include <cekeikon.h>
void ApplyFilter(cv::Mat &inFrame, cv::Mat &outFrame, double alpha)
{
    cv::Mat black = cv::Mat(inFrame.rows, inFrame.cols, inFrame.type(), 0.0);
    double beta = (1.0 - alpha);
    cv::addWeighted(inFrame, alpha, black, beta, 0.0, outFrame);
}

int main() {
    namedWindow("janela");
    Mat image = imread("116.png", IMREAD_COLOR);

    Mat imageContrastHigh4;
    //img.convertTo(imageContrastHigh4, -1, 4, 100); //increase the contrast by 4
    //ApplyFilter(img,imageContrastHigh4,0.5);

    cv::Mat new_image = cv::Mat::zeros(image.size(), image.type());
    double alpha = 0.5; /*< Simple contrast control */
    int beta = 50;       /*< Simple brightness control */

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < image.channels(); c++) {
                new_image.at<cv::Vec3b>(y, x)[c] =
                    cv::saturate_cast<uchar>(alpha*image.at<cv::Vec3b>(y, x)[c] + beta);
            }
        }
    }


    imwrite("a.png",new_image);
    imshow("janela",new_image);
    cvWaitKey(0);
}