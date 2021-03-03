#include<iostream>
#include<chrono>
#include<cmath>
#include<Windows.h>
#include"Screen.h"
#include"Player.h"

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.141592 / 6.0f;
float fDepth = 16.0f;


int main() {
	Screen screen(400, 300);
	Player p(2.0,2.0);

	std::string map = "";
	map += "################";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "#..............#";
	map += "################";

	auto prev_tp = std::chrono::system_clock::now();
	auto curr_tp = std::chrono::system_clock::now();

	while (1) {
		curr_tp = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = curr_tp - prev_tp;
		prev_tp = curr_tp;
		float fElapsedTime = elapsedTime.count();


		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			p.changeAngle(-1.0f * fElapsedTime);
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			p.changeAngle(1.0f * fElapsedTime);
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
			p.forward(5.0f * fElapsedTime);
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
			p.backward(5.0f * fElapsedTime);


		for (int x = 0; x < screen.width; x++) {

			float fRayAngle = (p.getA() - fFOV / 2.0f) + ((float)x / (float)screen.width) * fFOV;
			float fDistanceToWall = 0.0f;
			bool bHitWall = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall <= fDepth) {
				fDistanceToWall += 0.2f;
				int nTestX = (int)(p.getX() + fEyeX * fDistanceToWall);
				int nTestY = (int)(p.getY() + fEyeX * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else {
					if (map[nTestY * nMapWidth + nTestX] == '#') {
						bHitWall = true;
					}
				}
			}

			int nCeiling = (float)(screen.height / 2.0) - screen.height / ((float)fDistanceToWall);
			int nFloor = screen.height - nCeiling;

			cv::Scalar shade;
			//Close
			if (fDistanceToWall <= fDepth / 4.0f) shade = cv::Scalar(255, 255, 255);
			else if (fDistanceToWall <= fDepth / 3.0f) shade = cv::Scalar(204, 204, 204);
			else if (fDistanceToWall <= fDepth / 2.0f) shade = cv::Scalar(153, 153, 153);
			else if (fDistanceToWall <= fDepth) shade = cv::Scalar(102, 102, 102);
			else shade = cv::Scalar(51, 51, 51);
			//Far
			for (int y = 0; y < screen.height; y++) {
				if (y < nCeiling)
					screen.setPixel(x, y, cv::Scalar(0, 0, 0));
				else if (y > nCeiling && y <= nFloor)
					screen.setPixel(x, y, shade);
				else
					screen.setPixel(x, y, cv::Scalar(0, 0, 0));
			}
		}



		screen.disp("MainFrame");
		char command = cv::waitKey(1);
		if (command == 27)
			break;
	}


	return 0;
}