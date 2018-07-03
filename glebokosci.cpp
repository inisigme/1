#include "stdafx.h"
#include "glebokosci.h"


using namespace cv;
using namespace std;

void wczyanieObrazuZGlebokosci(std::string fileName, cv::Mat & glebokosci, cv::Mat & intensity) {
	std::string input;
	int intFromFile = 0;

	std::ifstream file;
	file.open(fileName, std::ios::binary);

	unsigned int cols = 9999999;
	unsigned int rows = 9999999;


	if (file.is_open())
	{

		std::cout << "File open.\n";

		std::getline(file, input, '\n');
		std::stringstream ss(input);
		ss >> rows;

		std::getline(file, input, '\n');
		ss.str(input);
		ss >> cols;

		std::cout << cols << "," << rows << std::endl;

		for (int i = 0; i < 9; i++) {
			std::getline(file, input, '\n');
		}

		glebokosci = cv::Mat::zeros(rows, cols, CV_8UC1);
		intensity = cv::Mat::zeros(rows, cols, CV_8UC1);
	
		float x, y, z, inten;
		unsigned int licznik = 0;
		while (!file.eof()) {
			std::getline(file, input, ' ');
			x = atof(input.c_str());
			std::getline(file, input, ' ');
			y = atof(input.c_str());
			std::getline(file, input, ' ');
			z = atof(input.c_str());
			std::getline(file, input, '\n');
			inten = atof(input.c_str());
			glebokosci.at<unsigned char>(licznik / cols, licznik % cols) = (unsigned char)(sin(sqrt(x*x + y*y + z*z) / 6.0f)    * 255.0f);
			if (glebokosci.at<unsigned char>(licznik / cols, licznik % cols) < 20.0) glebokosci.at<unsigned char>(licznik / cols, licznik % cols) = 0;
			intensity.at<unsigned char>(licznik / cols, licznik % cols) = (unsigned char)(inten * 255.0f);
			licznik++;
		}
		std::cout << licznik << std::endl;
		file.close();
		cv::imwrite("panoramaDEP.png", glebokosci);
		cv::imwrite("panoramaINT.png", intensity);
	}
	else
		std::cout << "Error opening file.\n";
}






void wczyanieObrazuZGlebokosciV2(std::string fileName, cv::Mat & glebokosci, cv::Mat & intensity) {
	glebokosci = cv::imread("panoramaDEP.png", 0);
	intensity = cv::imread("panoramaINT.png", 0);
}










/*
void wczytanieIObrobkaGlebokosci() {
	
	//cv::Mat glebokosci = wczyanieObrazuZGlebokosci("download/350_INT.ptx");
	cv::Mat imgRGB = imread("download/PANO_JPG/350.jpg", IMREAD_GRAYSCALE);

	int m = 1;
	cv::Mat img1 = cv::imread("download/PANO_SKANER/350.pano360rf.png", IMREAD_GRAYSCALE);
	cv::resize(img1, img1, cv::Size(2048 / m, 1024 / m), 0, 0, 1);
	imshow("orginal z intensity", img1);
	cvWaitKey();

	rectangle(glebokosci, Point(2000, 2000), Point(1000 + 2048, 1000 + 1024), Scalar(255.0), 1);

	cv::Rect srcRect1(cv::Point(2000, 2000), cv::Size(2048, 1024));
	cv::Rect dstRect1(cv::Point(0, 0), cv::Size(2048, 1024));
	cv::Mat outMat = cv::Mat::zeros(1024, 2048, CV_8UC1);
	glebokosci(srcRect1).copyTo(outMat(dstRect1));

	cv::resize(glebokosci, glebokosci, cv::Size(2048 / m, 1024 / m), 0, 0);
	imshow("duzy wygenerowany", glebokosci);
	cvWaitKey();
	imshow("wynik", outMat);
	cvWaitKey();


	threshold(outMat, outMat, 1, 255, THRESH_BINARY_INV);
	imshow("wynik z Cannym", outMat);
	cvWaitKey();

	cv::Mat imgRGB2 = cv::Mat::zeros(1024, 2048, CV_8UC1);
	imgRGB(srcRect1).copyTo(imgRGB2(dstRect1));



	cvWaitKey();
	getchar();
}

*/