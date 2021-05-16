#include <iostream>
#include <string>
using namespace std;

class Location {
public:
	int col; //행
	int row; //열
public:
	void set(int r, int c); //위치를 정하는 함수
	int getCol(); //return col;
	int getRow(); //return row;
	Location(int r, int c); //매개변수가 있는 생성자
	Location(); //기본 생성자
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