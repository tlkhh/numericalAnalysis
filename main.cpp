//#include<opencv2\core.hpp>
//#include<opencv2\highgui.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <LinearEquations.h>

//using namespace cv;
using namespace std;

double* readTraitFile(string filename, int TraitNum){
	//第i个特征点的x坐标在2*i,y坐标在2*i+1,i=0,1...TraitNum-1
	ifstream infile(filename);
	double* traitPoint =  new double[2*TraitNum];
	for (int i = 0; i < 2 * TraitNum; i++){
		infile >> traitPoint[i];
		//cout << traitPoint[i]<<endl;
	}
	return traitPoint;
}
double Ur(int x1, int y1, int x2, int y2) {
	double rSquare = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	return rSquare * log(rSquare);
}
double* constructMatrixL(double* original, double* transfer, int TraitNum) {
	double* L = new double[(TraitNum+3)*(TraitNum+3)]();
	int rows = TraitNum + 3;
	//cout << TraitNum<<endl;
	//构建矩阵K（由U构成）;
	for (int i = 0; i < TraitNum; i++) {
		for (int j = 0; j < TraitNum; j++) {
			//cout <<i*TraitNum + j<<endl;
			if(i==j)L[i*rows + j] = 0;
			else L[i*rows + j]=Ur(original[2 * i], original[2 * i + 1], original[2 * j], original[2 * j + 1]);
		}
	}
	//构建P矩阵和PT矩阵
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
	
	//右下角填充0
			for (int i = TraitNum; i < TraitNum + 3; i++) {
				for (int j = TraitNum; j < TraitNum + 3; j++) {
					L[i*rows+j] = 0;
				}
			}

			return L;


}
/*
double* transfermation(double x, double y, double* solution1, double* solution2, int n) {
	//solution为Lx = Y方程的解，形式为【w1~n,a1,ax,ay】solution1是第一个维度，solution2是第二个维度
	//返回值为{fx,fy}
	double fx = solution1[n-3]+solution1[n-2]*x +solution1[n-1]*y+
}*/

int main()
{
		double* testA = new double[16]{ 5,0,4,2,1,-1,2,1,4,1,2,0,1,1,1,1 };
		double* testB = new double[4]{ 3,1,1,0 };
		LinearEquations LE(4, testA, testB);
		LE.printC();
		LE.elimination();
		double* solution = NULL;
		solution = LE.lastStep();
		for (int i = 0; i<4; i++) {
			cout << solution[i] << endl;
	}

	getchar();
	system("pause");
	return 0;
	//读取特征点文件，表示为某种数据结构？
	    //构建K.P.=>L Y
    //求解方程组L*WA=Y，得到w,a向量
	//变换，遍历图中像素点
	//插值

}
