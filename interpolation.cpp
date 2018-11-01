#include <iostream>
#include <interpolation.h>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
using namespace std;
using namespace cv;

Interpolation::Interpolation(double* solution1, double* solution2, int traitNum, double* original, Mat oldImage) {
	//注意都是浅复制
	this->solution1 = solution1;
	this->solution2 = solution2;
	this->traitNum = traitNum;
	this->original = original;
	this->oldImage = oldImage;
	oldImage.copyTo(this->newImage);// Mat::zeros(oldImage.size(), oldImage.type());
}
Vec3b Interpolation::bilinear(double x, double y) {
	Vec3b a, b;
	a = (x - int(x))*oldImage.at<Vec3b>(int(x) + 1, int(y) + 1) + (int(x)+1-x)*oldImage.at<Vec3b>(int(x), int(y) + 1);
	b = (x - int(x))*oldImage.at<Vec3b>(int(x) + 1, int(y)) + (int(x) + 1 - x)*oldImage.at<Vec3b>(int(x), int(y));
	return (y - int(y))*a + (int(y) + 1 - y)*b;
}
double Interpolation::Ur(int x1, int y1, int x2, int y2) {
	double rSquare = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	return rSquare * log(rSquare);
}
double* Interpolation::trans(double x, double y) {
	//solution为Lx = Y方程的解，形式为【w1~n,a1,ax,ay】solution1是第一个维度，solution2是第二个维度
	//返回值为{fx,fy}
	int nadd3 = traitNum + 3;
	double sumU1 = 0;
	double sumU2 = 0;
	for (int i = 0; i < nadd3 - 3; i++) {
		sumU1 += solution1[i] * Ur(x, y, original[2 * i], original[2 * i + 1]);
		sumU2 += solution2[i] * Ur(x, y, original[2 * i], original[2 * i + 1]);
	}
	double fx = solution1[nadd3 - 3] + solution1[nadd3 - 2] * x + solution1[nadd3 - 1] * y + sumU1;
	double fy = solution2[nadd3 - 3] + solution2[nadd3 - 2] * x + solution2[nadd3 - 1] * y + sumU2;
	double* fxy = new double[2];
	fxy[0] = fx; fxy[1] = fy;
	return fxy;
}
void Interpolation::transfer() {
	double* temp = nullptr;
	for (int i = 0; i < oldImage.rows; i++) {
		for (int j = 0; j < oldImage.cols; j++) {
			temp = trans(i,j);
			//cout <<i<<"  "<< j  <<"    ///   "<<temp[0] << ends << temp[1] << endl;
			if (temp[0] <= oldImage.rows-1 && temp[0] > 0 && temp[1] <= oldImage.cols && temp[1] > 0) {
				newImage.at<Vec3b>(i, j) = bilinear(temp[0], temp[1]);
				//int(temp[0]),int(temp[1]) 注意不要超出图片边界
			}
		}
	}
}