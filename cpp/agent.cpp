#include "game_state.hpp"
#include "a_estrela.cpp"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <random>

const std::vector<std::string> _actions = {"up", "left", "right", "down", "bomb", "detonate"};

class Agent {
private:
  static std::string my_agent_id;
  static std::vector<std::string> my_units;

  static int tick;

  static void on_game_tick(int tick, const json& game_state);
  static std::string pathFinder(int tick, const json& game_state);
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

  srand(1234567 * (my_agent_id == "a" ? 1 : 0) + tick * 100 + 13);

  // send each (alive) unit a random action
  for (const auto& unit_id: my_units)
  {
    const json& unit = game_state["unit_state"][unit_id];
    if (unit["hp"] <= 0) continue;
    //std::string action = _actions[rand() % _actions.size()];
    std::string action = pathFinder(tick, game_state);
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

std::string Agent::pathFinder(int tick, const json& game_state) {

  GradesComPeso grade(15, 15);
  //grade.pontosComPeso = std::unordered_set<LocalizacaoGrade> {};

  const json& entities = game_state["entities"];
  for (const auto& entity: entities) {
    int x = entity["x"];
    int y = entity["y"];
    if (entity["type"] == "a") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "b") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "x") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "bp") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "m") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "o") {
      addEntidade(grade, x, y);
    } else if (entity["type"] == "w") {
      addEntidade(grade, x, y);
    }
  }

  const json& unit_ia = game_state["unit_state"]["d"];
  const json& unit_inimigo = game_state["unit_state"]["c"];

  std::vector<int> coordenadas_ia = unit_ia["coordinates"];
  std::vector<int> coordenadas_inimigo = unit_inimigo["coordinates"];

  LocalizacaoGrade inicio{coordenadas_ia[0], coordenadas_ia[1]}, destino{coordenadas_inimigo[0], coordenadas_inimigo[1]};
  std::unordered_map<LocalizacaoGrade, LocalizacaoGrade> veioDe;
  std::unordered_map<LocalizacaoGrade, double> custoAteAgora;
  
  buscaAestrela(grade, inicio, destino, veioDe, custoAteAgora);
  //desenharGrade(grade, nullptr, &veioDe, nullptr, &inicio, &destino);
  //std::cout << '\n';
  
  std::vector<LocalizacaoGrade> caminho = reconstruirCaminho(inicio, destino, veioDe);
  std::cout << "Proxima localizacao: ";
  std::cout << caminho[1] << std::endl;
  
  //desenharGrade(grade, nullptr, nullptr, &caminho, &inicio, &destino);
  //std::cout << '\n';
  //desenharGrade(grade, &custoAteAgora, nullptr, nullptr, &inicio, &destino);

  if(caminho[1].x > inicio.x) {
    return "right";
  } else if(caminho[1].x < inicio.x) {
    return "left";
  } else if(caminho[1].y < inicio.y) {
    return "down";
  } else if(caminho[1].y > inicio.y) {
    return "up";
  } else {
    return "right";
  }

  //return _actions[rand() % _actions.size()];
}

int main()
{
  Agent::run();
}
