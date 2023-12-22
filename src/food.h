#ifndef FOOD_H
#define FOOD_H

#include "gameObject.h"
#include "SDL.h"
#include <random>

class Food : public GameObject {
public:
    enum class FoodQuality { Low, High };
    Food(): food_quality(FoodQuality::High){};

    SDL_Point getPosition() const override;
    void setPosition(SDL_Point position);
    void setFoodQuality(FoodQuality set_value);
    FoodQuality getQuality();
    FoodQuality randomlyChangeQuality(std::uniform_int_distribution<int> &random_f, std::mt19937 &engine);
private:
    SDL_Point food_position;
    FoodQuality food_quality;
};
#endif