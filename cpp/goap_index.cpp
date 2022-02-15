//#include <iostream>
//#include <string>

using namespace std;

class Estado {
    //public bool temBomba;

    public:
        bool temBomba;
        bool estaVizinhoInimigo;
        bool estaPertoBombaInimiga;

        Estado(bool temBomba, bool estaVizinhoInimigo, bool estaPertoBombaInimiga);
};

Estado::Estado(bool temBomba, bool estaVizinhoInimigo, bool estaPertoBombaInimiga) {
    this->temBomba = temBomba;
    this->estaVizinhoInimigo = estaVizinhoInimigo;
    this->estaPertoBombaInimiga = estaPertoBombaInimiga;
}