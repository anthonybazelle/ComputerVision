#include "OCR.h"



OCR::OCR()
{
}


OCR::~OCR()
{
}

void OCR::GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return;

	do {
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
}

bool OCR::stringFromFile(const std::string& filename, std::string& str)
{
	std::ifstream file(filename.c_str());

	if (!file)
	{
		std::cerr << "Error: cannot open file \"" << filename << "\"." << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line))
		str += line;

	return true;
}

std::vector<cv::Rect> OCR::detectLetters(cv::Mat img)
{
	std::vector<cv::Rect> boundRect;
	cv::Mat img_gray, img_sobel, img_threshold, element;
	cvtColor(img, img_gray, CV_BGR2GRAY);
	cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
	cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3));
	cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element); //Does the trick
	std::vector< std::vector< cv::Point> > contours;
	cv::findContours(img_threshold, contours, 0, 1);
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	for (int i = 0; i < contours.size(); i++)
		if (contours[i].size()>100)
		{
			cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
			cv::Rect appRect(boundingRect(cv::Mat(contours_poly[i])));
			if (appRect.width>appRect.height)
				boundRect.push_back(appRect);
		}
		return boundRect;
}

void OCR::launchDetecting(std::vector<std::string> filesName)
{

	/*cv::Mat* dst = new cv::Mat();
	cv::Mat src = cv::imread(filesName.at(1), cv::WINDOW_NORMAL);
	cv::imshow("Image 1 : ", src);*/

	int u = 0;
	matReshape.create(filesName.size(), 8 * 8, CV_32FC1);
	for (std::vector<std::string>::iterator it = filesName.begin(); it != filesName.end(); ++it) // J'ai mis -1 car un fichier Thumbs.db se cache ...
	{
		cv::Mat dst;
		cv::Mat resized;
		resized.create(8, 8, CV_32FC1);
		cv::Mat src = cv::imread(*it, 0);
		cv::threshold(src, dst, 130, 255, cv::THRESH_BINARY);
		cv::resize(dst, resized, cv::Size(8, 8));
		matReshape.push_back<float>(resized.reshape(1, 1));
		cv::imshow("Resized", matReshape);
		
		++u;

		resized.release();
		dst.release();
		src.release();
	}

	// A remplir !!!!!!!
	labels(&cv::Range(0, u));


	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::SIGMOID;
	params.gamma = 3;

	CvSVM svm;
	svm.train_auto(matReshape, labels, cv::Mat(), cv::Mat(), params);
	svm.save("trainingOCR.txt");
	std::cout << "Training finish and saved" << std::endl;
	cv::imshow("Resized", matReshape);
	cv::waitKey(0);
}