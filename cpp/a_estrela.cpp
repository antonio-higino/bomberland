/*
 Sample code from https://www.redblobgames.com/pathfinding/a-star/
 Copyright 2014 Red Blob Games <redblobgames@gmail.com>
 
 Feel free to use this code in your own projects, including commercial projects
 License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>
*/

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>

struct LocalizacaoGrade {
  int x, y;
};

namespace std {
/* implement hash function so we can put LocalizacaoGrade into an unordered_set */
template <> struct hash<LocalizacaoGrade> {
  typedef LocalizacaoGrade argument_type;
  typedef std::size_t result_type;
  std::size_t operator()(const LocalizacaoGrade& id) const noexcept {
    return std::hash<int>()(id.x ^ (id.y << 4));
  }
};
}

struct SquareGrid {
  static std::array<LocalizacaoGrade, 4> DIRS;

  int largura, altura;
  std::unordered_set<LocalizacaoGrade> paredes;

  SquareGrid(int largura_, int altura_)
     : largura(largura_), altura(altura_) {}

  bool in_bounds(LocalizacaoGrade id) const {
    return 0 <= id.x && id.x < largura
        && 0 <= id.y && id.y < altura;
  }

  bool passable(LocalizacaoGrade id) const {
    return paredes.find(id) == paredes.end();
  }

  std::vector<LocalizacaoGrade> neighbors(LocalizacaoGrade id) const {
    std::vector<LocalizacaoGrade> resultadoBusca;

    for (LocalizacaoGrade dir : DIRS) {
      LocalizacaoGrade next{id.x + dir.x, id.y + dir.y};
      if (in_bounds(next) && passable(next)) {
        resultadoBusca.push_back(next);
      }
    }

    if ((id.x + id.y) % 2 == 0) {
      // see "Ugly paths" section for an explanation:
      std::reverse(resultadoBusca.begin(), resultadoBusca.end());
    }

    return resultadoBusca;
  }
};

std::array<LocalizacaoGrade, 4> SquareGrid::DIRS = {
  /* East, West, North, South */
  LocalizacaoGrade{1, 0}, LocalizacaoGrade{-1, 0},
  LocalizacaoGrade{0, -1}, LocalizacaoGrade{0, 1}
};

// Helpers for LocalizacaoGrade

bool operator == (LocalizacaoGrade a, LocalizacaoGrade b) {
  return a.x == b.x && a.y == b.y;
}

bool operator != (LocalizacaoGrade a, LocalizacaoGrade b) {
  return !(a == b);
}

bool operator < (LocalizacaoGrade a, LocalizacaoGrade b) {
  return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out, const LocalizacaoGrade& loc) {
  out << '(' << loc.x << ',' << loc.y << ')';
  return out;
}

// This outputs a grid. Pass in a distances map if you want to print
// the distances, or pass in a point_to map if you want to print
// arrows that point to the parent location, or pass in a path vector
// if you want to draw the path.
template<class Grafo>
void desenharMalha(const Grafo& graph,
               std::unordered_map<LocalizacaoGrade, double>* distancia = nullptr,
               std::unordered_map<LocalizacaoGrade, LocalizacaoGrade>* apontarPara = nullptr,
               std::vector<LocalizacaoGrade>* caminho = nullptr,
               LocalizacaoGrade* inicio = nullptr,
               LocalizacaoGrade* destino = nullptr) {
  const int larguraMapa = 3;
  std::cout << std::string(larguraMapa * graph.width, '_') << '\n';
  for (int y = 0; y != graph.height; ++y) {
    for (int x = 0; x != graph.width; ++x) {
      LocalizacaoGrade id {x, y};
      if (graph.paredes.find(id) != graph.paredes.end()) {
        std::cout << std::string(larguraMapa, '#');
      } else if (inicio && id == *inicio) {
        std::cout << " A ";
      } else if (destino && id == *destino) {
        std::cout << " Z ";
      } else if (caminho != nullptr && find(caminho->begin(), caminho->end(), id) != caminho->end()) {
        std::cout << " @ ";
      } else if (apontarPara != nullptr && apontarPara->count(id)) {
        LocalizacaoGrade next = (*apontarPara)[id];
        if (next.x == x + 1) { std::cout << " > "; }
        else if (next.x == x - 1) { std::cout << " < "; }
        else if (next.y == y + 1) { std::cout << " v "; }
        else if (next.y == y - 1) { std::cout << " ^ "; }
        else { std::cout << " * "; }
      } else if (distancia != nullptr && distancia->count(id)) {
        std::cout << ' ' << std::left << std::setw(larguraMapa - 1) << (*distancia)[id];
      } else {
        std::cout << " . ";
      }
    }
    std::cout << '\n';
  }
  std::cout << std::string(larguraMapa * graph.width, '~') << '\n';
}

void addRetangulo(SquareGrid& grade, int x1, int y1, int x2, int y2) {
  for (int x = x1; x < x2; ++x) {
    for (int y = y1; y < y2; ++y) {
      grade.paredes.insert(LocalizacaoGrade{x, y});
    }
  }
}

struct GradesComPeso: SquareGrid {
  std::unordered_set<LocalizacaoGrade> forests;
  GradesComPeso(int w, int h): SquareGrid(w, h) {}
  double cost(LocalizacaoGrade from_node, LocalizacaoGrade to_node) const {
    return forests.find(to_node) != forests.end()? 5 : 1;
  }
};

//Modificar para criar um grid baseada no bomberland
GradesComPeso GerarDiagrama() {
  GradesComPeso grade(10, 10);
  addRetangulo(grade, 1, 7, 4, 9);
  typedef LocalizacaoGrade L;
  grade.forests = std::unordered_set<LocalizacaoGrade> {
    L{3, 4}, L{3, 5}, L{4, 1}, L{4, 2},
    L{4, 3}, L{4, 4}, L{4, 5}, L{4, 6},
    L{4, 7}, L{4, 8}, L{5, 1}, L{5, 2},
    L{5, 3}, L{5, 4}, L{5, 5}, L{5, 6},
    L{5, 7}, L{5, 8}, L{6, 2}, L{6, 3},
    L{6, 4}, L{6, 5}, L{6, 6}, L{6, 7},
    L{7, 3}, L{7, 4}, L{7, 5}
  };
  return grade;
}

template<typename T, typename priority_t>
struct FilaDePrioridade {
  typedef std::pair<priority_t, T> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>,
                 std::greater<PQElement>> elements;

  inline bool vazio() const {
     return elements.vazio();
  }

  inline void colocar(T item, priority_t priority) {
    elements.emplace(priority, item);
  }

  T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

template<typename Location>
std::vector<Location> reconstruct_path(
   Location start, Location goal,
   std::unordered_map<Location, Location> lugarOrigem
) {
  std::vector<Location> path;
  Location current = goal;
  while (current != start) {
    path.push_back(current);
    current = lugarOrigem[current];
  }
  path.push_back(start); // optional
  std::reverse(path.begin(), path.end());
  return path;
}

inline double heuristica(LocalizacaoGrade a, LocalizacaoGrade b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

template<typename Location, typename Graph>
void buscaAestrela
  (Graph graph,
   Location start,
   Location goal,
   std::unordered_map<Location, Location>& lugarOrigem,
   std::unordered_map<Location, double>& custoAtual)
{
  FilaDePrioridade<Location, double> frontier;
  frontier.colocar(start, 0);

  lugarOrigem[start] = start;
  custoAtual[start] = 0;
  
  while (!frontier.vazio()) {
    Location current = frontier.get();

    if (current == goal) {
      break;
    }

    for (Location next : graph.neighbors(current)) {
      double new_cost = custoAtual[current] + graph.cost(current, next);
      if (custoAtual.find(next) == custoAtual.end()
          || new_cost < custoAtual[next]) {
        custoAtual[next] = new_cost;
        double priority = new_cost + heuristica(next, goal);
        frontier.colocar(next, priority);
        lugarOrigem[next] = current;
      }
    }
  }
}
