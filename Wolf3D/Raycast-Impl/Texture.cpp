#include "Texture.h"

Texture::Texture(std::string filepath)
{
    tex = cv::imread(filepath);
    width = tex.cols;
    height = tex.rows;
    data = static_cast<uchar*>(tex.data);
    step = 3;
}

cv::Scalar Texture::getColor(float sampleX, float sampleY)
{
    int width = tex.rows;
    int height = tex.cols;

    int px = (float)width * sampleX;
    int py = (float)height * sampleY;

    int offset = (py * width + px) * step;

    uchar B = data[offset + 0];
    uchar G = data[offset + 1];
    uchar R = data[offset + 2];

    cv::Scalar pixel(B,G,R);

    return pixel;
}
