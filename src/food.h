#ifndef FOOD_H
#define FOOD_H

#include "gameObject.h"
#include "SDL.h"

class Food : public GameObject {
public:
    enum class FoodQuality { Low, High };
    Food(): food_quality(FoodQuality::High){};

    SDL_Point getPosition() const override;
    void setPosition(SDL_Point position);
    FoodQuality getQuality();
private:
    SDL_Point food_position;
    FoodQuality food_quality;
};
#endif