//Flann.cpp com 4 arvores - pos2020
//Linkar com OpenCV2 ou OpenCV3
#include <cekeikon.h>
int main(int argc, char *argv[]) {
  MNIST mnist(14, true, true);
  mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
  TimePoint t1=timePoint();
  flann::Index ind(mnist.ax,flann::KDTreeIndexParams(64));
  TimePoint t2=timePoint();
  vector<int> indices(1); vector<float> dists(1);
  //for (int l=0; l<mnist.qx.rows; l++) {
  //  ind.knnSearch(mnist.qx.row(l),indices,dists,1,flann::SearchParams(64));
  //  mnist.qp(l)=mnist.ay(indices[0]);
  //}

  Mat grayImg = imread(argv[1], IMREAD_GRAYSCALE);

  // let's downscale the image using new  width and height
  int down_width = 14;
  int down_height = 14;
  Mat resized_down;
  //resize down
  resize(grayImg, resized_down, Size(down_width, down_height), INTER_LINEAR);

  Mat cImg = resized_down.reshape(1,1);
  Mat tmp;    
  cImg.convertTo(tmp,CV_32FC1);
  ind.knnSearch(tmp,indices,dists,1,flann::SearchParams(5120));

  TimePoint t3=timePoint();
  //printf("Erros=%10.2f%%\n",100.0*mnist.contaErros()/mnist.nq);
  std::cout << mnist.ay(indices[0]) << std::endl;
  //printf("Tempo de treinamento: %d\n",indices[0]);
  //printf("Tempo de predicao: %f\n",timeSpan(t2,t3));

}