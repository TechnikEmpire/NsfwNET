/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <NsfwNet.hpp>
#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

#pragma comment(lib, "NsfwNetNative.lib")
#pragma comment(lib, "opencv_core331.lib")
#pragma comment(lib, "opencv_dnn331.lib")
#pragma comment(lib, "opencv_imgproc331.lib")
#pragma comment(lib, "opencv_video331.lib")
#pragma comment(lib, "opencv_videoio331.lib")
#pragma comment(lib, "opencv_highgui331.lib")

// This example shows how to, with the help of ffmpeg, classify
// videos in realtime using the squeezenet model.

static void matPrint(cv::Mat& img, int lineOffsY, cv::Scalar fontColor, const std::string &ss)
{
	int fontFace = cv::FONT_HERSHEY_DUPLEX;
	double fontScale = 0.8;
	int fontThickness = 2;
	cv::Size fontSize = cv::getTextSize("T[]", fontFace, fontScale, fontThickness, 0);

	cv::Point org;
	org.x = 1;
	org.y = 3 * fontSize.height * (lineOffsY + 1) / 2;
	putText(img, ss, org, fontFace, fontScale, cv::Scalar(0, 0, 0), 5 * fontThickness / 2, 16);
	putText(img, ss, org, fontFace, fontScale, fontColor, fontThickness, 16);
}

static void DisplayState(cv::Mat& canvas, double fps, bool isPorn)
{
	cv::Scalar fontColorRed = cv::Scalar(255, 0, 0);
	cv::Scalar fontColorNV = cv::Scalar(118, 185, 0);

	std::ostringstream ss;
	ss << "FPS = " << std::setprecision(1) << std::fixed << fps;
	matPrint(canvas, 0, fontColorRed, ss.str());
	ss.str("");
	ss << "Is Pornography? " << std::boolalpha << isPorn;
	matPrint(canvas, 1, fontColorRed, ss.str());
}

int main(int argc, char** argv)
{

	if (argc < 2 || argc > 2)
	{
		std::cout << "Supply the path to a video file as the sole param." << std::endl;
		return -1;
	}

	// TODO - These files names are out dated.

#ifndef USE_YAHOO
	std::string prototxtPath(u8"deploy_2.prototxt");
	std::string mdlPath(u8"train_iter_58000.caffemodel");
#else
	std::string prototxtPath(u8"deploy.prototxt");
	std::string mdlPath(u8"resnet_50_1by2_nsfw.caffemodel");
#endif

	auto classifier = classifier_create_from_fs(prototxtPath.c_str(), prototxtPath.size(), mdlPath.c_str(), mdlPath.size(), 1);

	if (classifier == nullptr)
	{
		std::cout << "Failed to allocate classifier." << std::endl;
		return -1;
	}

	classifier_set_cutoff(classifier, 0.8);

	cv::VideoCapture capture;
	cv::Mat frame;

	std::string videoFilePath = argv[1];
	std::cout << videoFilePath << std::endl;

	capture.open(videoFilePath);

	if (!capture.isOpened())
	{
		std::cout << "Failed to open video file." << std::endl;
	}

	// Skip ahead a few minutes in the video to get straight to the point.
	capture.set(CV_CAP_PROP_POS_FRAMES, 28500);

	cv::namedWindow("result", 1);

	cv::TickMeter tm;	
	
	bool isPorn = false;

	auto blurSize = cv::Size(37, 37);

	int check = 0;

	cv::Mat lastPornFrame;

	uint_fast8_t count = 0;

	auto black = cv::Scalar(0, 0, 0);
	for (;;)
	{
		tm.reset();
		tm.start();

		capture >> frame;

		if (frame.empty())
		{
			break;
		}

		cv::Mat classFrame = frame.clone();

		if (isPorn && check > 0)
		{
			--check;
		}
		else
		{
			++count;

			if (count % 10 == 0)
			{

				if (classifier_classify_cvmat(classifier, &classFrame))
				{
					isPorn = true;
					check = 200;
					lastPornFrame = frame.clone();
					//lastPornFrame.setTo(black);
					cv::blur(lastPornFrame, lastPornFrame, cv::Size(75, 75));
					//lastPornFrame *= .5;
				}
				else
				{
					isPorn = false;
				}
			}
		}

		tm.stop();
		double detectionTime = tm.getTimeMilli();
		double fps = 1000 / detectionTime;

		if (isPorn)
		{
			lastPornFrame *= .95;
			DisplayState(lastPornFrame, fps, isPorn);
			cv::imshow("result", lastPornFrame);
			//frame.setTo(black);
		}
		else
		{
			DisplayState(frame, fps, isPorn);
			cv::imshow("result", frame);
		}
		
		char key = (char)cv::waitKey(5);
		if (key == 27)
		{
			break;
		}
	}

	return 0;
}
