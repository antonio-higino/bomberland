/*#include "a_estrela.cpp"

int main() {
  GradesComPeso grid = GerarDiagrama();
  LocalizacaoGrade start{2, 5}, goal{6, 9};
  std::unordered_map<LocalizacaoGrade, LocalizacaoGrade> came_from;
  std::unordered_map<LocalizacaoGrade, double> cost_so_far;
  buscaAestrela(grid, start, goal, came_from, cost_so_far);
  desenharGrade(grid, nullptr, &came_from, nullptr, &start, &goal);
  std::cout << '\n';
  std::vector<LocalizacaoGrade> path = reconstruirCaminho(start, goal, came_from);
  desenharGrade(grid, nullptr, nullptr, &path, &start, &goal);
  std::cout << '\n';
  desenharGrade(grid, &cost_so_far, nullptr, nullptr, &start, &goal);
}*/