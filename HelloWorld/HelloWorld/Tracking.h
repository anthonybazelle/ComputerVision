#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\video.hpp>
#include <vector>

#include <iostream>

class Tracking
{
public:
	Tracking(void);
	~Tracking(void);
	int fluxVideo();
	void draw();
	void detectPoints(cv::Mat& img);
	void trackPoints();
	void CallBackFunc(int event, int x, int y, int flags, void* userdata);
	static Tracking* singleton;

private:
	cv::Mat nextInput;
	int maxCorners;
	int maxTrackbar;
	std::vector<cv::Point2f> prevPoints;
	cv::Mat prevInput;
	std::vector<cv::Point2f> nextPoints;
	cv::Mat imgDetect;
	std::vector<float> err;
	std::vector<uchar> status;

	cv::Rect roi;
	cv::Point start;
};

