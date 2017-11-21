#include "Disparity.h"



Disparity::Disparity()
{
}


Disparity::~Disparity()
{
}

void Disparity::FindMatchings(cv::Mat img1, cv::Mat img2)
{
	cv::Mat imgDisparity;
	cv::StereoBM* stereoBM = new cv::StereoBM();
	stereoBM->init(0, 16 * 5, 21);
	stereoBM->operator()(img1, img2, imgDisparity);
	cv::imshow("Disparity window", imgDisparity);
	
	cv::waitKey(0);
}
