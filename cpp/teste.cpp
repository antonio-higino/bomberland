#include <iostream>
#include <fstream>

/*void printInfile() {
    std::ofstream arquivoTeste;
    arquivoTeste.open("C:\\pastaTeste\\testeOutput.txt", std::ios::app);

    if(arquivoTeste.fail()){
        std::cout << "Falhou" << std::endl;
    }

    arquivoTeste << "1" << std::endl;
    //std::cout << "Tentei criar 2" << std::endl;

    arquivoTeste.close();
}*/

int main() {

    std::string matrizMapa[15][15];

    for (int i = 0; i < 15; i++){
      for (int j = 0; j < 15; j++){
        matrizMapa[i][j] = {"0"};
      }
    }

    for (int i = 0; i < 15; i++){
      for (int j = 0; j < 15; j++){
        std::cout << matrizMapa[i][j];
      }
      std::cout << std::endl;
    }
    
    return 0;
}