//
//  RenderWindow.cpp
//  MiniGolfGame
//
//  Created by Thinh Kieu on 23/04/2023.
//

#include "RenderWindow.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <iostream>
#include "Entity.h"
#include "Constants.h"

using namespace std;

//init function: create window and renderer
RenderWindow::RenderWindow(const char* title, int width, int height) : window(NULL), renderer(NULL){
    //initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        std::cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    if (!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
    if (TTF_Init() != 0)
        std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    
    if (window == NULL){
        cout<< "Window failed to initialize. Error: " << SDL_GetError() << endl;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    //load assets
    loadAllTextures();
    loadAllFonts();
    loadAllSounds();
}
//deconstructor
RenderWindow::~RenderWindow(){}
//load texture from filePath
SDL_Texture* RenderWindow::loadTexture(const char* filePath){
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);
    
    if (texture == NULL)
        cout << "Failed to load texture from " << filePath << ". Error: " << SDL_GetError() << endl;
    return texture;
}
//close fonts
void RenderWindow::closeFonts(){
    for (auto& f : fonts){
        TTF_CloseFont(f.second);
    }
}
//destroy window
void RenderWindow::cleanUp(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
//render clear
void RenderWindow::clear(){
    SDL_RenderClear(renderer);
}
//render object using entity
void RenderWindow::render(Entity &_entity){
    SDL_Rect srcRect;
    srcRect.x = _entity.getCurrentFrame().x;
    srcRect.y = _entity.getCurrentFrame().y;
    srcRect.w = _entity.getCurrentFrame().w;
    srcRect.h = _entity.getCurrentFrame().h;
    
    SDL_Rect destRect;
    destRect.x = _entity.getPos().x + (_entity.getCurrentFrame().w - _entity.getCurrentFrame().w * _entity.getScale().x) / 2;
    destRect.y = _entity.getPos().y + (_entity.getCurrentFrame().h - _entity.getCurrentFrame().h * _entity.getScale().y) / 2;
    destRect.w = _entity.getCurrentFrame().w * _entity.getScale().x;
    destRect.h = _entity.getCurrentFrame().h * _entity.getScale().y;
    
    SDL_RenderCopyEx(renderer, _entity.getTex(), &srcRect, &destRect, _entity.getAngle(), 0, SDL_FLIP_NONE);
}
//render object using texture
void RenderWindow::render(int xpos, int ypos, SDL_Texture* _tex){
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    SDL_QueryTexture(_tex, NULL, NULL, &srcRect.w, &srcRect.h);
    
    SDL_Rect destRect;
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
    
    SDL_RenderCopy(renderer, _tex, &srcRect, &destRect);
}
//render text
void RenderWindow::render(float xpos, float ypos, string text, TTF_Font* font, SDL_Color textColor){
    SDL_Surface* messageSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = messageSurface->w;
    srcRect.h = messageSurface->h;
    
    SDL_Rect destRect;
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
    
    SDL_RenderCopy(renderer, messageTexture, &srcRect, &destRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}
//render logo in title screen
void RenderWindow::renderLogo(){
    SDL_Texture* logoTexture = getTextureFromID("logoTexture");
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    SDL_QueryTexture(logoTexture, NULL, NULL, &srcRect.w, &srcRect.h);
    
    SDL_Rect destRect;
    destRect.x = WINDOW_WIDTH/2 - (srcRect.w/2)/2;
    destRect.y = WINDOW_HEIGHT/5 + 4*SDL_sin(SDL_GetTicks()*(3.14/1500));
    destRect.w = srcRect.w/2;
    destRect.h = srcRect.h/2;
    
    SDL_RenderCopy(renderer, logoTexture, &srcRect, &destRect);
}
//render text with position is text's center.
void RenderWindow::renderCenter(float xpos, float ypos, string text, TTF_Font* font, SDL_Color textColor){
    SDL_Surface* messageSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = messageSurface->w;
    srcRect.h = messageSurface->h;
    
    SDL_Rect destRect;
    destRect.x = WINDOW_WIDTH/2 - srcRect.w/2 + xpos;
    destRect.y = WINDOW_HEIGHT/2 - srcRect.h/2 + ypos;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
    
    SDL_RenderCopy(renderer, messageTexture, &srcRect, &destRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}
//render present
void RenderWindow::display(){
    SDL_RenderPresent(renderer);
}
//load all needed textures
void RenderWindow::loadAllTextures(){
    textures.emplace("ballTexture", loadTexture("assets/gfx/ball.png"));
    textures.emplace("holeTexture", loadTexture("assets/gfx/hole.png"));
    textures.emplace("pointTexture", loadTexture("assets/gfx/point.png"));
    textures.emplace("tileDarkTexture32", loadTexture("assets/gfx/tile32_dark.png"));
    textures.emplace("tileDarkTexture64", loadTexture("assets/gfx/tile64_dark.png"));
    textures.emplace("tileLightTexture32", loadTexture("assets/gfx/tile32_light.png"));
    textures.emplace("tileLightTexture64", loadTexture("assets/gfx/tile64_light.png"));
    textures.emplace("ballShadowTexture",loadTexture("assets/gfx/ball_shadow.png"));
    textures.emplace("bgTexture", loadTexture("assets/gfx/bg.png"));
    textures.emplace("uiBgTexture", loadTexture("assets/gfx/UI_bg.png"));
    textures.emplace("levelTextBgTexture", loadTexture("assets/gfx/levelText_bg.png"));
    textures.emplace("powerMeterTexture_FG", loadTexture("assets/gfx/powermeter_fg.png"));
    textures.emplace("powerMeterTexture_BG", loadTexture("assets/gfx/powermeter_bg.png"));
    textures.emplace("powerMeterTexture_overlay", loadTexture("assets/gfx/powermeter_overlay.png"));
    textures.emplace("logoTexture", loadTexture("assets/gfx/logo.png"));
    textures.emplace("click2start", loadTexture("assets/gfx/click2start.png"));
    textures.emplace("endscreenOverlayTexture", loadTexture("assets/gfx/end.png"));
    textures.emplace("splashBgTexture", loadTexture("assets/gfx/splashbg.png"));
}
//get texture from id
SDL_Texture* RenderWindow::getTextureFromID(const char* id){
    return textures[id];
}
//load all needed fonts
void RenderWindow::loadAllFonts(){
    fonts.emplace("font24", TTF_OpenFont("assets/font/font.ttf", 24));
    fonts.emplace("font32", TTF_OpenFont("assets/font/font.ttf", 32));
    fonts.emplace("font48", TTF_OpenFont("assets/font/font.ttf", 48));
}
//get font from id
TTF_Font* RenderWindow::getFontFromID(const char* id){
    return fonts[id];
}
//load all needed sounds
void RenderWindow::loadAllSounds(){
    sounds.emplace("chargeSfx", Mix_LoadWAV("assets/sfx/charge.mp3"));
    sounds.emplace("swingSfx", Mix_LoadWAV("assets/sfx/swing.mp3"));
    sounds.emplace("holeSfx", Mix_LoadWAV("assets/sfx/hole.mp3"));
}
//get sound from id
Mix_Chunk* RenderWindow::getSoundFromID(const char* id){
    return sounds[id];
}
