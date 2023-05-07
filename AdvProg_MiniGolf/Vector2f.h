//Vector2f class provides a (float, float) data structure for position, scale, velocity, ...

#ifndef Vector2f_hpp
#define Vector2f_hpp

#include <iostream>

using namespace std;

class Vector2f{
public:
    Vector2f() : x(0.0f), y(0.0f){}
    Vector2f(float _x, float _y) : x(_x), y(_y){}
    
    void print(){
        cout << x << ", " << y << endl;
    }
    
    float x, y;
    
};

#endif /* Vector2f_hpp */
