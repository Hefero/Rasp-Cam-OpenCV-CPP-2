//Flann.cpp com 4 arvores - pos2020
//Linkar com OpenCV2 ou OpenCV3
#include <cekeikon.h>
int main() {
  MNIST mnist(28, true, true);
  mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
  TimePoint t1=timePoint();
  flann::Index ind(mnist.ax,flann::KDTreeIndexParams(4));
  TimePoint t2=timePoint();
  vector<int> indices(1); vector<float> dists(1);
  //for (int l=0; l<mnist.qx.rows; l++) {
  //  ind.knnSearch(mnist.qx.row(l),indices,dists,1,flann::SearchParams(64));
  //  mnist.qp(l)=mnist.ay(indices[0]);
  //}
  //Mat grayImg = imread("8.png", CV_32F);//, IMREAD_COLOR);

  //Mat data;
  //grayImg.convertTo(data, CV_32F);
  //data = data.reshape(1, 1).t();

  Mat grayImg = imread("0.png");//, IMREAD_GRAYSCALE);//, IMREAD_COLOR);
  Mat_<FLT> img = Mat::zeros(28 , 28, IMREAD_GRAYSCALE);

   //cv::Mat matROIResized;
    //cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));     // resize image, this will be more consistent for recognition and storage

    cv::Mat matROIFloat;
    grayImg.convertTo(matROIFloat, CV_32FC1);             // convert Mat to float, necessary for call to find_nearest

    cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

    cv::Mat matCurrentChar(0, 0, CV_32F);

    //kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);     // finally we can call find_nearest !!!

  //Mat_<GRY> MNIST::bbox(Mat_<GRY> a);

  Mat_<GRY> convert;
  //convert=mnist.bbox(grayImg);

  Mat_<GRY> matResults;
  ind.knnSearch(matROIFlattenedFloat,indices,dists,1,flann::SearchParams(64));

  TimePoint t3=timePoint();
  //printf("Erros=%10.2f%%\n",100.0*mnist.contaErros()/mnist.nq);
  std::cout << mnist.ay(indices[0]) << std::endl;
  //printf("Tempo de treinamento: %d\n",indices[0]);
  //printf("Tempo de predicao: %f\n",timeSpan(t2,t3));
}