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
	int offset = (y * width + x) * step;
	pixel[offset] = color[0];
	pixel[offset + 1] = color[1];
	pixel[offset + 2] = color[2];
}

void Screen::disp(std::string window_name)
{
	cv::imshow(window_name, frame);
}
