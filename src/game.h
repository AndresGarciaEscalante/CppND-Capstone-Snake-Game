#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "food.h"
#include <memory>
#include <future>
#include <thread>

class Game {
 public:
  struct gameInformation{std::string player_name; int score; int size;};
  
  Game(std::size_t grid_width, std::size_t grid_height, std::unique_ptr<Food> food);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  void setInitialGameParameters();
  std::string GetPlayerName() const;
  int GetScore() const;
  int GetSize() const;
  void GetEndGameInformation(std::promise<gameInformation>&& promise);

 private:
  Snake snake;
  std::unique_ptr<Food> food_ptr;
  std::string player_name;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif