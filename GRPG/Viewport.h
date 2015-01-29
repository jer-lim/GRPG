#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "game.h"

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
	~Viewport(){}

	float getX(){ return x; }
	float getY(){ return y; }
	void setX(float x2){ x = x2; }
	void setY(float y2){ y = y2; }
	
	int getWidth() { return width; }
	int getHeight() { return height; }

	VECTOR2 getTopLeft(){ return VECTOR2(x - width / 2, y - height / 2); }
	VECTOR2 getTopRight(){ return VECTOR2(x + width / 2, y - height / 2); }
	VECTOR2 getBottomLeft(){ return VECTOR2(x - width / 2, y + height / 2); }
	VECTOR2 getBottomRight(){ return VECTOR2(x + width / 2, y + height / 2); }

	// Translate viewport coordinates into screen coordinates
	VECTOR2 translate(float x, float y){
		return VECTOR2(x - getTopLeft().x, y - getTopLeft().y);
	}

	VECTOR2 reverseTranslate(float x, float y){
		return VECTOR2(x + getTopLeft().x, y + getTopLeft().y);
	}
	
};

#endif