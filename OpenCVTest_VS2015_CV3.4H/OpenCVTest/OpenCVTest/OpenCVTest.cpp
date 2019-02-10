#include <opencv2/opencv.hpp>
#include <iostream>
#include <iostream>
#include <vector>
#include <fstream>
#include "ImageRecognition.h"
#include "Plate.h"
#include "OCR.h"

using namespace std;
using namespace cv;
using namespace cv:: ml;

string getFilename(string s) {

	char sep = '/';
	char sepExt = '.';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != string::npos) {
		string fn = (s.substr(i + 1, s.length() - i));
		size_t j = fn.rfind(sepExt, fn.length());
		if (i != string::npos) {
			return fn.substr(0, j);
		}
		else {
			return fn;
		}
	}
	else {
		return "";
	}
}

void outintotxt(string s) {
	ofstream fout;
	fout.open("LicencePlateNum.txt");
	fout << s << "\n";
	fout << flush; 
	fout.close();
}

int main(int argc, char** argv) {
	cout << "OpenCV Automatic Number Plate Recognition\n";
	char* filename;
	Mat InputImage;

	// if image input correctly
	if (argc >= 2)
	{
		filename = argv[1];
		InputImage = imread(filename, 1);
		stringstream ss(stringstream::in | stringstream::out);
		ss << "input.jpg";
		imwrite(ss.str(), InputImage);
	}
	else {
		printf("Use:\n\t%s image\n", argv[0]);
		return 0;
		//filename = "D:\\OpenCVTestImage\\plate\\FromiPhone\\jingf72089.jpg";
		//InputImage = imread(filename, 1);
	}

	string filename_whithoutExt = getFilename(filename);
	cout << "working with file: " << filename_whithoutExt << "\n";

	// just for test
	// imshow("input",InputImage);
	// waitKey(0);
	// just for test

	// Detect a plate
	ImageRecognition detector;
	detector.setFilename(filename_whithoutExt);
	detector.saveRecognition = false;
	detector.showSteps = false;										//=================================================��ʾrec���迪��
	vector<Plate> PosibleRecognition = detector.run(InputImage);
	// just for test
	// imshow("input",InputImage);
	// waitKey(0);
	// just for test

	// ѵ��SVM,����ѵ���Ͳ��Ե�ͼ�����ݱ�����SVM.xml�ļ���
	FileStorage fs;
	fs.open("D:\\GraduationProject\\OpenCVTest_VS2015_CV3.4H\\OpenCVTest\\OpenCVTest\\SVM.xml", FileStorage::READ);
	Mat SVM_TrainingData;
	Mat SVM_Classes;
	fs["TrainingData"] >> SVM_TrainingData;
	fs["classes"] >> SVM_Classes;

	// ����SVM�Ļ�������
	Ptr<SVM> svm = SVM::create();
	svm->setType (SVM::C_SVC);		// SVM����
	svm->setKernel (SVM::LINEAR);	// ����ӳ��
	svm->setDegree (0);
	svm->setGamma (1);
	svm->setCoef0 (0);
	svm->setC (1);
	svm->setNu (0);
	svm->setP (0);
	svm->setTermCriteria (cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01));

	// ������ѵ��������
	Ptr<TrainData> tData = TrainData::create(SVM_TrainingData, ROW_SAMPLE, SVM_Classes);
	svm->train(tData);

	// ʹ��ѵ���õķ������Բü�ͼ����з��࣬�жϸ������Ƿ�Ϊ����
	vector<Plate> plates;
	for (int i = 0; i< PosibleRecognition.size(); i++)
	{
		Mat img = PosibleRecognition[i].plateImg;
		Mat p = img.reshape(1, 1);
		// ��ͼ��ת��Ϊ����ֵ����Ϊ�����͵ĵ�ͨ��ͼ��
		p.convertTo(p, CV_32FC1);

		int response = (int)svm->predict(p); // ����SVM������
		if (response == 1) {
			plates.push_back(PosibleRecognition[i]);
			// cout << i+1 << "\n";
		}
	}

	// �����������ж�Ϊ����ʱ���1������Ϊ0
	cout << "Num plates detected: " << plates.size() << "\n";
	// int wait1num;
	// cin >> wait1num;
	if (plates.size() != 0) {
		// �Ա��궨Ϊ���Ƶ��������OCR�ָ�
		OCR ocr("OCR.xml");
		ocr.saveSegments = false;
		ocr.DEBUG = false;												//=================================================��ʾocr���迪��
		ocr.filename = filename_whithoutExt;
		for (int i = 0; i < plates.size(); i++) {
			Plate plate = plates[i];
			string plateNumber = ocr.run(&plate);
			string licensePlate = plate.str();
			cout << "================================================\n";
			cout << "License plate number: " << licensePlate << "\n";
			cout << "================================================\n";
			outintotxt(licensePlate); //�����պ����������ߵ��ļ��У���������ʾ��ûʲôʵ������
			rectangle(InputImage, plate.position, Scalar(0, 0, 200));
			putText(InputImage, licensePlate, Point(plate.position.x, plate.position.y), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 200), 2);
			if (false) {
				imshow("Plate Detected seg", plate.plateImg);
				cvWaitKey(0);
			}

		}
		//imshow("Numbers of the Plate", InputImage);
		stringstream ss(stringstream::in | stringstream::out);
		ss << "output.jpg";
		Mat OutputImageResize;
		resize(InputImage, OutputImageResize, Size(720, 540));
		imwrite(ss.str(), OutputImageResize);
		/* һ���ҿ������ĵȴ�������asiic����27��esc
		for (;;)
		{
			int c;
			c = cvWaitKey(10);
			if ((char)c == 27)
				break;
		}
		return 0;
		*/
	}
	else {
		outintotxt("000000");
	}
}