#include "game_state.hpp"
#include "a_estrela.cpp"
#include "behavior_tree.cpp"

#include <string>
#include <stdlib.h>
#include <iostream>

const std::vector<std::string> _actions = {"up", "left", "right", "down", "bomb", "detonate"};

class Agent {
private:
  static std::string my_agent_id;
  static std::vector<std::string> my_units;

  static int tick;

  static void on_game_tick(int tick, const json& game_state);
  static int distanciaAbsoluta(std::vector<int> coordenadas_inicio, std::vector<int> coordenadas_alvo);
  static std::string escolherOrdem(const json& game_state);
  static std::string pathFinder(std::vector<int> coordenadas_inicio, std::vector<int> coordenadas_alvo, const json& game_state);
public:
  static void run();
};

std::string Agent::my_agent_id;
std::vector<std::string> Agent::my_units;

int Agent::tick;

void Agent::run() 
{
  const char* connection_string = getenv ("GAME_CONNECTION_STRING");

  if (connection_string == NULL) 
  {
    connection_string = "ws://127.0.0.1:3000/?role=agent&agentId=agentId&name=defaultName";
  }
  std::cout << "The current connection_string is: " << connection_string << std::endl;

  GameState::connect(connection_string); 
  GameState::set_game_tick_callback(on_game_tick);
  GameState::handle_messages();
}

void Agent::on_game_tick(int tick_nr, const json& game_state) 
{
  DEBUG("*************** on_game_tick");
  DEBUG(game_state.dump());
  TEST(tick_nr, game_state.dump());
  
  tick = tick_nr;
  std::cout << "Tick #" << tick << std::endl;
  if (tick == 1)
  {
    my_agent_id = game_state["connection"]["agent_id"];
    my_units = game_state["agents"][my_agent_id]["unit_ids"].get<std::vector<std::string>>();
  }

  for (const auto& unit_id: my_units)
  {
    const json& unit = game_state["unit_state"][unit_id];
    if (unit["hp"] <= 0) continue;
    //Mandar ordem
    std::string action = escolherOrdem(game_state);
    std::cout << "action: " << unit_id << " -> " << action << std::endl;

    if (action == "up" || action == "left" || action == "right" || action == "down")
    {
      GameState::send_move(action, unit_id);
    }
    else if (action == "bomb")
    {
      GameState::send_bomb(unit_id);
    }
    else if (action == "detonate")
    {
      const json& entities = game_state["entities"];
      for (const auto& entity: entities) 
      {
        if (entity["type"] == "b" && entity["unit_id"] == unit_id) 
        {
          int x = entity["x"], y = entity["y"];
          GameState::send_detonate(x, y, unit_id);
          break;
        }
      }
    }
    else 
    {
      std::cerr << "Unhandled action: " << action << " for unit " << unit_id << std::endl;
    }
  }
}

//Serve para calcular a distancia entre duas unidades/entidades
int Agent::distanciaAbsoluta(std::vector<int> coordenadas_inicio, std::vector<int> coordenadas_alvo) {
  int distanciaX = coordenadas_inicio[0] - coordenadas_alvo[0];
  int distanciaY = coordenadas_inicio[1] - coordenadas_alvo[1];
  return abs(distanciaX) + abs(distanciaY);

  /*if(distaciaAbsoluta == 1) {
    return true;
  } else {
    return false;
  }*/
}

std::string Agent::escolherOrdem(const json& game_state) {

  std::string id_meu_agente = game_state["connection"]["agent_id"];
  std::string id_ia = "";
  std::string id_inimigo = "";

  if(id_meu_agente == "a") {
    id_ia = "c";
    id_inimigo = "d";
  } else {
    id_ia = "d";
    id_inimigo = "c";
  }

  const json& unit_ia = game_state["unit_state"][id_ia];
  const json& unit_inimigo = game_state["unit_state"][id_inimigo];

  std::vector<int> coordenadas_inicio = unit_ia["coordinates"];
  std::vector<int> coordenadas_inimigo = unit_inimigo["coordinates"];
  //std::vector<int> coordenadas_bomba = {0,0};
  std::vector<int> coordenadas_municao = {7,7};
  std::vector<int> coordenadas_seguro = {7,7};

  Estado estado_ia;

  if(unit_ia["inventory"]["bombs"] > 0){
    estado_ia.naoTemMunicao = false;
  }

  if(distanciaAbsoluta(coordenadas_inicio, coordenadas_inimigo) == 1) {
    estado_ia.estaVizinhoInimigo = true;
  }

  const json& entities = game_state["entities"];
  for(const auto& entity: entities) {
    if(entity["type"] == "b") {
      std::vector<int> coordenadas_bomba = {entity["x"], entity["y"]};
      if(distanciaAbsoluta(coordenadas_inicio, coordenadas_bomba) <= 3) {
        estado_ia.estaPertoBomba = true;
      }
    }
  }

  for(const auto& entity: entities) {
    if(entity["type"] == "a") {
      coordenadas_municao = {entity["x"], entity["y"]};
    }
  }

  //std::cout << "Municao: " << estado_ia.temMunicao << std::endl;
  //std::cout << "Vizinho: " << estado_ia.estaVizinhoInimigo << std::endl;
  //std::cout << "Perto Bomba: " << estado_ia.estaPertoBomba << std::endl;

  std::string resultado = behaviorTree(estado_ia);

  if(resultado == "perigo") {
    return pathFinder(coordenadas_inicio, coordenadas_seguro, game_state);
  } else if(resultado == "municao") {
    return pathFinder(coordenadas_inicio, coordenadas_municao, game_state);
  } else if(resultado == "pursuit") {
    return pathFinder(coordenadas_inicio, coordenadas_inimigo, game_state);
  } else if(resultado == "vizinho") {
    return "bomb";
  } else {
    return "down";
  }

  //-------------Tomada de decisao-------------
  //Se estiver vizinho ao inimigo, tenta soltar bomba
  //Senão, chama o pathfinding para ir até o inimigo
  /*if(estado_ia.estaVizinhoInimigo){
    std::cout << "Estou vizinho ao inimigo" << std::endl;
    if(estado_ia.temBomba = true){
      return "bomb";
    } else {
      std::cout << "Estou sem bombas" << std::endl;
      return pathFinder(coordenadas_inicio, coordenadas_alvo, game_state);
    }
  } else {
    return pathFinder(coordenadas_inicio, coordenadas_alvo, game_state);
  }*/
}

std::string Agent::pathFinder(std::vector<int> coordenadas_inicio, std::vector<int> coordenadas_alvo, const json& game_state) {
  GradesComPeso grade(15, 15);
  
  //Preenche a grade com as entidades que são obstáculos
  const json& entities = game_state["entities"];
  for (const auto& entity: entities) {
    int x = entity["x"];
    int y = entity["y"];

    if (entity["type"] == "b") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "x") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "m") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "o") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "w") {
      addEntidade(grade, x, y);
    }
  }

  LocalizacaoGrade inicio{coordenadas_inicio[0], coordenadas_inicio[1]}, destino{coordenadas_alvo[0], coordenadas_alvo[1]};
  std::unordered_map<LocalizacaoGrade, LocalizacaoGrade> veioDe;
  std::unordered_map<LocalizacaoGrade, double> custoAteAgora;

  buscaAestrela(grade, inicio, destino, veioDe, custoAteAgora);
  std::vector<LocalizacaoGrade> caminho = reconstruirCaminho(inicio, destino, veioDe);

  if(caminho[1].x > inicio.x) {
    return "right";
  } else if(caminho[1].x < inicio.x) {
    return "left";
  } else if(caminho[1].y < inicio.y) {
    return "down";
  } else if(caminho[1].y > inicio.y) {
    return "up";
  }
}

int main()
{
  Agent::run();
}
