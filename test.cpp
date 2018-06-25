#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <vector>
#include <thread>


#include <opencv2/opencv.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;


#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2\features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\features2d\features2d.hpp>


#include <opencv2/calib3d/calib3d.hpp>
//#include <opencv2/nonfree/nonfree.hpp>


int main(int argc, char* argv[])
{
	try {
		cv::Mat img = cv::imread("download/PANO_JPG/350.jpg",1);
		cv::resize(img, img, cv::Size(1600, 900), 0, 0, 1);
	
		cv::Mat img2 = cv::imread("download/PANO_SKANER/350.pano360rf.png", 0);
		cv::resize(img2, img2, cv::Size(1600, 900), 0, 0, 1);

		cvtColor(img, img, CV_RGB2GRAY);

		//std::vector<cv::KeyPoint> keypointsA, keypointsB;
		//cv::Mat descriptorsA, descriptorsB;

		//int Threshl = 60;
		//int Octaves = 4; // (pyramid layer) from which the keypoint has been extracted
		//float PatternScales = 1.0f;

////		cv::BRISK  BRISKD(Threshl, Octaves, PatternScales);//initialize algoritm
//		auto b = cv::BRISK();//initialize algoritm
//		
//
//		cv::Ptr<cv::BRISK> ptrBrisk = cv::BRISK::create();
//		ptrBrisk->detect(img, keypointsA);
//
//	//	b.create("Feature2D.BRISK");
//		b.create(Threshl, Octaves, PatternScales);
//	
//
//		b.detect(img, keypointsA);
//		b.compute(img, keypointsA, descriptorsA);
//
//		b.detect(img2, keypointsB);
//		b.compute(img2, keypointsB, descriptorsB);
//
//		BFMatcher matcher(NORM_L2);
//
//		std::vector<cv::DMatch> matches;
//		matcher.match(descriptorsA, descriptorsB, matches);
//
//		cv::Mat all_matches;
//		cv::drawMatches(img, keypointsA, img2, keypointsB,
//			matches, all_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
//			vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//		cv::imshow("BRISK All Matches", all_matches);
//		cv::waitKey(0);
//
//

		//-- Show detected matches
		//imshow("Matches", img_matches);


		//blur(img, img, Size(4, 4));
		//cvtColor(img, img, CV_RGB2GRAY);
		cv::Mat canny(img.size(), CV_8UC1);
		//bitwise_not(img, img);
		Canny(img, canny, 50, 200, 3);
		Canny(img2, img2, 50, 200, 3);

		cv::namedWindow("bw", CV_WINDOW_AUTOSIZE);
		cv::namedWindow("kol", CV_WINDOW_AUTOSIZE);

		//if (!img.data || !img2.data)
		//{
		//	std::cout << " --(!) Error reading images " << std::endl; getchar(); return -1;
		//
		//}

		Mat dst = img;
		Mat cdst = img;
		Canny(img, dst, 50, 200, 3);
		cvtColor(dst, cdst, CV_GRAY2BGR);

		//vector<Vec4i> lines;
		//HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
		//for (size_t i = 0; i < lines.size(); i++)
		//{
		//	Vec4i l = lines[i];
		//	line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
		//}

		cv::imshow("bw", img2);
		//std::thread t1([=]() { imshow("t2", img); });
		cvWaitKey(0);
		//std::thread t2([=]() { imshow("t2", img2); });
		cv::imshow("kol", img);
		cvWaitKey(0);

//		t1.join(); t2.join();
		getchar();
	}
	catch (std::exception& exc) {
		std::cout << exc.what() << std::endl;
		getchar();
		exit(EXIT_FAILURE);
	}

	return 0;
}
