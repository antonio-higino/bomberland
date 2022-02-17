#include <iostream>
#include <list>

using namespace std;

string resultado = "";

struct Estado {
    bool naoTemMunicao = true;
    bool estaVizinhoInimigo = false;
    bool estaPertoBomba = false;
};

class Node {
	public:
		virtual bool run() = 0;
};

class NodeComposto : public Node {
	private:
		list<Node*> filhos;
	public:
		const list<Node*>& getFilhos() const {return filhos;}
		void addFilho (Node* filho) {filhos.emplace_back(filho);}
};

class Seletor : public NodeComposto {
	public:
		virtual bool run() override {
			for (Node* filho : getFilhos()) { 
				if (filho->run())
					return true;
			}
			return false;
		}
};

class EstaEmPerigo : public Node {
	private:
		bool input;
	public:
		EstaEmPerigo (bool input) : input(input) {}
		virtual bool run() override {
			if (input == true) {
				resultado = "perigo";
			}
			return input;
		}
};

class NaoTemMunicao : public Node {
	private:
		bool input;
	public:
		NaoTemMunicao (bool input) : input(input) {}
		virtual bool run() override {
			if (input == true) {
				resultado = "municao";
			}
			return input;
		}
};

class EstaVizinho : public Node {
	private:
		bool input;
	public:
		EstaVizinho (bool input) : input(input) {}
		virtual bool run() override {
			if (input == true) {
				resultado = "vizinho";
			}else {
				resultado = "pursuit";
			}
			return true;
		}
};

string behaviorTree(Estado estado_ia) {

	Seletor* root = new Seletor;
	Seletor* seletorGeral = new Seletor;
	EstaEmPerigo* estaEmPerigo = new EstaEmPerigo (estado_ia.estaPertoBomba);
	NaoTemMunicao* naoTemMunicao = new NaoTemMunicao (estado_ia.naoTemMunicao);
	EstaVizinho* estaVizinho = new EstaVizinho (estado_ia.estaVizinhoInimigo);
	
	root->addFilho (seletorGeral);
	
	seletorGeral->addFilho (estaEmPerigo);
	seletorGeral->addFilho (naoTemMunicao);
	seletorGeral->addFilho (estaVizinho);
	
	while (!root->run()) {}

	cout << "Resultado da BehaviorTree: " << resultado << endl;

	return resultado;
}