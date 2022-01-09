#include <iostream>
#include <fstream>

void testar() {

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
}