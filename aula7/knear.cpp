//knearest_v3.cpp - pos2020
//Linkar com opencv3 (compila knearest_v3 -c -v3)
#include <cekeikon.h>
int main() {
  MNIST mnist(28, true, true);
  mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
  TimePoint t1=timePoint();
  Ptr<ml::KNearest>  knn(ml::KNearest::create());
  knn->train(mnist.ax, ml::ROW_SAMPLE, mnist.ay);
  TimePoint t2=timePoint();
  Mat_<FLT> dist;

  
  Mat grayImg = imread("8.png", CV_32F);//, IMREAD_COLOR);
  Mat_<FLT> img = Mat::zeros(14 , 14, IMREAD_GRAYSCALE);

  //Mat_<GRY> MNIST::bbox(Mat_<GRY> a);

  Mat_<GRY> convert;
  //convert=mnist.bbox(grayImg);

  Mat_<FLT> matResults;

  Mat1f grayImg2 = imread("0.png", IMREAD_GRAYSCALE);
  //cv::Mat1f matROIFloat;
  //grayImg2.convertTo(matROIFloat, IMREAD_GRAYSCALE);             // convert Mat to float, necessary for call to find_nearest
  //cv::Mat1f matROIResized;
  //cv::resize(matROIFloat, matROIResized, cv::Size(14, 14));     // resize image, this will be more consistent for recognition and storage
  //cv::Mat1f matROIFlattenedFloat = matROIResized.reshape(1, 1);
  //cv::Mat1f matCurrentChar(0, 0, IMREAD_GRAYSCALE);
  //knn->findNearest(grayImg2, 1, noArray(), mnist.qp);     // finally we can call find_nearest !!!

  Mat res;
    // predict on majority of k(5) neighbours:
    knn->findNearest(grayImg2, 1, res, matResults);
    //int e = test_labels.at<int>(i);
    //int p = (int)res.at<float>(0);
    cerr << matResults << endl;

  //knn->findNearest(img, 1, noArray(), matResults, dist);

  //std::cout << matCurrentChar << std::endl;
  //TimePoint t3=timePoint();
  //printf("Erros=%10.2f%%\n",100.0*mnist.contaErros()/mnist.nq);
  //printf("Tempo de treinamento: %f\n",timeSpan(t1,t2));
  //printf("Tempo de predicao: %f\n",timeSpan(t2,t3));
}
