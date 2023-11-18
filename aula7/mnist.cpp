#include <cekeikon.h>
int main() {
  MNIST mnist(28,true,true); //nao redimensiona imagens
                              //inverte preto/branco=true,
                              //crop bounding box=false
  mnist.le("/home/hae/cekeikon5/tiny_dnn/data");
  //mnist.qp.setTo(-1); //Coloca respostas erradas de proposito para imprimir todas imagens
  for (size_t i = 0; i < mnist.AX.size(); i++)
  {
    if (mnist.AY[i] == 9){
        Mat_<GRY> e=mnist.AX[i]; //Organiza 10000 imagens em 100 linhas e 100 colunas
        string filename = "9/" +  to_string(i) + ".png";
        imp(e,filename); //Imprime 10000 imagens-teste como visual.png
    }
  }
}
