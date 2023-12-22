#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "food.h"
#include <memory>
#include <future>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  
  auto food = std::make_unique<Food>();
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight, std::move(food));
  game.Run(controller, renderer, kMsPerFrame);

  //Use promise - future to retrieve score 
  std::promise<Game::gameInformation> myPromise;
  std::future<Game::gameInformation> myFuture = myPromise.get_future();

  // Start a thread to produce the result using the MyClass method
  std::thread producer(&Game::GetEndGameInformation, &game, std::move(myPromise));
  Game::gameInformation gameInfo = myFuture.get();
  
  // Printout the final Score
  std::cout << "---------------------------------------------------\n";
  std::cout << "END OF THE GAME !!! WELL DONE \n";
  std::cout << "Player: "<< gameInfo.player_name <<" has terminated successfully!\n";
  std::cout << "Score: " << gameInfo.score << "\n";
  std::cout << "Size: " << gameInfo.size << "\n";

  //thread barrier
  producer.join();
  return 0;
}