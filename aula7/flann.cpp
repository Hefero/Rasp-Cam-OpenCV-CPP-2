//Flann.cpp com 4 arvores - pos2020
//Linkar com OpenCV2 ou OpenCV3
#include <cekeikon.h>

void ApplyFilter(cv::Mat &inFrame, cv::Mat &outFrame, double alpha)
{
    cv::Mat black = cv::Mat(inFrame.rows, inFrame.cols, inFrame.type(), 0.0);
    double beta = (1.0 - alpha);
    cv::addWeighted(inFrame, alpha, black, beta, 0.0, outFrame);
}


int main() {
  namedWindow("janela");
  MNIST mnist(28, true, true);
  mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
  TimePoint t1=timePoint();
  flann::Index ind(mnist.ax,flann::KDTreeIndexParams(8));
  TimePoint t2=timePoint();
  vector<int> indices(1); vector<float> dists(1);
  for (int l=0; l<mnist.qx.rows; l++) {
    ind.knnSearch(mnist.qx.row(l),indices,dists,1,flann::SearchParams(128));
    mnist.qp(l)=mnist.ay(indices[0]);
  }

  Mat grayImg = imread("116.png", IMREAD_GRAYSCALE);

  Mat imgContrast;
  ApplyFilter(grayImg,imgContrast,0.3);
  imgContrast.convertTo(imgContrast, -1, 0, 50);
  imwrite("a.png",imgContrast);
  imshow("janela",imgContrast);
  


  Mat cImg = imgContrast.reshape(1,1);
  Mat tmp;
  cImg.convertTo(tmp,CV_32FC1);
  ind.knnSearch(tmp,indices,dists,1,flann::SearchParams(128));

  TimePoint t3=timePoint();
  //printf("Erros=%10.2f%%\n",100.0*mnist.contaErros()/mnist.nq);
  std::cout << mnist.ay(indices[0]) << std::endl;
  //printf("Tempo de treinamento: %d\n",indices[0]);
  //printf("Tempo de predicao: %f\n",timeSpan(t2,t3));
  cvWaitKey(0);
}