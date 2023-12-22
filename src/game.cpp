#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height,Food food)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  setInitialGameParameters();
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food.getPosition(), food.getQuality());

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      SDL_Point point;
      point.x = x;
      point.y = y;
      food.setPosition(point);
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.getPosition().x == new_x && food.getPosition().y == new_y && food.getQuality() == Food::FoodQuality::High) {
    score = score + 3;
    PlaceFood();
    // Grow snake
    snake.GrowBody();
  }
  else if (food.getPosition().x == new_x && food.getPosition().y == new_y && food.getQuality() == Food::FoodQuality::Low){
    score++;
    PlaceFood();
    // Grow snake
    snake.GrowBody();  
  } 
}

void Game::setInitialGameParameters(){
  // Read the name of the player
  // Welcome the player
  std::cout << "  W   W  EEEEE  L      CCCCC   OOO   M     M  EEEEE\n";
  std::cout << "  W   W  E      L     C       O   O  MM   MM  E    \n";
  std::cout << "  W W W  EEEE   L     C       O   O  M M M M  EEEE \n";
  std::cout << "  W W W  E      L     C       O   O  M  M  M  E    \n";
  std::cout << "   W W   EEEEE  LLLLL  CCCCC   OOO   M     M  EEEEE\n";
  std::cout << "---------------------------------------------------\n";
  // Add the Initial paramters of the game
  std::cout << "GAME INITIAL SETTINGS \n";
  std::cout << "Insert the name of the player: ";
  std::cin >> player_name;
  std::cout << "Insert the desired number of foods in the game: ";
  std::cin >> number_foods;
}

std::string Game::GetPlayerName() const {return player_name;}
int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }