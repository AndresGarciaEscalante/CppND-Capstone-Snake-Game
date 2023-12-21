#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "SDL.h"

class GameObject {
 public:
    virtual SDL_Point getPosition() const = 0;  // Pure virtual function 
    virtual ~GameObject() = default; // Virtual destructor for proper cleanup
};
#endif