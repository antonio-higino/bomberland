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

class CompositeNode : public Node {
	private:
		list<Node*> children;
	public:
		const list<Node*>& getChildren() const {return children;}
		void addChild (Node* child) {children.emplace_back(child);}
};

class Selector : public CompositeNode {
	public:
		virtual bool run() override {
			for (Node* child : getChildren()) { 
				if (child->run())
					return true;
			}
			return false;
		}
};

class Sequence : public CompositeNode {
	public:
		virtual bool run() override {
			for (Node* child : getChildren()) { 
				if (!child->run()) 
					return false;
			}
			return true;
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

	//string resultado = "";

	Sequence* root = new Sequence; 
	Selector* seletorGeral = new Selector;
	EstaEmPerigo* estaEmPerigo = new EstaEmPerigo (estado_ia.estaPertoBomba);
	NaoTemMunicao* naoTemMunicao = new NaoTemMunicao (estado_ia.naoTemMunicao);
	EstaVizinho* estaVizinho = new EstaVizinho (estado_ia.estaVizinhoInimigo);
	
	root->addChild (seletorGeral);
	
	seletorGeral->addChild (estaEmPerigo);
	seletorGeral->addChild (naoTemMunicao);
	seletorGeral->addChild (estaVizinho);
	
	while (!root->run())  // If the operation starting from the root fails, keep trying until it succeeds.
	//root->run();
		//cout << "--------------------" << endl;
	//cout << endl << "Operation complete.  Behaviour tree exited." << endl;
	//cin.get();

	cout << "Resultado da BehaviorTree: " << resultado << endl;

	return resultado;
}