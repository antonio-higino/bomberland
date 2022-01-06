#include <iostream>
#include <fstream>

void printInfile() {
    std::ofstream arquivoTeste;
    arquivoTeste.open("C:\\pastaTeste\\testeOutput.txt", std::ios::app);

    if(arquivoTeste.fail()){
        std::cout << "Falhou" << std::endl;
    }

    arquivoTeste << "1" << std::endl;
    //std::cout << "Tentei criar 2" << std::endl;

    arquivoTeste.close();
}

int main() {

    for(int i = 0; i < 5; i++){
        printInfile();
    }
    
    return 0;
}