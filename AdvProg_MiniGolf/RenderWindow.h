#pragma once
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_image/SDL_image.h>

#include <map>
#include "Entity.h"

class RenderWindow 
{
public:
	RenderWindow(const char* title, int w, int h);
    ~RenderWindow();
	SDL_Texture* loadTexture(const char* filePath);
    void loadAllTextures();
    SDL_Texture* getTextureFromID(const char* id);
    void loadAllFonts();
    TTF_Font* getFontFromID(const char* id);
    void loadAllSounds();
    Mix_Chunk* getSoundFromID(const char* id);
    void closeFonts();
	void cleanUp();
	void clear();
	void render(Entity& entity);
	void render(int x, int y, SDL_Texture* tex);
	void render(float x, float y, string text, TTF_Font* font, SDL_Color textColor);
    void renderLogo();
	void renderCenter(float x, float y, string text, TTF_Font* font, SDL_Color textColor);
	void display();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
    std::map<const char*, SDL_Texture*> textures;
    std::map<const char*, TTF_Font*> fonts;
    std::map<const char*, Mix_Chunk*> sounds;
};
