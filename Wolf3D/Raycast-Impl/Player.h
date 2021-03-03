#pragma once
#include<cmath>
class Player
{
public:
	Player() {}
	Player(float x, float y) : fPlayerX(x), fPlayerY(y) {}
	Player(float x, float y, float angle) : fPlayerA(angle) { Player(x, y); }
	
	float getX() { return fPlayerX; }
	float getY() { return fPlayerY; }
	float getA() { return fPlayerA; }

	void changeAngle(float diff) { fPlayerA += diff; }
	void forward(float speed) {
		fPlayerX += sinf(fPlayerA) * speed;
		fPlayerY += cosf(fPlayerA) * speed;
	}
	void backward(float speed) {
		fPlayerX -= sinf(fPlayerA) * speed;
		fPlayerY -= cosf(fPlayerA) * speed;
	}
	

private:
	float fPlayerX = 0.0f;
	float fPlayerY = 0.0f;
	float fPlayerA = 0.0f;
};

