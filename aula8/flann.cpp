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

  Mat grayImg = imread("3.png", IMREAD_GRAYSCALE);

  Mat cImg = grayImg.reshape(1,1);
  Mat tmp;
  cImg.convertTo(tmp,CV_32FC3);
  ind.knnSearch(tmp,indices,dists,1,flann::SearchParams(128));

  TimePoint t3=timePoint();
  //printf("Erros=%10.2f%%\n",100.0*mnist.contaErros()/mnist.nq);
  std::cout << mnist.ay(indices[0]) << std::endl;
  //printf("Tempo de treinamento: %d\n",indices[0]);
  //printf("Tempo de predicao: %f\n",timeSpan(t2,t3));

}