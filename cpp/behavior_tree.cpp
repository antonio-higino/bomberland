#include <iostream>
#include <list>

using namespace std;

string resultado = "";

struct Estado {
    bool temMunicao = false;
    bool estaVizinhoInimigo = false;
    bool estaPertoBomba = false;
};

class Node {  // This class represents each node in the behaviour tree.
	public:
		virtual bool run() = 0;
};

class CompositeNode : public Node {  //  This type of Node follows the Composite Pattern, containing a list of other Nodes.
	private:
		list<Node*> children;
	public:
		const list<Node*>& getChildren() const {return children;}
		void addChild (Node* child) {children.emplace_back(child);}
};

class Selector : public CompositeNode {
	public:
		virtual bool run() override {
			for (Node* child : getChildren()) {  // The generic Selector implementation
				if (child->run())  // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
					return true;
			}
			return false;  // All children failed so the entire run() operation fails.
		}
};

class EstaEmPerigo : public Node {  // Each task will be a class (derived from Node of course).
	private:
		bool input;
	public:
		EstaEmPerigo (bool input) : input(input) {}
		virtual bool run() override {
			if (input == true){
				resultado = "perigo";
				//cout << "The person sees that the door is open." << endl;  // will return true
			//else
				//cout << "The person sees that the door is closed." << endl;  // will return false
			}
			return input;
		}
};

class TemMunicao : public Node {  // Each task will be a class (derived from Node of course).
	private:
		bool input;
	public:
		TemMunicao (bool input) : input(input) {}
		virtual bool run() override {
			if (input == true) {
				resultado = "municao";
				//cout << "The person sees that the door is open." << endl;  // will return true
			//else
				//cout << "The person sees that the door is closed." << endl;  // will return false
			}
			return input;
		}
};

class EstaVizinho : public Node {  // Each task will be a class (derived from Node of course).
	private:
		bool input;
	public:
		EstaVizinho (bool input) : input(input) {}
		virtual bool run() override {
			if (input == true)
				resultado = "vizinho";
				//cout << "The person sees that the door is open." << endl;  // will return true
			//else
				//cout << "The person sees that the door is closed." << endl;  // will return false
			return input;
		}
};

class NaoEstaVizinho : public Node {  // Each task will be a class (derived from Node of course).
	private:
		bool input;
	public:
		NaoEstaVizinho (bool input) : input(input) {}
		virtual bool run() override {
			if (input == false)
				resultado = "pursuit";
				//cout << "The person sees that the door is open." << endl;  // will return true
			//else
				//cout << "The person sees that the door is closed." << endl;  // will return false
			return input;
		}
};

string behaviorTree(Estado estado_ia) {

	//string resultado = "";

	Selector *root = new Selector, *seletorGeral = new Selector, *vizinhoInimigo = new Selector;  // Note that root can be either a Sequence or a Selector, since it has only one child.
	EstaEmPerigo* estaEmPerigo = new EstaEmPerigo (estado_ia.estaPertoBomba);
	TemMunicao* temMunicao = new TemMunicao (estado_ia.temMunicao);
	EstaVizinho* estaVizinho = new EstaVizinho (estado_ia.estaVizinhoInimigo);
	NaoEstaVizinho* naoEstaVizinho = new NaoEstaVizinho (estado_ia.estaVizinhoInimigo);
	
	root->addChild (seletorGeral);
	
	seletorGeral->addChild (estaEmPerigo);
	seletorGeral->addChild (temMunicao);
	seletorGeral->addChild (vizinhoInimigo);
	
	vizinhoInimigo->addChild (estaVizinho);
	vizinhoInimigo->addChild (naoEstaVizinho);
	
	while (!root->run())  // If the operation starting from the root fails, keep trying until it succeeds.
		//cout << "--------------------" << endl;
	//cout << endl << "Operation complete.  Behaviour tree exited." << endl;
	//cin.get();

	cout << "Resultado da BehaviorTree: " << resultado << endl;

	return resultado;
}