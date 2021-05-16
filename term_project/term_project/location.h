#include <iostream>
#include <string>
using namespace std;

class Location {
public:
	int col; //��
	int row; //��
public:
	void set(int r, int c); //��ġ�� ���ϴ� �Լ�
	int getCol(); //return col;
	int getRow(); //return row;
	Location(int r, int c); //�Ű������� �ִ� ������
	Location(); //�⺻ ������
};

Location::Location() {
	col = 0;
	row = 0;
}

Location::Location(int r, int c) {
	this->col = c;
	this->row = r;
}

void Location::set(int r, int c) {
	this->col = c;
	this->row = r;
}

int Location::getCol() {
	return this->col;
}

int Location::getRow() {
	return this->row;
}