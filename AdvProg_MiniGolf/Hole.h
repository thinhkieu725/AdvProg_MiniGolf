//Hole class manages all the holes with basic components: position and texture.

#ifndef Hole_hpp
#define Hole_hpp

#include <SDL2/SDL.h>
#include "Entity.h"
#include "Vector2f.h"

class Hole : public Entity{
public:
    Hole(Vector2f _pos, SDL_Texture* _tex) : Entity(_pos, _tex){
    }
    ~Hole(){
    }

private:
    
};

#endif /* Hole_hpp */
