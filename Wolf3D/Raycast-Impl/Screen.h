#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>

class Screen
{
public:
	Screen(int width, int height);
	void setPixel(int x, int y, cv::Scalar color);
	void disp(std::string window_name);

	int width;
	int height;

private:
	cv::Mat frame;
	uchar* pixel = nullptr;
	int step;
};

