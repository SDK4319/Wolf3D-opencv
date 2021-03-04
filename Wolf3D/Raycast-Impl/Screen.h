#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>

class Screen
{
public:
	Screen(int width, int height);
	void setPixel(int x, int y, cv::Scalar color);
	void disp(std::string window_name);
	void disp(std::string window_name, int screenWidth, int screenHeight,int type = cv::INTER_NEAREST);
	void setFlip(bool flipX, bool flipY) {
		this->flipX = flipX;
		this->flipY = flipY;
	}
	int width;
	int height;

private:
	bool flipX, flipY;
	cv::Mat frame;
	uchar* pixel = nullptr;
	int step;
};

