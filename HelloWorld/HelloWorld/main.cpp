//#include "opencv2/opencv.hpp"
#include <string>
#include <iostream>
#include <cstdlib>
#include "Tracking.h"
#include "OCR.h"
#include "Disparity.h"

/*
void draw()
{
	Mat img = nextInput.clone();
}*/

int main(int, char**)
{
	/*
	Tracking* pTracking = new Tracking();
    int res = pTracking->fluxVideo();

	if(res == -1)
	{
		std::cout << "Erreur lors de l'ouverture de la webcam" << std::endl;
	}
	*/
	

	
	OCR* pOCR = new OCR();

	std::vector<std::string> out;
	pOCR->GetFilesInDirectory(out, "C:/Users/Anthony/Documents/ESGI/5A DJV/Vision par ordinateur/ressources/Training");
	pOCR->launchDetecting(out);
	

	/*
	Disparity* pDisparity = new Disparity();
	cv::Mat img1 = cv::imread("C:/Users/Anthony/Documents/ESGI/5A DJV/Vision par ordinateur/ressources/Disparity/img1.jpg", 0);
	cv::Mat img2 = cv::imread("C:/Users/Anthony/Documents/ESGI/5A DJV/Vision par ordinateur/ressources/Disparity/img2.jpg", 0);

	pDisparity->FindMatchings(img1, img2);
	*/
	// C:\Users\Anthony\Documents\ESGI\5A DJV\Vision par ordinateur\ressources\Training

    // the camera will be deinitialized automatically in VideoCapture destructor
	cv::waitKey(0);
    return 0;
}