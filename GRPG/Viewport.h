#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "game.h"

struct Coordinates {
	float x;
	float y;

	Coordinates(float x2, float y2){
		x = x2;
		y = y2;
	}
};

class Viewport {
private:

	Game* gamePtr;

	float x;
	float y;
	int width;
	int height;

public:
	Viewport(Game* g, float x2, float y2, int w, int h){
		gamePtr = g; x = x2; y = y2; width = w; height = h;
	}

	float getX(){ return x; }
	float getY(){ return y; }
	void setX(float x2){ x = x2; }
	void setY(float y2){ y = y2; }
	
	int getWidth() { return width; }
	int getHeight() { return height; }

	Coordinates getTopLeft(){ return Coordinates(x - width / 2, y - height / 2); }
	Coordinates getTopRight(){ return Coordinates(x + width / 2, y - height / 2); }
	Coordinates getBottomLeft(){ return Coordinates(x - width / 2, y + height / 2); }
	Coordinates getBottomRight(){ return Coordinates(x + width / 2, y + height / 2); }

	// Translate viewport coordinates into screen coordinates
	Coordinates translate(float x, float y){
		return Coordinates(x - getTopLeft().x, y - getTopLeft().y);
	}
	
};

#endif