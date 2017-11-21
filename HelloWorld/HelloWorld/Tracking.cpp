#include "Tracking.h"

Tracking* Tracking::singleton;

Tracking::Tracking(void)
{
	maxCorners = 23;
	maxTrackbar = 100;
	Tracking::singleton = this;
}

Tracking::~Tracking(void)
{
}

static void StaticCallBackFunction(int event, int x, int y, int flags, void* userdata)
{
	Tracking::singleton->CallBackFunc(event, x, y, flags, userdata);
}

void Tracking::CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		start = cv::Point(x, y);
		roi = cv::Rect();
		prevPoints.clear();
		nextPoints.clear();
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		if (start.x >= 0) {
			cv::Point end(x, y);
			roi = cv::Rect(start, end);
		}
	}
	else if (event == cv::EVENT_LBUTTONUP) {
		cv::Point end(x, y);
		roi = cv::Rect(start, end);
		start = cv::Point(-1, -1);
	}
}



std::vector<cv::Point2f> purgePoints(std::vector<cv::Point2f>& points, std::vector<uchar>& status)
{
	std::vector<cv::Point2f> result;

	for(int i = 0; i < points.size(); ++i)
	{
		if(status[i]>0)
		{
			result.push_back(points[i]);
		}
	}

	return result;
}

void Tracking::trackPoints()
{
	if(!prevInput.empty())
	{
		prevPoints = nextPoints;

		if(prevPoints.size() < 10)
		{	
			//cv::Mat img = prevInput.clone();
			detectPoints(prevInput);
			if (prevPoints.size() < 10)
			{
				std::cout << "No points ! " << std::endl;
				return;
			}
		}
		cv::calcOpticalFlowPyrLK(prevInput, nextInput, prevPoints, nextPoints, status, err);
		//prevPoints = purgePoints(prevPoints, status);
		//nextPoints = purgePoints(nextPoints, status);
	}
	
	prevInput = nextInput.clone();
}

void updateROI() {
	//si roi est vide ou si une selection est en cours, on quitte direct
	//sinon, on calcule le rectangle englobant notre ensemble de points stockés
	// dans nextPoints, et on l’assigne a roi
}

void Tracking::detectPoints(cv::Mat& img)
{
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	cv::RNG rng(12345);
	cv::Mat copy;

	cv::Mat imgCopy;
	cv::cvtColor(img, copy, CV_BGR2GRAY);
	//img.copyTo(imgCopy, )
	cv::goodFeaturesToTrack(copy, prevPoints, 150, qualityLevel, minDistance, cv::Mat(), 3, useHarrisDetector, k);

	std::cout << "Number of corner detected : " << prevPoints.size() << std::endl;
}

void Tracking::draw()
{
	cv::Mat img = nextInput.clone();
	trackPoints();
	
	for( int i = 0; i < prevPoints.size(); i++ )
	{ 
		cv::line(img,prevPoints[i],nextPoints[i],cv::Scalar(255,0,0),0.5);
	}
	
	int r = 4;
	for (int i = 0; i < nextPoints.size(); i++)
	{
		cv::circle(img, nextPoints[i], r, cv::Scalar(0, 255, 0), -1, 8, 0);
	}


	cv::rectangle(img, roi, cv::Scalar(0, 0, 255), 1, 8, 0);
	cv::imshow("Tracking", img);
	cv::setMouseCallback("Tracking", StaticCallBackFunction, NULL);
}

int Tracking::fluxVideo()
{
	cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    cv::Mat edges;
    cv::namedWindow("Tracking",1);
    for(;;)
    {
        cv::Mat frame;
        cap >> nextInput; // get a new frame from camera
		draw();
		
        if(cv::waitKey(10) >= 0) break;
    }

	return -1;
}