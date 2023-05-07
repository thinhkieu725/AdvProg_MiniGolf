//Constants.hpp defines commonly used constants in the programm.

#ifndef Constants_hpp
#define Constants_hpp

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const int MAP_WIDTH = 320;

const int BALL_W_H = 16;
const int SMALL_W_H = 32;
const int BIG_W_H = 64;

const int MAX_LEVEL = 5;           // Number of levels

const int VELO_PER_PULLDIS = -150; // Ball's mouse pulling distance to velocity ratio

const double PI = 3.1415;
const double RAD_TO_DEG_RATIO = 180/PI;

#endif /* Constants_hpp */
