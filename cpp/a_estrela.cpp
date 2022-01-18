#include <iostream>
//#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>

using namespace std;

struct LocalizacaoGrade {
  int x, y;
};

namespace std {
template <> struct hash<LocalizacaoGrade> {
  typedef LocalizacaoGrade argument_type;
  typedef size_t result_type;
  size_t operator()(const LocalizacaoGrade& id) const noexcept {
    return hash<int>()(id.x ^ (id.y << 4));
  }
};
}

struct GradeQuadrada {
  static array<LocalizacaoGrade, 4> Direcoes;

  int largura, altura;

  unordered_set<LocalizacaoGrade> paredes;

  GradeQuadrada(int largura_, int altura_)
     : largura(largura_), altura(altura_) {}

  bool dentroDoMapa(LocalizacaoGrade id) const {
    return 0 <= id.x && id.x < largura && 0 <= id.y && id.y < altura;
  }

  bool navegavel(LocalizacaoGrade id) const {
    return paredes.find(id) == paredes.end();
  }

  vector<LocalizacaoGrade> vizinhos(LocalizacaoGrade id) const {
    vector<LocalizacaoGrade> resultadoBusca;

    for (LocalizacaoGrade dir : Direcoes) {
      LocalizacaoGrade next{id.x + dir.x, id.y + dir.y};
      if (dentroDoMapa(next) && navegavel(next)) {
        resultadoBusca.push_back(next);
      }
    }

    if ((id.x + id.y) % 2 == 0) {
      reverse(resultadoBusca.begin(), resultadoBusca.end());
    }

    return resultadoBusca;
  }
};

array<LocalizacaoGrade, 4> GradeQuadrada::Direcoes = {
  /* Leste, Oeste, Norte, Sul */
  LocalizacaoGrade{1, 0}, LocalizacaoGrade{-1, 0},
  LocalizacaoGrade{0, -1}, LocalizacaoGrade{0, 1}
};

// Funções para ajudar LocalizacaoGrade
bool operator == (LocalizacaoGrade a, LocalizacaoGrade b) {
  return a.x == b.x && a.y == b.y;
}

bool operator != (LocalizacaoGrade a, LocalizacaoGrade b) {
  return !(a == b);
}

bool operator < (LocalizacaoGrade a, LocalizacaoGrade b) {
  return tie(a.x, a.y) < tie(b.x, b.y);
}

basic_iostream<char>::basic_ostream& operator<<(basic_iostream<char>::basic_ostream& out, const LocalizacaoGrade& loc) {
  out << '(' << loc.x << ',' << loc.y << ')';
  return out;
}

// This outputs a grid. Pass in a distances map if you want to print
// the distances, or pass in a point_to map if you want to print
// arrows that point to the parent Localizacao, or pass in a caminho vector
// if you want to draw the caminho.
/*template<class Grafo>
void desenharGrade(const Grafo& graph,
               unordered_map<LocalizacaoGrade, double>* distancia = nullptr,
               unordered_map<LocalizacaoGrade, LocalizacaoGrade>* apontarPara = nullptr,
               vector<LocalizacaoGrade>* caminho = nullptr,
               LocalizacaoGrade* inicio = nullptr,
               LocalizacaoGrade* destino = nullptr) {
  const int larguraMapa = 3;
  cout << string(larguraMapa * graph.largura, '_') << '\n';
  for (int y = 0; y != graph.altura; ++y) {
    for (int x = 0; x != graph.largura; ++x) {
      LocalizacaoGrade id {x, y};
      if (graph.paredes.find(id) != graph.paredes.end()) {
        cout << string(larguraMapa, '#');
      } else if (inicio && id == *inicio) {
        cout << " A ";
      } else if (destino && id == *destino) {
        cout << " Z ";
      } else if (caminho != nullptr && find(caminho->begin(), caminho->end(), id) != caminho->end()) {
        cout << " @ ";
      } else if (apontarPara != nullptr && apontarPara->count(id)) {
        LocalizacaoGrade next = (*apontarPara)[id];
        if (next.x == x + 1) { cout << " > "; }
        else if (next.x == x - 1) { cout << " < "; }
        else if (next.y == y + 1) { cout << " v "; }
        else if (next.y == y - 1) { cout << " ^ "; }
        else { cout << " * "; }
      } else if (distancia != nullptr && distancia->count(id)) {
        cout << ' ' << left << setw(larguraMapa - 1) << (*distancia)[id];
      } else {
        cout << " . ";
      }
    }
    cout << '\n';
  }
  cout << string(larguraMapa * graph.largura, '~') << '\n';
}*/

void addEntidade(GradeQuadrada& grade, int x, int y) {
  grade.paredes.insert(LocalizacaoGrade{x, y});
}

struct GradesComPeso: GradeQuadrada {
  unordered_set<LocalizacaoGrade> pontosComPeso;
  GradesComPeso(int w, int h): GradeQuadrada(w, h) {}
  double custo(LocalizacaoGrade from_node, LocalizacaoGrade to_node) const {
    return pontosComPeso.find(to_node) != pontosComPeso.end()? 5 : 1;
  }
};

template<typename T, typename priority_t>
struct FilaDePrioridade {
  typedef pair<priority_t, T> PQElement;
  priority_queue<PQElement, vector<PQElement>,
                 greater<PQElement>> elementos;

  inline bool empty() const {
     return elementos.empty();
  }

  inline void put(T item, priority_t priority) {
    elementos.emplace(priority, item);
  }

  T get() {
    T best_item = elementos.top().second;
    elementos.pop();
    return best_item;
  }
};

template<typename Localizacao>
vector<Localizacao> reconstruirCaminho(Localizacao inicio, Localizacao destino, unordered_map<Localizacao, Localizacao> veioDe) {
  vector<Localizacao> caminho;
  Localizacao current = destino;
  while (current != inicio) {
    caminho.push_back(current);
    current = veioDe[current];
  }
  caminho.push_back(inicio); // optional
  reverse(caminho.begin(), caminho.end());
  return caminho;
}

inline double heuristica(LocalizacaoGrade a, LocalizacaoGrade b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

template<typename Localizacao, typename Grafo>
void buscaAestrela
  (Grafo graph, 
  Localizacao inicio, 
  Localizacao destino, 
  unordered_map<Localizacao, Localizacao>& veioDe, 
  unordered_map<Localizacao, double>& custoAteAgora) {
  
  FilaDePrioridade<Localizacao, double> fronteira;
  fronteira.put(inicio, 0);

  veioDe[inicio] = inicio;
  custoAteAgora[inicio] = 0;
  
  while (!fronteira.empty()) {
    Localizacao current = fronteira.get();

    if (current == destino) {
      break;
    }

    for (Localizacao next : graph.vizinhos(current)) {
      double novoCusto = custoAteAgora[current] + graph.custo(current, next);
      if (custoAteAgora.find(next) == custoAteAgora.end() || novoCusto < custoAteAgora[next]) {
        custoAteAgora[next] = novoCusto;
        double priority = novoCusto + heuristica(next, destino);
        fronteira.put(next, priority);
        veioDe[next] = current;
      }
    }
  }
}