#pragma once
#include <iostream>
using namespace std;

class LinearEquations {
	//用来求解方阵Ax=b
private:
	int n;  //A的维度
	double* A;
	double* B;  //假设为n*1矩阵
	double* C;
	int* line;//保存换行后行的顺序
	double& mA(int i, int j);
public:
	LinearEquations(int n, double* A, double* B, double* C=nullptr);
	double* solutionB;
	double* solutionC;
	void solve();
	void elimination();
	void lastStep();
	bool zeroUp(int i);//换行使得第i行第一个不为0;如果第一行仍然为0，return false
	void zeroFirstCol(int j);//使得ajj左下元素全为0
	void changeLine(int i, int j);//换行
	void addLine(int line1, int line2, double mul); //line1 += line2*mul
	void printABC(); //打印AB
};