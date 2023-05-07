#include "Entity.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "Constants.h"

Entity::Entity(Vector2f _pos, SDL_Texture* _tex) : pos(_pos), tex(_tex){
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 32;
    currentFrame.h = 32;
    SDL_QueryTexture(_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
}
Entity::~Entity(){
}
// set function
void Entity::setPos(float x, float y){
    pos.x = x;
    pos.y = y;
}
void Entity::setScale(float w, float h){
    scale.x = w;
    scale.y = h;
}
void Entity::setAngle(float _angle){
    angle = _angle;
}
// get function
SDL_Texture* Entity::getTex(){
    return tex;
}
SDL_Rect Entity::getCurrentFrame(){
    return currentFrame;
}

void Entity::update(double deltaTime){}
