#include "food.h"
#include "random"

SDL_Point Food::getPosition() const {
    return food_position;
}

void Food::setPosition(SDL_Point position) {
    food_position = position;
}

Food::FoodQuality Food::getQuality() {
    return food_quality;
}

