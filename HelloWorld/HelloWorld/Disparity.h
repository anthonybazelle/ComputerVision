#pragma once


#include <opencv2/core/core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\calib3d\calib3d.hpp>


class Disparity
{
public:
	Disparity();
	~Disparity();
	void FindMatchings(cv::Mat img1, cv::Mat img2);
};

