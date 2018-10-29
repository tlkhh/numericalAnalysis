#include <iostream>
#include<LinearEquations.h>
using namespace std;
//ֻ���ڷ���n*n������r=n������¹���

LinearEquations::LinearEquations(int n, double* A, double* B, int sizeB) {
	this->n = n;
	this->A = A;
	this->B = B;
	this->line = new int[n];
	//��֤����ľ�����û��Ӧ��Ϊ0�ķ���С��
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (abs(A[i*n+j]) <= 2 * DBL_EPSILON)       A[i*n+j] = 0;
		}
	}
	//��ʼ��line,ע��line��ʼ��ǰ����mA
	for (int i = 0; i < n; i++) {
		line[i] = i;
	}
}

double* LinearEquations::solve() {
	//�����һ�е�n��ϵ����ȫΪ0;
	elimination();
	return lastStep();
}

void LinearEquations::elimination() {
	//gauss��Ԫ
	for (int i = 0; i < n; i++) {
		zeroUp(i); //����ʹa11(��һ�е�һ��)��Ϊ0��
		zeroFirstCol(i); //����һ�е�k���ӵ���2��n�У�ʹ�õ�һ��Ϊ0������a11��
		cout << "-----------------" << i << "---------------" << endl;
		this->printC();
	}
}

double* LinearEquations::lastStep() {
	                                                                             //δ���ǳ���0�����
	double* solution = new double[n]();
	solution[n - 1] = B[n - 1] / mA(n - 1, n - 1);
	for (int i = n - 2; i >= 0; i--) {
		solution[i] = B[i];
		for (int k = i + 1; k < n; k++) {
			solution[i] -= mA(i, k)*solution[k];
		}
		solution[i] /= mA(i, i);
	}
	return solution;
}
bool LinearEquations::zeroUp(int i) {
	//��i,i����
	if (abs(mA(i, i)) <= 2*DBL_EPSILON) {
		for (int k = i + 1; k < n; k++) {
			if (mA(k, i) != 0) {
				changeLine(i, k); break;
			}
		}
	}
	if (mA(i, i) == 0) return false;                                                                            //���return false ��ô�죿
	else                        return true;
}

void LinearEquations::zeroFirstCol(int j) {
	if (mA(j, j) == 0) return;
	//������м���-akj/ajj*linej
	cout << "j:" << j << endl;
	for (int k = j + 1; k < n; k++) {
		double mul = 0 - mA(k, j) / mA(j, j);
		cout << "k:" << k << ends << "mul:" << mul << endl;
		addLine(k, j, mul);   //linek += linej * mul
	}
}
void LinearEquations::changeLine(int i, int j) {
	//��A
	if (i >= n || j >= n) return;
	int temp = line[i];
	line[i] = line[j];
	line[j] = temp;
	//��B
	temp = B[i];
	B[i] = B[j];
	B[j] = temp;
}

double& LinearEquations::mA(int i, int j) {
	return A[line[i] * n + j];
}

void LinearEquations::addLine(int line1, int line2, double mul) {
	//line1 += line2*mul
	cout<<"before------adline-------------"<<endl;
	this->printC();
	for (int i = 0; i < n; i++) {
		mA(line1, i) += mA(line2, i)*mul;
	}
	B[line1] += B[line2] * mul;
	cout << "-----------addline:" << endl;
	this->printC();
}

void LinearEquations::printC() {
	for (int i = 0; i<n; i++) cout << line[i] << endl;
	cout << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << mA(i, j) << ends;
		}
		cout << B[i] << endl;
	}
}
