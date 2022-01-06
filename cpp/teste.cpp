#include <iostream>
#include <fstream>

using namespace std;

int main() {

    ofstream arquivoTeste("testeOutput.txt");

    arquivoTeste << "1" << endl;
    cout << "Tentei criar" << endl;

    arquivoTeste.close();
    
    return 0;
}