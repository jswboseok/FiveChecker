#include <iostream>
#include <string>
#include <cstdlib>
#include "location.h"
using namespace std;
#define Board_Size 10
#define MARK3 99999 //컴퓨터가 막아야 하는 부분을 마킹
#define MARK4 999999 //컴퓨터가 막아야 하는 부분을 마킹, 4개 짜리가 더 위험하므로

typedef struct spot { //한 지점의 상태값과 가중치
	int state; // 상태값
	int weight; // 가중치
}spot;

class Game_Board {
public:
	//Location location; //바둑판 위에서 위치를 잡기 위한 멤버 변수
	spot gameBoard[Board_Size][Board_Size]; //바둑판 선언
	int turn; //1 == 내 차례, 0 == 컴퓨터 차례
public:
	void chakshu(); //한 점에 착수
	void check_closed_4(); //닫힌 4목 확인
	void check_blanked_4(); //띈 4목 확인
	void check_blanked_3(); //띈 3목 확인
	void check_opened_3(); //열린 3목 확인
	bool check_5(); //승리조건 확인
	int getGBstate(int r, int c); //한 spot의 state 불러옴
	int getGBweight(int r, int c); //한 spot의 가중치 불러옴
	void setGBstate(int r, int c, int value);
	void setGBweight(int r, int c, int value);
	void draw(); //오목판 그리기
	void init(int);
	void calc_weight();
};

void Game_Board::init(int select) {
	for (int i = 0; i < Board_Size; i++) {
		for (int j = 0; j < Board_Size; j++) {
			setGBstate(i, j, -1);
			setGBweight(i, j, 0);
		}
	}
	turn = select;
	int i = rand() % Board_Size;
	int j = rand() % Board_Size;
	setGBweight(i, j, MARK3);
}

void Game_Board::chakshu() {
	this->calc_weight();
	int r, c;
	
	if (turn) { //내 차례
		cout << "점을 선택하시오 : ";
		cin >> r >> c;
		if (getGBstate(r, c) != -1) {
			setGBweight(r, c, 0);
			cout << "다시 선택하세요" << endl << endl;
		}
		else {
			//location.set(r, c);
			setGBstate(r, c, turn); //내 돌 놓기
			turn = !turn; //차례 돌리기
		}
	} 
	else { //상대 차례
		check_blanked_3();
		check_opened_3();
		check_closed_4();
		check_blanked_4();
		int max = -1, index_i = 0, index_j = 0;
		for (int i = 0; i < Board_Size; i++) {
			for (int j = 0; j < Board_Size; j++) {
				if (getGBstate(i, j) != -1) { //돌이 있는 곳 혹시 몰라 weight 다시 0
					setGBweight(i, j, 0);
				}
				if (getGBstate(i, j) == -1 && getGBweight(i, j) >= max) { //돌이 없고 가중치가 가장 높은 곳을 찾음
					max = gameBoard[i][j].weight;
					index_i = i;
					index_j = j;
				}
			}
		}
		setGBstate(index_i, index_j, turn); //상대돌 놓기
		turn = !turn; //차례 돌리기
	}
}
void Game_Board::check_closed_4() {
	//////////////////////
	//가로 닫힌 4 체크////
	//////////////////////
	for (int i = 0; i < Board_Size; i++) { //맨 왼쪽 가로로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[j][i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(4, i, MARK4);
	}
	for (int i = 0; i < Board_Size; i++) { //맨 오른쪽 가로로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[Board_Size - 1 - j][i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(Board_Size - 5, i, MARK4);
	}
	for (int j = 0; j < Board_Size; j++) { // 가운데 가로로 닫힌 4 -> 왼쪽에 상대돌
		for (int i = 1; i < Board_Size - 4; i++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i - 1][j].state == 0 && gameBoard[i + 4][j].state == -1 && getGBstate(i + k, j) == 1)
					cnt++;
			}
			if (cnt == 4)
				setGBweight(i + 4, j, MARK4);
		}
	}
	for (int j = 0; j < Board_Size; j++) { // 가운데 가로로 닫힌 4 -> 오른쪽에 상대돌
		for (int i = Board_Size - 2; i >= 5; i--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i + 1][j].state == 0 && gameBoard[i - 4][j].state == -1 && getGBstate(i - k, j) == 1)
					cnt++;
			}
			if (cnt == 4)
				setGBweight(i - 4, j, MARK4);
		}
	}
	/////////////////////////
	//세로 닫힌 4 체크////
	/////////////////////////
	for (int i = 0; i < Board_Size; i++) { //위쪽 세로로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i, j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i, 4, MARK4);
	}
	for (int i = 0; i < Board_Size; i++) { //아래쪽 세로로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i, Board_Size - 1 - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i, Board_Size - 5, MARK4);
	}
	for (int i = 0; i < Board_Size; i++) { // 가운데 세로로 닫힌 4 -> 위쪽에 상대돌
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i, j - 1) == 0 && getGBstate(i,j + 4) == -1 && getGBstate(i, j + k) == 1)
					cnt++;
			}
			if (cnt == 4)
				setGBweight(i, j + 4, MARK4);
		}
	}
	for (int i = 0; i < Board_Size; i++) { // 가운데 세로로 닫힌 4 -> 아래쪽에 상대돌
		for (int j = Board_Size - 2; j >= 5; j--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i, j + 1) == 0 && getGBstate(i, j - 4) == -1 && getGBstate(i, j - k) == 1)
					cnt++;
			}
			if (cnt == 4)
				setGBweight(i, j - 4, MARK4);
		}
	}
	

	///////////////////////////////////////////////////////
	/////왼쪽 위 -> 오른쪽 아래 대각선으로 닫힌 4 체크/////
	///////////////////////////////////////////////////////
	for (int i = 0; i < Board_Size - 4; i++) { //맨 위쪽(j == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i + j, j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i + 4, 4, MARK4);
	}
	for (int j = 0; j < Board_Size - 4; j++) { // 맨 오른쪽(i == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(i, j + i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(4, j + 4, MARK4);
	}
	for (int j = 4; j < Board_Size; j++) { // 맨 왼쪽(i == Board_Size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(Board_Size - 1 - i, j - i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(Board_Size - 5, j - 4, MARK4);
	}
	for (int i = 4; i < Board_Size; i++) { //맨 아래쪽(j == Board_size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i - j, Board_Size - 1 - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i - 4, Board_Size - 5, MARK4);
	}
	for (int i = 1; i < Board_Size - 4; i++) { //가운데 대각선으로 닫힌 4 -> 위쪽에 돌
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i - 1, j - 1) == 0 && getGBstate(i + 4, j + 4) == -1 && getGBstate(i + k, j + k) == 1)
					cnt++;
			}
			if (cnt == 4)
				setGBweight(i + 4, j + 4, MARK4);
		}
	}
	for (int i = Board_Size - 2; i > 4; i--) { //가운데 대각선으로 닫힌 4 -> 아래쪽에 돌
		for (int j = Board_Size - 2; j > 4; j--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + 1, j + 1) == 0 && getGBstate(i - 4, j - 4) == -1 && getGBstate(i - k, j - k) == 1)
					cnt++;
			}
			if (cnt == 4)
				setGBweight(i - 4, j - 4, MARK4);
		}
	}
	//////////////////////////////////////////////////////////////
	///////오른쪽 위 -> 왼쪽 아래 대각선으로 닫힌 4 체크//////////
	//////////////////////////////////////////////////////////////
	for (int i = 4; i < Board_Size; i++) { //맨 위쪽(j == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i - j][j].state == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i - 4, 4, MARK4);
	}
	for (int j = 4; j < Board_Size; j++) { // 맨 오른쪽(i == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(j - i, i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(j - 4, 4, MARK4);
	}
	for (int j = 0; j < Board_Size - 4; j++) { // 맨 왼쪽(i == Board_Size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(Board_Size - 1 - i, j + i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(Board_Size - 5, j + 4, MARK4);
	}
	for (int i = 0; i < Board_Size - 4; i++) { //맨 아래쪽(j == Board_size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i + j, Board_Size - 1 - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i + 4, Board_Size - 5, MARK4);
	}
	for (int i = Board_Size - 2; i > 4; i--) { //가운데 대각선으로 닫힌 4 -> 위쪽에 돌
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + 1, j - 1) == 0 && getGBstate(i - 4, j + 4) == -1 && getGBstate(i - k, j + k) == 1)
					cnt++;
			}
			if (cnt == 4)
				setGBweight(i - 4, j + 4, MARK4);
		}
	}
	for (int i = 1; i < Board_Size - 4; i++) { //가운데 대각선으로 닫힌 4 -> 아래쪽에 돌
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i - 1, j + 1) == 0 && getGBstate(i + 4, j - 4) == -1 && getGBstate(i + k, j - k) == 1)
					cnt++;
			}
			if (cnt == 4)
				setGBweight(i + 4, j - 4, MARK4);
		}
	}
}
void Game_Board::check_blanked_4() {
	//가로
	for (int j = 0; j < Board_Size; j++) {
		for (int i = 0; i < Board_Size - 4; i++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j) == 1)
					cnt++;
				if ((i + k != i || i + k != i + 4) && getGBstate(i + k, j) == -1) {
					blank++;
					index = i + k;
				}
			}
			if (blank == 1 && cnt == 4 && index != -1)
				setGBweight(index, j, MARK4);
		}
	}
	//세로
	for (int i = 0; i < Board_Size; i++) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i, j + k) == 1)
					cnt++;
				if ((j + k != j || j + k != j + 4) && getGBstate(i, j + k) == -1) {
					blank++;
					index = j + k;
				}
			}
			if (blank == 1 && cnt == 4 && index != -1)
				setGBweight(i, index, MARK4);
		}
	}
	//왼쪽 위 -> 오른쪽 아래 대각선
	for (int i = 0; i < Board_Size - 4;i++) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j + k) == 1)
					cnt++;
				if (getGBstate(i + k, j + k) == -1) {
					blank++;
					index = k;
				}
			}
			if (blank == 1 && cnt == 4)
				setGBweight(i + index, j + index, MARK4);
		}
	}
	//오른쪽 위 -> 왼쪽 아래 대각선
	for (int i = Board_Size - 1; i > 3; i--) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int blank = 0, cnt = 0, blank_index_i = -1, blank_index_j = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i - k, j + k) == 1)
					cnt++;
				if (getGBstate(i - k, j + k) == -1) {
					blank++;
					blank_index_i = i - k;
					blank_index_j = j + k;
				}
			}
			if (blank == 1 && cnt == 4)
				setGBweight(blank_index_i, blank_index_j,MARK4);
		}
	}
}
void Game_Board::check_blanked_3() {

	//가로
	for (int j = 0; j < Board_Size; j++) {
		for (int i = 0; i < Board_Size - 3; i++) {
			int index = i, blank = 0, cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j) == 1)
					cnt++;
				if (getGBstate(i + k, j) == -1) {
					blank++;
					index = i + k;
				}
			}
			if(blank == 1 && cnt == 3)
				setGBweight(index, j, MARK3);
		}
	}
	//세로
	for (int i = 0; i < Board_Size; i++) {
		for (int j = 0; j < Board_Size - 3; j++) {
			int index = j, blank = 0, cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i, j + k) == 1)
					cnt++;
				if (getGBstate(i, j + k) == -1) {
					blank++;
					index = j + k;
				}
			}
			if (blank == 1 && cnt == 3)
				setGBweight(i, index, MARK3);
		}
	}
	//대각선1
	for (int i = 0; i < Board_Size - 3; i++) {
		for (int j = 0; j < Board_Size - 3; j++) {
			int index = 0, blank = 0, cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j + k) == 1)
					cnt++;
				if (getGBstate(i + k, j + k) == -1) {
					blank++;
					index = k;
				}
			}
			if (blank == 1 && cnt == 3)
				setGBweight(i + index, j + index, MARK3);
		}
	}
	//대각선2
	for (int i = 3; i < Board_Size; i++) {
		for (int j = 0; j < Board_Size - 3; j++) {
			int index = 0, blank = 0, cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i - k, j + k) == 1)
					cnt++;
				if (getGBstate(i - k, j + k) == -1) {
					blank++;
					index = k;
				}
			}
			if (blank == 1 && cnt == 3)
				setGBweight(i - index, j + index, MARK3);
		}
	}
}
void Game_Board::check_opened_3() {
	//가로
	for (int j = 0; j < Board_Size; j++) {
		for (int i = 1; i < Board_Size - 3; i++) {
			if (getGBstate(i - 1, j) == -1 && getGBstate(i + 3, j) == -1) {
				int cnt = 0;
				for (int k = 0; k < 3; k++) {
					if (getGBstate(i + k, j) == 1)
						cnt++;
				}
				if (cnt == 3) {
					if (i < Board_Size / 2)
						setGBweight(i - 3, j, MARK3);
					else
						setGBweight(i + 1, j, MARK3);
				}
			}
		}
	}
	//세로
	for (int i = 0; i < Board_Size; i++) {
		for (int j = 1; j < Board_Size - 3; j++) {
			if (getGBstate(i, j - 1) == -1 && getGBstate(i, j + 3) == -1) {
				int cnt = 0;
				for (int k = 0; k < 3; k++) {
					if (getGBstate(i, j + k) == 1)
						cnt++;
				}
				if (cnt == 3) {
					if (i < Board_Size / 2)
						setGBweight(i, j - 3, MARK3);
					else
						setGBweight(i, j - 1, MARK3);
				}
			}
		}
	}
	//대각선1
	for (int i = 1; i < Board_Size - 3; i++) {
		for (int j = 1; j < Board_Size - 3; j++) {
			if (getGBstate(i - 1, j - 1) == -1 && getGBstate(i + 3, j + 3) == -1) {
				int cnt = 0;
				for (int k = 0; k < 3; k++) {
					if (getGBstate(i + k, j + k) == 1)
						cnt++;
				}
				if (cnt == 3) {
					if (i < Board_Size / 2)
						setGBweight(i - 3, j - 3, MARK3);
					else
						setGBweight(i + 1, j - 1, MARK3);
				}
			}
		}
	}
	//대각선2
	for (int i = Board_Size - 1; i > 2; i--) {
		for (int j = 1; j < Board_Size - 3; j++) {
			if (getGBstate(i + 1, j - 1) == -1 && getGBstate(i - 3, j + 3) == -1) {
				int cnt = 0;
				for (int k = 0; k < 3; k++) {
					if (getGBstate(i - k, j + k) == 1)
						cnt++;
				}
				if (cnt == 3) {
					if (i < Board_Size / 2)
						setGBweight(i - 3, j - 3, MARK3);
					else
						setGBweight(i + 1, j - 1, MARK3);
				}
			}
		}
	}
}

bool Game_Board::check_5() {
	//가로 체크
	for (int i = 0; i < Board_Size - 4; i++) {
		for (int j = 0; j < Board_Size; j++) {
			int mycnt = 0, opcnt = 0;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j) == 1) {
					mycnt++;
				}
				else if (getGBstate(i + k, j) == 0) {
					opcnt++;
				}
			}
			if (mycnt == 5) {
				cout << "내가 이김" << endl;
				return true;
			}
			else if (opcnt == 5) {
				cout << "내가 짐" << endl;
				return true;
			}
		}
	}
	//세로 체크
	for (int i = 0; i < Board_Size; i++) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int mycnt = 0, opcnt = 0;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i, j + k) == 1) {
					mycnt++;
				}
				else if (getGBstate(i, j + k) == 0) {
					opcnt++;
				}
			}
			if (mycnt == 5) {
				cout << "내가 이김" << endl;
				return true;
			}
			else if (opcnt == 5) {
				cout << "내가 짐" << endl;
				return true;
			}
		}
	}
	//왼쪽 위 -> 오른쪽 아래 대각선 체크
	for (int i = 0; i < Board_Size - 4; i++) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int mycnt = 0, opcnt = 0;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j + k) == 1) {
					mycnt++;
				}
				else if (getGBstate(i + k, j + k) == 0) {
					opcnt++;
				}
			}
			if (mycnt == 5) {
				cout << "내가 이김" << endl;
				return true;
			}
			else if (opcnt == 5) {
				cout << "내가 짐" << endl;
				return true;
			}
		}
	}
	//오른쪽 위 -> 왼쪽 아래 대각선 체크
	for (int i = Board_Size - 1; i > 4; i--) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int mycnt = 0, opcnt = 0;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i - k, j + k) == 1) {
					mycnt++;
				}
				else if (getGBstate(i - k, j + k) == 0) {
					opcnt++;
				}
			}
			if (mycnt == 5) {
				cout << "내가 이김" << endl;
				return true;
			}
			else if (opcnt == 5) {
				cout << "내가 짐" << endl;
				return true;
			}
		}
	}
	return false;
}

int Game_Board::getGBstate(int r, int c) {
	return gameBoard[r][c].state;
}
int Game_Board::getGBweight(int r, int c) {
	return gameBoard[r][c].weight;
}
void Game_Board::setGBstate(int r, int c, int value) {
	gameBoard[r][c].state = value;
}
void Game_Board::setGBweight(int r, int c, int value) {
	gameBoard[r][c].weight = value;
}

void Game_Board::draw() {
	this->calc_weight();
	cout << endl;
	for (int i = -1; i < Board_Size; i++) {
		if (i == -1)
			cout << "    ";
		else
			cout << i << " ";
	}
	cout << endl;
	for (int i = -1; i <= Board_Size; i++)
		cout << "==";
	cout << endl;

	for (int j = 0; j < Board_Size; j++) {
		cout << j << " | ";
		for (int i = 0; i < Board_Size; i++) {
			if (getGBstate(i, j) == -1) {
				cout << "+ ";
			}
			else if (getGBstate(i, j) == 0) {
				cout << "X ";
			}
			else if (getGBstate(i, j) == 1) {
				cout << "O ";
			}
		}
		cout << endl;
	}
	cout << endl << endl;
}

void Game_Board::calc_weight() { //가중치 구하기
	for (int i = 0; i < Board_Size; i++) { //weight 초기화
		for (int j = 0; j < Board_Size; j++) {
			if (gameBoard[i][j].weight < 1000) { //MARK4보다 훨씬 작은 수는 MARK4ING이 안된 것이라는 의미이므로
				setGBweight(i, j, 0);
			}
		}
	}

	for (int j = 0; j < Board_Size; j++) {
		for (int i = 0; i < Board_Size; i++) {
			if (j == 0) { // 위
				if (i == 0) {//왼쪽 위 모서리
					if (getGBstate(i, j) == 1) { //상대 돌이면 가중치 -2
						gameBoard[i + 1][j].weight -= 2;
						gameBoard[i + 1][j + 1].weight -= 2;
						gameBoard[i][j + 1].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //내 돌이면 가중치 +2
						gameBoard[i + 1][j].weight += 2;
						gameBoard[i + 1][j + 1].weight += 2;
						gameBoard[i][j + 1].weight += 2;
					}
				}
				else if (i == 9) { //오른쪽 위
					if (getGBstate(i, j) == 1) { //상대 돌이면 가중치 -2
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i - 1][j].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //내 돌이면 가중치 +2
						gameBoard[i][j - 1].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i - 1][j].weight += 2;
					}
				}
				else { //위쪽
					if (getGBstate(i, j) == 1) { //상대 돌이면 가중치 -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i + 1][j].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //내 돌이면 가중치 +2
						gameBoard[i - 1][j].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight += 2;
						gameBoard[i + 1][j - 1].weight += 2;
						gameBoard[i + 1][j].weight += 2;
					}
				}
			}
			else if (j == 9) { //밑
				if (i == 0) {//왼쪽 아래 모서리
					if (getGBstate(i, j) == 1) { //상대 돌이면 가중치 -2
						gameBoard[i + 1][j].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //내 돌이면 가중치 +2
						gameBoard[i + 1][j].weight += 2;
						gameBoard[i + 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight += 2;
					}
				}
				else if (i == 9) { //오른쪽 아래 모서리
					if (getGBstate(i, j) == 1) { //상대 돌이면 가중치 -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //내 돌이면 가중치 +2
						gameBoard[i - 1][j].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight += 2;
					}
				}
				else { //밑쪽
					if (getGBstate(i, j) == 1) { //상대 돌이면 가중치 -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i + 1][j].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //내 돌이면 가중치 +2
						gameBoard[i - 1][j].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight += 2;
						gameBoard[i + 1][j].weight += 2;
					}
				}
			}
			else { //가운데 부분
				if (getGBstate(i, j) == 1) { //상대 돌이면 가중치 -2
					gameBoard[i - 1][j - 1].weight -= 2;
					gameBoard[i - 1][j].weight -= 2;
					gameBoard[i - 1][j + 1].weight -= 2;
					gameBoard[i][j - 1].weight -= 2;
					gameBoard[i][j].weight -= 2;
					gameBoard[i][j + 1].weight -= 2;
					gameBoard[i + 1][j - 1].weight -= 2;
					gameBoard[i + 1][j].weight -= 2;
					gameBoard[i + 1][j + 1].weight -= 2;
				}
				if (getGBstate(i, j) == 0) { //내 돌이면 가중치 +2
					gameBoard[i - 1][j - 1].weight += 2;
					gameBoard[i - 1][j].weight += 2;
					gameBoard[i - 1][j + 1].weight += 2;
					gameBoard[i][j - 1].weight += 2;
					gameBoard[i][j].weight += 2;
					gameBoard[i][j + 1].weight += 2;
					gameBoard[i + 1][j - 1].weight += 2;
					gameBoard[i + 1][j].weight += 2;
					gameBoard[i + 1][j + 1].weight += 2;
				}
			}
		}
	}

	for (int j = 0; j < Board_Size; j++) { //바둑돌이 놓여진 곳의 가중치는 고려 안하기 때문
		for (int i = 0; i < Board_Size; i++) {
			if (getGBstate(i, j) != -1 && gameBoard[i][j].weight != 0) {
				setGBweight(i, j, 0);
			}
		}
	}
}
