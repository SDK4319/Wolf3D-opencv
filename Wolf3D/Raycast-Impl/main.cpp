#include<iostream>
#include<chrono>
#include<cmath>
#include<Windows.h>
#include"Screen.h"
#include"Player.h"
#include"Texture.h"

int nMapHeight = 32;
int nMapWidth = 32;

float fFOV = 3.141592 / 6.0f;
float fDepth = 16.0f;


int main() {
	Screen screen(150, 100);
	Screen UImap(32, 32);
	UImap.setFlip(true, false);
	Player p(1.0,1.0);
	Texture texture("./texture/wall.bmp");


	std::string map = "";
	map += "################################";
	map += "#..............................#";
	map += "#.......#......................#";
	map += "#.......#......................#";
	map += "#.......#......................#";
	map += "#########......................#";
	map += "#..............................#";
	map += "#..............................#";
	map += "#..............................#";
	map += "#..............................#";
	map += "#...###################........#";
	map += "#.....................#........#";
	map += "#.....................#........#";
	map += "#.....................#........#";
	map += "#.......#########.....#........#";
	map += "#.......#.......#.....#........#";
	map += "#.......#.......#.....#........#";
	map += "#.......#.......#.....#........#";
	map += "#.......#.......#.....#........#";
	map += "#.......#########.....#........#";
	map += "#.....................#........#";
	map += "#.....................#........#";
	map += "#.....................#........#";
	map += "##############........#........#";
	map += "#.....................#........#";
	map += "#.....................#........#";
	map += "#....#.........................#";
	map += "#....#.........##########......#";
	map += "#....#.........................#";
	map += "#....#.........................#";
	map += "#..............................#";
	map += "################################";

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
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			p.forward(5.0f * fElapsedTime);
			if (map[(int)p.getY() * nMapWidth + (int)p.getX()] == '#') {
				p.backward(5.0f * fElapsedTime);
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
			p.backward(5.0f * fElapsedTime);
			if (map[(int)p.getY() * nMapWidth + (int)p.getX()] == '#') {
				p.forward(5.0f * fElapsedTime);
			}
		}



		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {
				if(map[y * nMapWidth + x] == '#')
					UImap.setPixel(x, y, cv::Scalar(255, 255, 255));
				else
					UImap.setPixel(x, y, cv::Scalar(0, 0, 0));
			}
		}


		for (int x = 0; x < screen.width; x++) {

			float fRayAngle = (p.getA() - fFOV / 2.0f) + ((float)x / (float)screen.width) * fFOV;
			float fDistanceToWall = 0.0f;
			bool bHitWall = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			float fSampleX = 0.0f;

			while (!bHitWall && fDistanceToWall <= fDepth) {
				fDistanceToWall += 0.2f;
				int nTestX = (int)(p.getX() + fEyeX * fDistanceToWall);
				int nTestY = (int)(p.getY() + fEyeY * fDistanceToWall);


				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else {
					if (map[nTestY * nMapWidth + nTestX] == '#') {
						bHitWall = true;

						float fBlockMidX = (float)nTestX + 0.5f;
						float fBlockMidY = (float)nTestY + 0.5f;

						float fTestPointX = p.getX() + fEyeX * fDistanceToWall;
						float fTestPointY = p.getY() + fEyeY * fDistanceToWall;

						float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX));

						if (fTestAngle >= -3.14159f * 0.25f && fTestAngle < 3.14159f * 0.25f)
							fSampleX = fTestPointY - (float)nTestY;
						if (fTestAngle >= 3.14159f * 0.25f && fTestAngle < 3.14159f * 0.75f)
							fSampleX = fTestPointX - (float)nTestX;
						if (fTestAngle < -3.14159f * 0.25f && fTestAngle >= -3.14159f * 0.75f)
							fSampleX = fTestPointX - (float)nTestX;
						if (fTestAngle >= 3.14159f * 0.75f || fTestAngle < -3.14159f * 0.75f)
							fSampleX = fTestPointY - (float)nTestY;

					}
					else {
						UImap.setPixel(nTestX, nTestY, cv::Scalar(0, 255, 255));
					}
				}
			}

			int nCeiling = (float)(screen.height / 2.0) - screen.height / ((float)fDistanceToWall);
			int nFloor = screen.height - nCeiling;

			
			for (int y = 0; y < screen.height; y++) {
				if (y < nCeiling)
					screen.setPixel(x, y, cv::Scalar(0, 0, 0));
				else if (y > nCeiling && y <= nFloor) {
					float fSampleY = ((float)y - (float)nCeiling) / ((float)nFloor - (float)nCeiling);
					cv::Scalar shade = texture.getColor(fSampleX, fSampleY);
					screen.setPixel(x, y, shade);
				}
				else { //Floor
					if(y > screen.height / 2)
						screen.setPixel(x, y, cv::Scalar(127,0,0));
					else
						screen.setPixel(x, y, cv::Scalar(0, 0, 0));
				}
			}
		}
		UImap.setPixel((int)p.getX(), (int)p.getY(), cv::Scalar(0, 0, 255));

		UImap.disp("map", 256, 256);
		screen.disp("MainFrame", 300, 200);
		char command = cv::waitKey(1);
		if (command == 27)
			break;
	}


	return 0;
}