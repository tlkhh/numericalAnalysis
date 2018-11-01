#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <LinearEquations.h>
#include <interpolation.h>

using namespace cv;
using namespace std;

double* readTraitFile(string filename, int TraitNum){
	//��i���������x������2*i,y������2*i+1,i=0,1...TraitNum-1
	ifstream infile(filename);
	double* traitPoint =  new double[2*TraitNum];
	for (int i = 0; i < 2 * TraitNum; i++){
		infile >> traitPoint[i];
		//cout << traitPoint[i]<<endl;
	}
	return traitPoint;
}
double Ur(int x1, int y1, int x2, int y2) {
	double rSquare = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	return rSquare * log(rSquare);
}
double* constructMatrixL(const double* original, int TraitNum) {
	double* L = new double[(TraitNum+3)*(TraitNum+3)]();
	int rows = TraitNum + 3;
	//cout << TraitNum<<endl;
	//��������K����U���ɣ�;
	for (int i = 0; i < TraitNum; i++) {
		for (int j = 0; j < TraitNum; j++) {
			//cout <<i*TraitNum + j<<endl;
			if(i==j)L[i*rows + j] = 0;
			else L[i*rows + j]=Ur(original[2 * i], original[2 * i + 1], original[2 * j], original[2 * j + 1]);
		}
	}
	//����P�����PT����
	for (int i = 0; i < TraitNum; i++) {
		for (int j = 0; j < 3; j++) {
			int indexP = TraitNum + j;
			int pos = i * rows + indexP;
			int posT = indexP * rows + i;
			if (j == 0)      L[pos] = L[posT] = 1;
			else if (j == 1) L[pos] = L[posT] = original[2 * i];
			else                L[pos] = L[posT] = original[2 * i + 1];
		}
	}
	
	//���½����0
			for (int i = TraitNum; i < TraitNum + 3; i++) {
				for (int j = TraitNum; j < TraitNum + 3; j++) {
					L[i*rows+j] = 0;
				}
			}

			return L;


}
void constructY(const double*transfer, int traitNum, double* Y1, double* Y2) {
	//Y1����������X����
	for (int i = 0; i < traitNum; i++) {
		Y1[i] = transfer[2*i];
	}
	//Y2����������Y����
	for (int i = 0; i < traitNum; i++) {
		Y2[i] = transfer[2*i+1];
	}
	//��0
	for (int i = traitNum; i < traitNum + 3; i++) {
		Y1[i] = Y2[i] = 0;
	}
}
void showImage(Mat image,string imageName) {
	namedWindow(imageName, CV_WINDOW_AUTOSIZE);
	imshow(imageName, image);
	waitKey(0);
	destroyWindow(imageName);
}
int main()
{
	Mat oldImage = imread("1.jpg", CV_LOAD_IMAGE_UNCHANGED);
	showImage(oldImage, "oldImage");
	cout << oldImage.cols << ends << oldImage.rows<<"   --------------"<<endl;
	Mat Image2 = imread("2.jpg", CV_LOAD_IMAGE_UNCHANGED);
	Image2.resize(500, 800);                                                          //ò��Ҫ�Լ�дresize
	showImage(Image2, "Image2");
	cout <<Image2.cols << ends << Image2.rows<< "   --------------"<<endl;
	//original:1, transfer:2
	int traitNum = 68;
	double* original = readTraitFile("1.txt",traitNum);
	double* transfer = readTraitFile("2.txt",traitNum);
	double* L = constructMatrixL(original, traitNum);
	double* Y1 = new double[traitNum + 3];
	double* Y2 = new double[traitNum + 3];
	constructY(transfer, traitNum, Y1, Y2);
	LinearEquations LE(traitNum + 3, L,Y1,Y2);
		//LE.printABC();
		LE.elimination();
		//LE.printABC();
		LE.lastStep();
		for (int i = 0; i<71; i++) {
			cout << LE.solutionB[i] << endl;
	}
		cout << "------c---" << endl;
		for (int i = 0; i<71; i++) {
			cout << LE.solutionC[i] << endl;
		}
		//��ֵ
		Interpolation inter(LE.solutionB, LE.solutionC,traitNum,original,Image2);
		inter.transfer();
		showImage(inter.newImage, "newImage");

	getchar();
	//system("pause");
	return 0;
	//��ȡ�������ļ�����ʾΪĳ�����ݽṹ��
	    //����K.P.=>L Y
    //��ⷽ����L*WA=Y���õ�w,a����
	//�任������ͼ�����ص�
	//��ֵ

}
