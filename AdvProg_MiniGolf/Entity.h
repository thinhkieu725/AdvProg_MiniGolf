// Entity class manages fundamental components of an existing object: position, angle, scale, currentFrame and texture.

#ifndef Entity_hpp
#define Entity_hpp

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "Vector2f.h"

class Entity{
public:
    //init func
    Entity(Vector2f _pos, SDL_Texture* _tex);
    ~Entity();
    
    //access to parameters function
    Vector2f& getPos(){
        return pos;
    }
    float getAngle(){
        return angle;
    }
    Vector2f getScale(){
        return scale;
    }
    
    //alter parameters function
    void setPos(float x, float y);
    void setScale(float w, float h);
    void setAngle(float _angle);
    
    SDL_Texture* getTex();              //return the object's texture
    SDL_Rect getCurrentFrame();
    
    void update(double deltaTime);      
    
private:
    Vector2f pos;                       // position (x, y)
    float angle = 0;                    // used for arror: angle between arrow and y-axis
    Vector2f scale = Vector2f(1, 1);    // scale used for rendering
    SDL_Rect currentFrame;              // mainly used to save texture's width and height
    SDL_Texture* tex;                   // texture
};

#endif /* Entity_hpp */
