#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "RenderWindow.h"
#include "Entity.h"
#include "Ball.h"	
#include "Tile.h"
#include "Hole.h"
#include "Constants.h"

using namespace std;

vector<Tile> loadTiles(int level);          // setup tiles for each level
void loadLevel(int level);                  // init balls and holes' pos, velo and call loadTiles to setup tiles
string getStrokeText();                     // get stroke text to display
string getLevelText(int side);              // get level text to display
void update();                              // update objects by dTime
void graphics();                            // display background, objects and texts
void titleScreen();                         // manage title screen
void game();                                // game loop

//initialize window and renderer
RenderWindow window("Mini Golf", WINDOW_WIDTH, WINDOW_HEIGHT);

//text colors
SDL_Color white = { 255, 255, 255 };
SDL_Color black = { 0, 0, 0 };

//initialize balls
Ball balls[2] = {
    Ball(Vector2f(0, 0), window.getTextureFromID("ballTexture"), window.getTextureFromID("pointTexture"), window.getTextureFromID("powerMeterTexture_FG"), window.getTextureFromID("powerMeterTexture_BG"), 0),
    Ball(Vector2f(0, 0), window.getTextureFromID("ballTexture"), window.getTextureFromID("pointTexture"), window.getTextureFromID("powerMeterTexture_FG"), window.getTextureFromID("powerMeterTexture_BG"), 1)
};
//initialize holes
vector<Hole> holes = {
    Hole(Vector2f(0, 0), window.getTextureFromID("holeTexture")),
    Hole(Vector2f(0, 0), window.getTextureFromID("holeTexture"))
};

int state = 0;                                      // 0 = title screen, 1 = game, 2 = end screen
int level = 0;                                      // from 0 to MAX_LEVEL-1
std::vector<Tile> tiles = loadTiles(level);         // contain tiles for each level

bool gameRunning = true;                            // stop and close the game if gameRunning==false
SDL_Event event;                                    // handle mouseEvents
bool mouseDown = false;
bool mousePressed = false;

bool swingPlayed = false;                           //used in title screen to play intro swing sound once only

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;                               // dTime is used for update function

int main(int argc, char* args[])
{
	loadLevel(level);
	while (gameRunning)
	{
		game();
	}
    window.closeFonts();
	window.cleanUp();
	SDL_Quit();
	TTF_Quit();
	return 0;
}
void game()
{
    if (state == 0)
    {
        titleScreen();
    }
    else
    {
        update();
        graphics();
    }
}
vector<Tile> loadTiles(int level)
{
    vector<Tile> levelTiles = {};
    switch(level)
    {
        case 0:
            levelTiles.push_back(Tile(Vector2f(BIG_W_H*3, BIG_W_H*3), window.getTextureFromID("tileDarkTexture64")));
            levelTiles.push_back(Tile(Vector2f(BIG_W_H*4, BIG_W_H*3), window.getTextureFromID("tileDarkTexture64")));
            levelTiles.push_back(Tile(Vector2f(BIG_W_H*0, BIG_W_H*3), window.getTextureFromID("tileDarkTexture64")));
            levelTiles.push_back(Tile(Vector2f(BIG_W_H*1, BIG_W_H*3), window.getTextureFromID("tileDarkTexture64")));

            levelTiles.push_back(Tile(Vector2f(BIG_W_H*3 + MAP_WIDTH, BIG_W_H*3), window.getTextureFromID("tileLightTexture64")));
            levelTiles.push_back(Tile(Vector2f(BIG_W_H*4 + MAP_WIDTH, BIG_W_H*3), window.getTextureFromID("tileLightTexture64")));
            levelTiles.push_back(Tile(Vector2f(BIG_W_H*0 + MAP_WIDTH, BIG_W_H*3), window.getTextureFromID("tileLightTexture64")));
            levelTiles.push_back(Tile(Vector2f(BIG_W_H*1 + MAP_WIDTH, BIG_W_H*3), window.getTextureFromID("tileLightTexture64")));
        break;
        case 1:
            levelTiles.push_back(Tile(Vector2f(BIG_W_H*2, BIG_W_H*3), window.getTextureFromID("tileDarkTexture64")));

            levelTiles.push_back(Tile(Vector2f(BIG_W_H*4 + MAP_WIDTH, BIG_W_H*3), window.getTextureFromID("tileLightTexture64")));
        break;
        case 2:
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*1 + MAP_WIDTH + 16, SMALL_W_H*5), window.getTextureFromID("tileLightTexture32")));
        break;
        case 3:
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*4, SMALL_W_H*7), window.getTextureFromID("tileDarkTexture64")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*3, SMALL_W_H*5), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*6, SMALL_W_H*3), window.getTextureFromID("tileDarkTexture32")));

            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*4 + MAP_WIDTH, SMALL_W_H*2), window.getTextureFromID("tileLightTexture64")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*3 + MAP_WIDTH, SMALL_W_H*6), window.getTextureFromID("tileLightTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*6 + MAP_WIDTH, SMALL_W_H*9), window.getTextureFromID("tileLightTexture32")));
        break;
        case 4:
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*3, SMALL_W_H*1), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*1, SMALL_W_H*3), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*5, SMALL_W_H*3), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*3, SMALL_W_H*5), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*7, SMALL_W_H*5), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*7, SMALL_W_H*10), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*3, SMALL_W_H*10), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*5, SMALL_W_H*12), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*7, SMALL_W_H*10), window.getTextureFromID("tileDarkTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*8, SMALL_W_H*7), window.getTextureFromID("tileDarkTexture64")));

            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*2 + MAP_WIDTH, SMALL_W_H*2), window.getTextureFromID("tileLightTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*5 + MAP_WIDTH, SMALL_W_H*11), window.getTextureFromID("tileLightTexture32")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*3 + MAP_WIDTH, SMALL_W_H*1), window.getTextureFromID("tileLightTexture64")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*8 + MAP_WIDTH, SMALL_W_H*6), window.getTextureFromID("tileLightTexture64")));
            levelTiles.push_back(Tile(Vector2f(SMALL_W_H*3 + MAP_WIDTH, SMALL_W_H*11), window.getTextureFromID("tileLightTexture64")));
        break;
    }
    return levelTiles;
}
void loadLevel(int level)
{
    if (level > 4)
    {
        state = 2;
        return;
    }
    balls[0].setVelocity(0, 0);
    balls[1].setVelocity(0,0);
    balls[0].setScale(1, 1);
    balls[1].setScale(1, 1);
    balls[0].setWin(false);
    balls[1].setWin(false);

    tiles = loadTiles(level);

    switch (level)
    {
        case 0:
            balls[0].setPos(24 + SMALL_W_H*4, 24 + SMALL_W_H*11);
            balls[1].setPos(24 + SMALL_W_H*4 + MAP_WIDTH, 24 + SMALL_W_H*11);

            holes.at(0).setPos(24 + SMALL_W_H*4, 22 + SMALL_W_H*2);
            holes.at(1).setPos(24 + SMALL_W_H*4 + MAP_WIDTH, 22 + SMALL_W_H*2);
        break;
        case 1:
            balls[0].setPos(24 + SMALL_W_H*4, 24 + SMALL_W_H*11);
            balls[1].setPos(24 + SMALL_W_H*4 + MAP_WIDTH, 24 + SMALL_W_H*11);

            holes.at(0).setPos(24 + SMALL_W_H*4, 22 + SMALL_W_H*2);
            holes.at(1).setPos(24 + SMALL_W_H*4 + MAP_WIDTH, 22 + SMALL_W_H*2);
        break;
        case 2:
            balls[0].setPos(8 + SMALL_W_H*7, 8 + SMALL_W_H*10);
            balls[1].setPos(8 + SMALL_W_H*7 + MAP_WIDTH, 8 + SMALL_W_H*10);

            holes.at(0).setPos(8 + SMALL_W_H*2, 6 + SMALL_W_H*5);
            holes.at(1).setPos(8 + SMALL_W_H*4 + MAP_WIDTH, 6 + SMALL_W_H*3);
        break;
        case 3:
            balls[0].setPos(24 + SMALL_W_H*4, 24 + SMALL_W_H*5);
            balls[1].setPos(24 + SMALL_W_H*4 + MAP_WIDTH, 24 + SMALL_W_H*4);

            holes.at(0).setPos(24 + SMALL_W_H*4, 22 + SMALL_W_H*1);
            holes.at(1).setPos(24 + SMALL_W_H*4 + MAP_WIDTH, 22 + SMALL_W_H*11);
        break;
        case 4:
            balls[0].setPos(24 + SMALL_W_H*2, 24 + SMALL_W_H*12);
            balls[1].setPos(24 + SMALL_W_H*0 + MAP_WIDTH, 24 + SMALL_W_H*5);

            holes.at(0).setPos(24 + SMALL_W_H*1, 22 + SMALL_W_H*1);
            holes.at(1).setPos(24 + SMALL_W_H*0 + MAP_WIDTH, 22 + SMALL_W_H*7);
        break;
    }
}
string getStrokeText()
{
    stringstream ss;
    int biggestStroke = 0;
    if (balls[1].getStrokes() > balls[0].getStrokes())
    {
        biggestStroke = balls[1].getStrokes();
    }
    else
    {
        biggestStroke = balls[0].getStrokes();
    }
    ss << "STROKES: " << biggestStroke;
    string s = ss.str();
    return s;
}

string getLevelText(int side)
{
    stringstream ss;
    int tempLevel = (level + 1)*2 - 1;
    if (side == 1)
    {
        tempLevel++;
    }
    ss << "HOLE: " << tempLevel;
    string s = ss.str();
    return s;
}
void update()
{
    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();
    deltaTime = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );

    mousePressed = false;
    //Get our controls and events
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            gameRunning = false;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                gameRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = true;
                mousePressed = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = false;
            }
            break;
        }
    }

    if (state == 1)
    {
        for (Ball& b : balls)
        {
            b.update(deltaTime, mouseDown, mousePressed, tiles, holes, window.getSoundFromID("chargeSfx"), window.getSoundFromID("swingSfx"), window.getSoundFromID("holeSfx"));
        }
        if (balls[0].getScale().x < -1 && balls[1].getScale().x < -1)
         {
            level++;
            loadLevel(level);
        }
    }
}

void graphics()
{
    window.clear();
    window.render(0, 0, window.getTextureFromID("bgTexture"));
    for (Hole& h : holes)
    {
        window.render(h);
    }
    for (Ball& b : balls)
    {
        if (!b.isWin())
        {
            window.render(b.getPos().x, b.getPos().y + 4, window.getTextureFromID("ballShadowTexture"));
        }
        for (Entity& e : b.getPoints())
        {
            window.render(e);
        }
        window.render(b);
    }
    for (Tile& t : tiles)
    {
        window.render(t);
    }
    for (Ball& b : balls)
    {
        for (Entity& e : b.getPowerBar())
        {
            window.render(e);
        }
        window.render(b.getPowerBar().at(0).getPos().x, b.getPowerBar().at(0).getPos().y, window.getTextureFromID("powerMeterTexture_overlay"));
        
    }
    if (state != 2)
    {
        window.render(640/4 - 132/2, 480 - 32, window.getTextureFromID("levelTextBgTexture"));
        window.renderCenter(-160, 240 - 16 + 3, getLevelText(0), window.getFontFromID("font24"), black);
        window.renderCenter(-160, 240 - 16, getLevelText(0), window.getFontFromID("font24"), white);

        window.render(640/2 + 640/4 - 132/2, 480 - 32, window.getTextureFromID("levelTextBgTexture"));
        window.renderCenter(160, 240 - 16 + 3, getLevelText(1), window.getFontFromID("font24"), black);
        window.renderCenter(160, 240 - 16, getLevelText(1), window.getFontFromID("font24"), white);

        window.render(640/2 - 196/2, 0, window.getTextureFromID("uiBgTexture"));
        window.renderCenter(0, -240 + 16 + 3, getStrokeText(), window.getFontFromID("font24"), black);
        window.renderCenter(0, -240 + 16, getStrokeText(), window.getFontFromID("font24"), white);
    }
    else
    {
        window.render(0, 0, window.getTextureFromID("endscreenOverlayTexture"));
        window.renderCenter(0, 3 - 32, "YOU COMPLETED THE COURSE!", window.getFontFromID("font48"), black);
        window.renderCenter(0, -32, "YOU COMPLETED THE COURSE!", window.getFontFromID("font48"), white);
        window.renderCenter(0, 3 + 32, getStrokeText(), window.getFontFromID("font32"), black);
        window.renderCenter(0, 32, getStrokeText(), window.getFontFromID("font32"), white);
    }
    window.display();
}

void titleScreen()
{
    
    if (!swingPlayed)
    {
        Mix_PlayChannel(-1, window.getSoundFromID("swingSfx"), 0);
        swingPlayed = true;
    }
    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();
    deltaTime = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );

    //Get our controls and events
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            gameRunning = false;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                gameRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                Mix_PlayChannel(-1, window.getSoundFromID("holeSfx"), 0);
                state = 1;
            }
            break;
        }
    }
    window.clear();
    window.render(0, 0, window.getTextureFromID("bgTexture"));
    window.renderLogo();
    window.render(0, 0, window.getTextureFromID("click2start"));
    window.renderCenter(0, 240 - 48 + 3 - 16*5, "LEFT CLICK TO START", window.getFontFromID("font32"), black);
    window.renderCenter(0, 240 - 48 - 16*5, "LEFT CLICK TO START", window.getFontFromID("font32"), white);
    window.display();
}

