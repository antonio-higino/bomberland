#include <iostream>
#include <list>

using namespace std;

string resultado = "";

struct Estado {
    bool naoTemMunicao = true;
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

class Sequence : public CompositeNode {
	public:
		virtual bool run() override {
			for (Node* child : getChildren()) {  // The generic Sequence implementation.
				if (!child->run())  // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
					return false;
			}
			return true;  // All children suceeded, so the entire run() operation succeeds.
		}
};

class EstaEmPerigo : public Node {  // Each task will be a class (derived from Node of course).
	private:
		bool input;
	public:
		EstaEmPerigo (bool input) : input(input) {}
		virtual bool run() override {
			if (input == true) {
				resultado = "perigo";
				//cout << "The person sees that the door is open." << endl;  // will return true
			//}else
				//cout << "The person sees that the door is closed." << endl;  // will return false
			}
			return input;
		}
};

class NaoTemMunicao : public Node {  // Each task will be a class (derived from Node of course).
	private:
		bool input;
	public:
		NaoTemMunicao (bool input) : input(input) {}
		virtual bool run() override {
			if (input == true) {
				resultado = "municao";
				//cout << "The person sees that the door is open." << endl;  // will return true
			//}else {
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
			if (input == true) {
				resultado = "vizinho";
				//cout << "The person sees that the door is open." << endl;  // will return true
			}else {
				resultado = "pursuit";
				//cout << "The person sees that the door is closed." << endl;  // will return false
			}
			return true;
		}
};

string behaviorTree(Estado estado_ia) {

	//string resultado = "";

	Sequence* root = new Sequence; 
	Selector* seletorGeral = new Selector;  // Note that root can be either a Sequence or a Selector, since it has only one child.
	EstaEmPerigo* estaEmPerigo = new EstaEmPerigo (estado_ia.estaPertoBomba);
	NaoTemMunicao* naoTemMunicao = new NaoTemMunicao (estado_ia.naoTemMunicao);
	EstaVizinho* estaVizinho = new EstaVizinho (estado_ia.estaVizinhoInimigo);
	//NaoEstaVizinho* naoEstaVizinho = new NaoEstaVizinho (estado_ia.estaVizinhoInimigo);
	
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