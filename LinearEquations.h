#pragma once
#include <iostream>
using namespace std;

class LinearEquations {
	//������ⷽ��Ax=b
private:
	int n;  //A��ά��
	double* A;
	double* B;  //����Ϊn*1����
	int* line;//���滻�к��е�˳��
	double& mA(int i, int j);
public:
	LinearEquations(int n, double* A, double* B, int sizeB = 1);
	double* solve();
	void elimination();
	double* lastStep();
	bool zeroUp(int i);//����ʹ�õ�i�е�һ����Ϊ0;�����һ����ȻΪ0��return false
	void zeroFirstCol(int j);//ʹ��ajj����Ԫ��ȫΪ0
	void changeLine(int i, int j);//����
	void addLine(int line1, int line2, double mul); //line1 += line2*mul
	void printC(); //��ӡAB
};