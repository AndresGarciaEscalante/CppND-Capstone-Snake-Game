#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height,std::unique_ptr<Food> food)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      random_f(0, static_cast<int>(0, 1)),
      food_ptr(std::move(food)) {
  setInitialGameParameters();
  PlaceFood();
}

void Game::timerFoodPosition(){
  Uint32 initial_timer = SDL_GetTicks();
  Uint32 current_timer;
  float time_difference;
  int x, y;
  SDL_Point point;
  while (snake.alive){
    current_timer = SDL_GetTicks();
    time_difference = (current_timer - initial_timer) / 1000.f;
    if(time_difference >= 5.0f){
      x = random_w(engine);
      y = random_h(engine);
      point.x = x;
      point.y = y;
      food_ptr->setPosition(point);
      time_difference = 0.0f;
      initial_timer = SDL_GetTicks();
      current_timer = 0;
    }
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  // Create background thread for changing food position
  std::thread backgroundThread([this](){this->timerFoodPosition();});
  // Detach the separate thread
  backgroundThread.detach();
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food_ptr->getPosition(), food_ptr->getQuality());

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
    food_ptr->setFoodQuality(food_ptr->randomlyChangeQuality(random_f, engine));
    
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      SDL_Point point;
      point.x = x;
      point.y = y;
      food_ptr->setPosition(point);
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
  if (food_ptr->getPosition().x == new_x && food_ptr->getPosition().y == new_y && food_ptr->getQuality() == Food::FoodQuality::High) {
    score = score + 3;
    PlaceFood();
    // Grow snake
    snake.GrowBody();
  }
  else if (food_ptr->getPosition().x == new_x && food_ptr->getPosition().y == new_y && food_ptr->getQuality() == Food::FoodQuality::Low){
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
}

std::string Game::GetPlayerName() const {return player_name;}
int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
void Game::GetEndGameInformation(std::promise<gameInformation>&& promise){
  gameInformation gameInfo;
  gameInfo.player_name = GetPlayerName();
  gameInfo.score = GetScore();
  gameInfo.size = GetSize();
  promise.set_value(gameInfo);
}