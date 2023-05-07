//Tile class manages all tile objects with 2 basic components: position and texture.

#ifndef Tile_hpp
#define Tile_hpp

#include <SDL2/SDL.h>
#include "Entity.h"
#include "Vector2f.h"

class Tile : public Entity{
public:
    Tile(Vector2f _pos, SDL_Texture* _tex) : Entity( _pos, _tex){}
    
private:
    
};

#endif /* Tile_hpp */
