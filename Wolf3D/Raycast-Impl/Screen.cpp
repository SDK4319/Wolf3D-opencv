#include "Screen.h"

Screen::Screen(int width, int height) : width(width), height(height), frame(height, width, CV_8UC3), step(3)
{
	pixel = static_cast<uchar*>(frame.data);
}

void Screen::setPixel(int x, int y, cv::Scalar color)
{
	if (pixel == nullptr) {
		std::cout << "setPixel Error => pixel ptr is nullptr" << std::endl;
		exit(1);
	}
	if (flipX) {
		x = width - 1 - x;
	}
	if (flipY) {
		y = width - 1 - y;
	}
	int offset = (y * width + x) * step;
	pixel[offset] = color[0];
	pixel[offset + 1] = color[1];
	pixel[offset + 2] = color[2];
}

void Screen::disp(std::string window_name)
{
	cv::imshow(window_name, frame);
}


void Screen::disp(std::string window_name, int screenWidth, int screenHeight, int type) {
	cv::Mat temp;
	cv::resize(frame, temp, cv::Size(screenWidth, screenHeight),0,0,type);
	cv::imshow(window_name,temp);
}
