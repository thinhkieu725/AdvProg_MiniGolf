#include "Ball.h"
#include "Entity.h"
#include "Vector2f.h"
#include "Tile.h"
#include "Hole.h"
#include "Constants.h"

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

//init function
Ball::Ball(Vector2f _pos, SDL_Texture* _tex, SDL_Texture* _pointTex, SDL_Texture* _powerMTexFG, SDL_Texture* _powerMTexBG, int _index)
:Entity(_pos, _tex)
{
    index = _index;
    points.push_back(Entity(Vector2f(-64, -64), _pointTex));
    powerBar.push_back(Entity(Vector2f(-64, -64), _powerMTexBG));
    powerBar.push_back(Entity(Vector2f(-64, -64), _powerMTexFG));
}
//get functions
Vector2f& Ball::getVelocity(){
    return velocity;
}
Vector2f& Ball::getInitialMousePos(){
    return initialMousePos;
}
std::vector<Entity> Ball::getPoints(){
    return points;
}
std::vector<Entity> Ball::getPowerBar(){
    return powerBar;
}
int Ball::getStrokes(){
    return strokes;
}
bool Ball::isWin(){
    return win;
}
//set functions
void Ball::setWin(bool _win){
    win = _win;
}
void Ball::setTarget(float x, float y){
    target.x = x;
    target.y = y;
}
void Ball::setVelocity(float x, float y){
    velocity.x = x;
    velocity.y = y;
}
void Ball::setLaunchedVelocity(float x, float y){
    launchedVelocity.x = x;
    launchedVelocity.y = y;
}
void Ball::setInitialMousePos(float x, float y){
    initialMousePos.x = x;
    initialMousePos.y = y;
}
//update by dTime function
void Ball::update(double deltaTime, bool mouseDown, bool mousePressed, std::vector<Tile> tiles,std::vector<Hole> holes, Mix_Chunk* chargeSfx, Mix_Chunk* swingSfx, Mix_Chunk* holeSfx){
    //if the ball falls into the hole, makes the ball move around inside the hole and smaller by time
    if (win){
        if (getPos().x < target.x)
        {
            setPos(getPos().x += 0.1*deltaTime, getPos().y);
        }
        else if (getPos().x > target.x)
        {
            setPos(getPos().x -= 0.1*deltaTime, getPos().y);
        }
        if (getPos().y < target.y)
        {
            setPos(getPos().x, getPos().y += 0.1*deltaTime);
        }
        else if (getPos().y > target.y)
        {
            setPos(getPos().x, getPos().y -= 0.1*deltaTime);
        }
        setScale(getScale().x - 0.001*deltaTime, getScale().y - 0.001*deltaTime);
        return;
    }
    //check if balls fall into holes
    for (Hole h : holes)
    {
        if (getPos().x + 4 > h.getPos().x && getPos().x + 16 < h.getPos().x + 20 && getPos().y + 4 > h.getPos().y && getPos().y + 16 < h.getPos().y + 20)
        {
            Mix_PlayChannel(-1, holeSfx, 0);
            setWin(true);
            //setTarget
            setTarget(h.getPos().x, h.getPos().y + 3);
        }
    }
    // process mousePressed signal when canMove
    if (mousePressed && canMove)
    {
        Mix_PlayChannel(-1, chargeSfx, 0);      // play chargeSound
        playedSwingFx = false;                  // prepare swingSound
        int mouseX = 0;                         //
	    int mouseY = 0;
	    SDL_GetMouseState(&mouseX, &mouseY);
        setInitialMousePos(mouseX, mouseY);     // setInitialMousePos at pressed position
    }
    //process mouse moving signals
    if (mouseDown && canMove)
    {
        //get mouse position
        int mouseX = 0;
	    int mouseY = 0;
	    SDL_GetMouseState(&mouseX, &mouseY);
        //calculate velocity
        setVelocity((mouseX - getInitialMousePos().x)/VELO_PER_PULLDIS, (mouseY - getInitialMousePos().y)/VELO_PER_PULLDIS);
        setLaunchedVelocity((mouseX - getInitialMousePos().x)/VELO_PER_PULLDIS, (mouseY - getInitialMousePos().y)/VELO_PER_PULLDIS);
        velocity1D = SDL_sqrt(SDL_pow(abs(getVelocity().x), 2) + SDL_pow(abs(getVelocity().y), 2));
        launchedVelocity1D = velocity1D;
        
        // set arror's position and angle
        points.at(0).setPos(getPos().x, getPos().y + 8 - 32);
        points.at(0).setAngle(SDL_atan2(velocity.y, velocity.x)*(RAD_TO_DEG_RATIO) + 90);
        
        // set direction
        dirX = velocity.x/abs(velocity.x);
        dirY = velocity.y/abs(velocity.y);

        // set powerBar position
        powerBar.at(0).setPos(getPos().x + 32 + 8, getPos().y - 32);
        powerBar.at(1).setPos(getPos().x + 32 + 8 + 4, getPos().y - 32 + 4 + 32 - 32*powerBar.at(1).getScale().y);
        if (velocity1D > 1)
        {
            velocity1D = 1;
            launchedVelocity1D = 1;
        }
        powerBar.at(1).setScale(1, velocity1D/1);
    }
    // when mouse is released
    else
    {
        // plau swing sound; increase stroke
        if (!playedSwingFx)
        {
            Mix_PlayChannel(-1, swingSfx, 0);
            playedSwingFx = true;
            strokes++;
        }
        // hide arrow and power bar
        points.at(0).setPos(-64, -64);
        powerBar.at(0).setPos(-64, -64);
        powerBar.at(1).setPos(-64, -64);
        // deny mousePressed signals
        canMove = false;
        // move the ball
        setPos(getPos().x + getVelocity().x*deltaTime, getPos().y + getVelocity().y*deltaTime);
        // update velocity
        if (getVelocity().x > 0.0001 || getVelocity().x < -0.0001 || getVelocity().y > 0.0001 || getVelocity().y < -0.0001)
        {
            if (velocity1D > 0)
            {
                velocity1D -= friction*deltaTime;
            }
            else
            {
                velocity1D = 0;
            }
            velocity.x = (velocity1D/launchedVelocity1D)*abs(launchedVelocity.x)*dirX;
            velocity.y = (velocity1D/launchedVelocity1D)*abs(launchedVelocity.y)*dirY;


        }
        // when velocity = 0, allow mousePressed signals
        else
        {
            setVelocity(0,0);
            int mouseX = 0;
	        int mouseY = 0;
            SDL_GetMouseState(&mouseX, &mouseY);
            setInitialMousePos(mouseX, mouseY);
            canMove = true;
        }
        // when ball hits map edge, bounce it back
        if (getPos().x + getCurrentFrame().w > 640/(2 - index))
        {
            setVelocity(-abs(getVelocity().x), getVelocity().y);
            dirX = -1;
        }
        else if (getPos().x < 0 + (index*320))
        {
            setVelocity(abs(getVelocity().x), getVelocity().y);
            dirX = 1;
        }
        else if (getPos().y + getCurrentFrame().h > 480)
        {
            setVelocity(getVelocity().x, -abs(getVelocity().y));
            dirY = -1;
        }
        else if (getPos().y < 0)
        {
            setVelocity(getVelocity().x, abs(getVelocity().y));
            dirY = 1;
        }
        // when ball hits a tile, bounce it back
        for (Tile& t : tiles)
	    { 
		    float newX = getPos().x + getVelocity().x*deltaTime;
            float newY = getPos().y;
            if (newX + 16 > t.getPos().x && newX < t.getPos().x + t.getCurrentFrame().w && newY + 16 > t.getPos().y && newY < t.getPos().y + t.getCurrentFrame().h - 3)
            {
                setVelocity(getVelocity().x*-1, getVelocity().y);
                dirX *= -1;
            }

            newX = getPos().x;
            newY = getPos().y + getVelocity().y*deltaTime;
            if (newX + 16 > t.getPos().x && newX < t.getPos().x + t.getCurrentFrame().w && newY + 16 > t.getPos().y && newY < t.getPos().y + t.getCurrentFrame().h - 3)
            {
                setVelocity(getVelocity().x, getVelocity().y*-1);
                dirY *= -1;
            }
	    }
    }
    
}
