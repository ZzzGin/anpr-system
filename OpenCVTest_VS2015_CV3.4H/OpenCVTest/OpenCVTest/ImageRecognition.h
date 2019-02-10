#pragma once

#ifndef ImageRecognition_h
#define ImageRecognition_h

#include <string.h>
#include <vector>

#include "Plate.h"

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ImageRecognition {
public:
	ImageRecognition();
	string filename;
	void setFilename(string f);
	bool saveRecognition;
	bool showSteps;
	vector<Plate> run(Mat input);

	vector<Plate> segment(Mat input);
	bool verifySizes(RotatedRect mr);
	Mat histeq(Mat in);
};

#endif
