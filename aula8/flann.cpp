//Flann.cpp com 4 arvores - pos2020
//Linkar com OpenCV2 ou OpenCV3
#include <cekeikon.h>
int main() {
  MNIST mnist(28, true, true);
  mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
  TimePoint t1=timePoint();
  flann::Index ind(mnist.ax,flann::KDTreeIndexParams(8));
  TimePoint t2=timePoint();
  vector<int> indices(1); vector<float> dists(1);
  for (int l=0; l<mnist.qx.rows; l++) {
    ind.knnSearch(mnist.qx.row(l),indices,dists,1,flann::SearchParams(64));
    mnist.qp(l)=mnist.ay(indices[0]);
  }

  Mat grayImg = imread("1199.png", IMREAD_GRAYSCALE);

  Mat new_image = Mat::zeros( grayImg.size(), grayImg.type() );
  double alpha = 1.0; /*< Simple contrast control */
  int beta = 0;       /*< Simple brightness control */
  for( int y = 0; y < grayImg.rows; y++ ) {
      for( int x = 0; x < grayImg.cols; x++ ) {
          for( int c = 0; c < grayImg.channels(); c++ ) {
              new_image.at<Vec3b>(y,x)[c] =
                saturate_cast<uchar>( alpha*grayImg.at<Vec3b>(y,x)[c] + beta );
          }
      }
  }

  Mat cImg = new_image.reshape(1,1);
  Mat tmp;
  cImg.convertTo(tmp,CV_32FC1);
  ind.knnSearch(tmp,indices,dists,1,flann::SearchParams(128));

  TimePoint t3=timePoint();
  //printf("Erros=%10.2f%%\n",100.0*mnist.contaErros()/mnist.nq);
  std::cout << mnist.ay(indices[0]) << std::endl;
  //printf("Tempo de treinamento: %d\n",indices[0]);
  //printf("Tempo de predicao: %f\n",timeSpan(t2,t3));

}