#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <vector>
#include "Hole.h"
#include "Tile.h"

#include "Entity.h"
#include "Vector2f.h"
#include "RenderWindow.h"

class Ball : public Entity
{
public:
    // init function
	Ball(Vector2f _pos, SDL_Texture* _tex, SDL_Texture* _pointTex, SDL_Texture* _powerMTexFG, SDL_Texture* _powerMTexBG, int _index);
    // get functions
    Vector2f& getVelocity();
    Vector2f& getInitialMousePos();
    std::vector<Entity> getPoints();
    std::vector<Entity> getPowerBar();
    int getStrokes();
    bool isWin();
    //set functions
    void setWin(bool _win);
    void setInitialMousePos(float x, float y);
    void setTarget(float x, float y);
    void setVelocity(float x, float y);
    void setLaunchedVelocity(float x, float y);
    //update by dTime function
    void update(double deltaTime, bool mouseDown, bool mousePressed, std::vector<Tile> tiles, std::vector<Hole> holes,Mix_Chunk* chargeSfx, Mix_Chunk* swingSfx, Mix_Chunk* holeSfx);
private:
    Vector2f velocity;              // velocity 2Dvector
    Vector2f target;                // is used when the ball falls into the hole, makes it move inside the hole
    Vector2f launchedVelocity;      // velocity vector when launched
    float velocity1D;               // = sqrt(velo.x^2 + velo.y^2); is used to modify velocity using ratio
    float launchedVelocity1D;       // velocity1D when launched
    Vector2f initialMousePos;       // mouse position when pressed
    bool canMove = true;            // allows controlling the ball if mouse pressed
    bool playedSwingFx = true;      // is used when mouse released, marks to make swing sound played only once
    int index;                      // 0 for left map, 1 for right map
    int strokes = 0;                // strokes
    int dirX = 1;                   // =1 if the ball is moving to the right; -1 if the ball is moving to the left
    int dirY = 1;                   // =1 if the ball is moving down        ; -1 if the ball is moving up
    bool win = false;               // =true if ball falls into the hole.
    float friction = 0.001;         // friction coefficient
    std::vector<Entity> points;     // direction arror; points[0] for left map, points[1] for right map
    std::vector<Entity> powerBar;   // powerBar[0] for powerBar_backGround; powerBar[1] for frontGround
};
