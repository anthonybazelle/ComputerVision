#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

#include <opencv2/core/core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\video.hpp>
#include <opencv2\ml\ml.hpp>


class OCR
{
private:
	cv::Mat matReshape;
	cv::Mat labels;

public:
	OCR();
	~OCR();
	void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory);
	bool stringFromFile(const std::string& filename, std::string& str);
	void launchDetecting(std::vector<std::string> filesName);
	std::vector<cv::Rect> detectLetters(cv::Mat img);
};

