#pragma once
#include <iostream>
#include <interpolation.h>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
using namespace std;
using namespace cv;
class Interpolation {
private:
	double* solution1;// LX=Y���̽�ĵ�һ��
	double* solution2;// LX=Y���̽�ĵڶ���
	int traitNum;
	double* original;
	Mat oldImage;
public:
	Interpolation(double* solution1, double* solution2, int traitNum, double* original, Mat oldImage);
	Mat newImage;
	double* trans(double x, double y);
	double Ur(int x1, int y1, int x2, int y2);
	void transfer();
	Vec3b bilinear(double x, double y); //˫���Բ�ֵ
};