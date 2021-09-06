#include<iostream>
#include"p0_starter.h"

using namespace scudb;
using namespace std;
int main() {
	int a[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	int b[9] = { 2, 0, 3, 4, 6, 2, 5, 0, 1 };
	int c[9] = { 7, 1, 2, 4, 1, 3, 0, 2, 1 };
	vector<int> sourceA(a, a + 9);
	vector<int> sourceB(b, b + 9);
	vector<int> sourceC(c, c + 9);
	vector<int> source_wrong(a, a + 8);
	RowMatrix<int> mA(3, 3);
	RowMatrix<int> mB(3, 3);
	RowMatrix<int> mC(3, 3);
	//
	mA.FillFrom(source_wrong);
	mA.FillFrom(sourceA);
	mA.Show();
	cout << endl;
	mB.FillFrom(sourceB);
	mB.Show();
	cout << endl;
	mC.FillFrom(sourceC);
	mC.Show();
	cout << endl;
	//
	RowMatrixOperations<int> op;
	std::unique_ptr<RowMatrix<int>> temp = op.GEMM(&mA,&mB,&mC);
	//
	temp.get()->Show();
	cout << endl;
	//
	temp = op.Add(&mA, &mB);
	temp.get()->Show();
	cout << endl;
	//
	temp = op.Multiply(&mC, &mB);
	temp.get()->Show();
	cout << endl;
}