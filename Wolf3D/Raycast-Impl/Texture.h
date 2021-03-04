#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>

class Texture
{
public:
	Texture(std::string filepath);
	const cv::Mat getTexture() { return tex; }
	cv::Scalar getColor(float sampleX, float sampleY);
public:
	cv::Mat tex;
	uchar* data;
	int width;
	int height;
	int step;
};