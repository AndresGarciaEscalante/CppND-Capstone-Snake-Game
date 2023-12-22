#include "food.h"
#include "random"

SDL_Point Food::getPosition() const {
    return food_position;
}

void Food::setPosition(SDL_Point position) {
    food_position = position;
}

void Food::setFoodQuality(FoodQuality set_value){
    food_quality = set_value;
}

Food::FoodQuality Food::getQuality() {
    return food_quality;
}

Food::FoodQuality Food::randomlyChangeQuality(std::uniform_int_distribution<int> &random_f, std::mt19937 &engine){
    // Generate a random value and return the corresponding FoodQuality
    return (random_f(engine) == 0) ? FoodQuality::Low : FoodQuality::High;
}
