#include <iostream>
#include<LinearEquations.h>
using namespace std;
//只能在方阵n*n，且秩r=n的情况下工作

LinearEquations::LinearEquations(int n, double* A, double* B, double* C) {
	//注意ABC都只做了浅复制
	this->n = n;
	this->A = A;
	this->B = B;
	this->C = C;
	this->solutionB = nullptr;
	this->solutionC = nullptr;
	this->line = new int[n];
	//保证传入的矩阵中没有应该为0的非零小数
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (abs(A[i*n+j]) <= 2 * DBL_EPSILON)       A[i*n+j] = 0;
		}
	}
	//初始化line,注意line初始化前调用mA
	for (int i = 0; i < n; i++) {
		line[i] = i;
	}
}

void LinearEquations::solve() {
	//假设第一列的n个系数不全为0;
	elimination();
    lastStep();
}

void LinearEquations::elimination() {
	//gauss消元
	for (int i = 0; i < n; i++) {
		zeroUp(i); //换行使a11(第一行第一个)不为0；
		zeroFirstCol(i); //将第一行的k倍加到第2到n行，使得第一列为0（除了a11）
		//cout << "-----------------" << i << "---------------" << endl;
		//this->printABC();
	}
}

void LinearEquations::lastStep() {
	                                                                             //未考虑除以0的情况
    solutionB = new double[n]();
	if(C!=nullptr)  solutionC = new double[n]();
	solutionB[n - 1] = B[n - 1] / mA(n - 1, n - 1);
	if (C != nullptr)  solutionC[n - 1] = C[n - 1] / mA(n - 1, n - 1);
	for (int i = n - 2; i >= 0; i--) {
		solutionB[i] = B[i];
		if (C != nullptr)  solutionC[i] = C[i];
		for (int k = i + 1; k < n; k++) {
			solutionB[i] -= mA(i, k)*solutionB[k];
			if (C != nullptr) solutionC[i] -= mA(i, k)*solutionC[k];
		}
		solutionB[i] /= mA(i, i);
		if (C != nullptr)  solutionC[i] /= mA(i, i);
	}
}
bool LinearEquations::zeroUp(int i) {
	//对i,i右下
	if (abs(mA(i, i)) <= 2*DBL_EPSILON) {
		for (int k = i + 1; k < n; k++) {
			if (mA(k, i) != 0) {
				changeLine(i, k); break;
			}
		}
	}
	if (mA(i, i) == 0) return false;                                                                            //如果return false 怎么办？
	else                        return true;
}

void LinearEquations::zeroFirstCol(int j) {
	if (mA(j, j) == 0) return;
	//后面的行加上-akj/ajj*linej
	//cout << "j:" << j << endl;
	for (int k = j + 1; k < n; k++) {
		double mul = 0 - mA(k, j) / mA(j, j);
		//cout << "k:" << k << ends << "mul:" << mul << endl;
		addLine(k, j, mul);   //linek += linej * mul
	}
}
void LinearEquations::changeLine(int i, int j) {
	//对A
	if (i >= n || j >= n) return;
	int temp = line[i];
	line[i] = line[j];
	line[j] = temp;
	//对B
	temp = B[i];
	B[i] = B[j];
	B[j] = temp;
	//对C
	if (C != nullptr) {
		temp = C[i];
		C[i] = C[j];
		C[j] = temp;
	}

}

double& LinearEquations::mA(int i, int j) {
	return A[line[i] * n + j];
}

void LinearEquations::addLine(int line1, int line2, double mul) {
	//line1 += line2*mul
	//cout<<"before------adline-------------"<<endl;
	//this->();
	for (int i = 0; i < n; i++) {
		mA(line1, i) += mA(line2, i)*mul;
	}
	B[line1] += B[line2] * mul;
	if(C!=nullptr) C[line1] += C[line2] * mul;
	//cout << "-----------addline:" << endl;
	//this->printABC();
}

void LinearEquations::printABC() {
	for (int i = 0; i<n; i++) cout << line[i] << endl;
	cout << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << mA(i, j) << ends;
		}
		cout << B[i] << ends;
		if (C != nullptr) cout << C[i];
		cout << endl;
	}
}

