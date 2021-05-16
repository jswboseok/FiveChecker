#include <iostream>
#include <string>
#include <cstdlib>
#include "location.h"
using namespace std;
#define Board_Size 10
#define MARK 99999

typedef struct spot { //한 지점의 상태값과 가중치
	int state; // 상태값
	int weight; // 가중치
}spot;

class Game_Board {
public:
	Location location; //바둑판 위에서 위치를 잡기 위한 멤버 변수
	spot gameBoard[Board_Size][Board_Size]; //바둑판 선언
	int turn; //1 == 내 차례, 0 == 컴퓨터 차례
public:
	void chakshu(); //한 점에 착수
	void check_closed_4(); //닫힌 4목 확인
	bool check_blanked_4(); //띈 4목 확인
	bool check_blanked_3(); //띈 3목 확인
	bool check_opened_3(); //열린 3목 확인
	bool check_5(); //승리조건 확인
	int getGBdata(int r, int c); //해당 위치의 가중치 또는 착수 여부 불러오기
	void draw(); //오목판 그리기
	void init();
	void calc_weight();
};

void Game_Board::init() {
	for (int i = 0; i < Board_Size; i++) {
		for (int j = 0; j < Board_Size; j++) {
			gameBoard[i][j].state = -1;
			gameBoard[i][j].weight = 0;
		}
	}
	turn = 0;
}

void Game_Board::chakshu() {
	this->calc_weight();
	int r, c;
	
	if (turn) { //내 차례
		cout << "점을 선택하시오 : ";
		cin >> r >> c;
		if (getGBdata(r, c) != -1) {
			gameBoard[r][c].weight = 0;
			cout << "다시 선택하세요" << endl << endl;
		}
		else {
			location.set(r, c);
			gameBoard[r][c].state = turn; //내 돌 놓기
			turn = !turn; //차례 돌리기
		}
	} else { //상대 차례
		check_closed_4();
		check_blanked_4();
		int max = -1, index_i = 0, index_j = 0;
		for (int i = 0; i < Board_Size; i++) {
			for (int j = 0; j < Board_Size; j++) {
				if (gameBoard[i][j].state != -1) { //돌이 있는 곳 혹시 몰라 weight 다시 0
					gameBoard[i][j].weight = 0;
				}
				if (gameBoard[i][j].state == -1 && gameBoard[i][j].weight >= max) { //돌이 없고 가중치가 가장 높은 곳을 찾음
					max = gameBoard[i][j].weight;
					index_i = i;
					index_j = j;
				}
			}
		}
		gameBoard[index_i][index_j].state = turn; //상대돌 놓기
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
			gameBoard[4][i].weight = MARK;
	}
	for (int i = 0; i < Board_Size; i++) { //맨 오른쪽 가로로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[Board_Size - 1 - j][i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[Board_Size - 5][i].weight = MARK;
	}
	for (int j = 0; j < Board_Size; j++) { // 가운데 가로로 닫힌 4 -> 왼쪽에 상대돌
		for (int i = 1; i < Board_Size - 4; i++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i - 1][j].state == 0 && gameBoard[i + 4][j].state == -1 && gameBoard[i + k][j].state == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i + 4][j].weight = MARK;
		}
	}
	for (int j = 0; j < Board_Size; j++) { // 가운데 가로로 닫힌 4 -> 오른쪽에 상대돌
		for (int i = Board_Size - 2; i >= 5; i--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i + 1][j].state == 0 && gameBoard[i - 4][j].state == -1 && gameBoard[i - k][j].state == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i - 4][j].weight = MARK;
		}
	}
	/////////////////////////
	//세로 닫힌 4 체크////
	/////////////////////////
	for (int i = 0; i < Board_Size; i++) { //위쪽 세로로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i][j].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i][4].weight = MARK;
	}
	for (int i = 0; i < Board_Size; i++) { //아래쪽 세로로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i][Board_Size - 1 - j].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i][Board_Size - 5].weight = MARK;
	}
	for (int i = 0; i < Board_Size; i++) { // 가운데 세로로 닫힌 4 -> 위쪽에 상대돌
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i][j - 1].state == 0 && gameBoard[i][j + 4].state == -1 && gameBoard[i][j + k].state == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i][j + 4].weight = MARK;
		}
	}
	for (int i = 0; i < Board_Size; i++) { // 가운데 세로로 닫힌 4 -> 아래쪽에 상대돌
		for (int j = Board_Size - 2; j >= 5; j--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i][j + 1].state == 0 && gameBoard[i][j - 4].state == -1 && gameBoard[i][j - k].state == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i][j - 4].weight = MARK;
		}
	}
	

	///////////////////////////////////////////////////////
	/////왼쪽 위 -> 오른쪽 아래 대각선으로 닫힌 4 체크/////
	///////////////////////////////////////////////////////
	for (int i = 0; i < Board_Size - 4; i++) { //맨 위쪽(j == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i + j][j].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i + 4][4].weight = MARK;
	}
	for (int j = 0; j < Board_Size - 4; j++) { // 맨 오른쪽(i == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (gameBoard[i][j + i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[4][j + 4].weight = MARK;
	}
	for (int j = 4; j < Board_Size; j++) { // 맨 왼쪽(i == Board_Size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (gameBoard[Board_Size - 1 - i][j - i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[Board_Size - 5][j - 4].weight = MARK;
	}
	for (int i = 4; i < Board_Size; i++) { //맨 아래쪽(j == Board_size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i - j][Board_Size - 1 - j].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i - 4][Board_Size - 5].weight = MARK;
	}
	for (int i = 1; i < Board_Size - 4; i++) { //가운데 대각선으로 닫힌 4 -> 위쪽에 돌
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i - 1][j - 1].state == 0 && gameBoard[i + 4][j + 4].state == -1 && gameBoard[i + k][j + k].state == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i + 4][j + 4].weight = MARK;
		}
	}
	for (int i = Board_Size - 2; i > 4; i--) { //가운데 대각선으로 닫힌 4 -> 아래쪽에 돌
		for (int j = Board_Size - 2; j > 4; j--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i + 1][j + 1].state == 0 && gameBoard[i - 4][j - 4].state == -1 && gameBoard[i - k][j - k].state == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i - 4][j - 4].weight = MARK;
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
			gameBoard[i - 4][4].weight = MARK;
	}
	for (int j = 4; j < Board_Size; j++) { // 맨 오른쪽(i == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (gameBoard[j - i][i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[j - 4][4].weight = MARK;
	}
	for (int j = 0; j < Board_Size - 4; j++) { // 맨 왼쪽(i == Board_Size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (gameBoard[Board_Size - 1 - i][j + i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[Board_Size - 5][j + 4].weight = MARK;
	}
	for (int i = 0; i < Board_Size - 4; i++) { //맨 아래쪽(j == Board_size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i + j][Board_Size - 1 - j].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i + 4][Board_Size - 5].weight = MARK;
	}
	for (int i = Board_Size - 2; i > 4; i--) { //가운데 대각선으로 닫힌 4 -> 위쪽에 돌
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i + 1][j - 1].state == 0 && gameBoard[i - 4][j + 4].state == -1 && gameBoard[i - k][j + k].state == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i - 4][j + 4].weight = MARK;
		}
	}
	for (int i = 1; i < Board_Size - 4; i++) { //가운데 대각선으로 닫힌 4 -> 아래쪽에 돌
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i - 1][j + 1].state == 0 && gameBoard[i + 4][j - 4].state == -1 && gameBoard[i + k][j - k].state == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i + 4][j - 4].weight = MARK;
		}
	}
}
bool Game_Board::check_blanked_4() {
	//가로
	for (int j = 0; j < Board_Size; j++) {
		for (int i = 0; i < Board_Size - 4; i++) {
			int blank = 0, cnt = 0, blank_index = -1;
			for (int k = 0; k < 5; k++) {
				if (gameBoard[i + k][j].state == 1)
					cnt++;
				if (i + k != i && i + k != i + 4 && gameBoard[i + k][j].state == -1) {
					blank++;
					blank_index = i + k;
				}
			}
			if (blank == 1 && cnt == 4 && blank_index != -1)
				gameBoard[blank_index][j].weight = MARK;
		}
	}
	//세로
	for (int i = 0; i < Board_Size; i++) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int blank = 0, cnt = 0, blank_index = -1;
			for (int k = 0; k < 5; k++) {
				if (gameBoard[i][j + k].state == 1)
					cnt++;
				if (j + k != j && j + k != j + 4 && gameBoard[i][j + k].state == -1) {
					blank++;
					blank_index = j + k;
				}
			}
			if (blank == 1 && cnt == 4 && blank_index != -1)
				gameBoard[i][blank_index].weight = MARK;
		}
	}
	//왼쪽 위 -> 오른쪽 아래 대각선
	for (int i = 0; i < Board_Size - 4;i++) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int blank = 0, cnt = 0, blank_index_i = -1, blank_index_j = -1;
			for (int k = 0; k < 5; k++) {
				if (gameBoard[i + k][j + k].state == 1)
					cnt++;
				if (gameBoard[i + k][j + k].state == -1) {
					blank++;
					blank_index_i = i + k;
					blank_index_j = j + k;
				}
			}
			if (blank == 1 && cnt == 4)
				gameBoard[blank_index_i][blank_index_j].weight = MARK;
		}
	}
	//오른쪽 위 -> 왼쪽 아래 대각선
	/*for (int i = Board_Size - 1; i > 3; i--) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int blank = 0, cnt = 0, blank_index_i = -1, blank_index_j = -1;
			for (int k = 0; k < 5; k++) {
				if (gameBoard[i - k][j + k].state == 1)
					cnt++;
				if (gameBoard[i - k][j + k].state == -1) {
					blank++;
					blank_index_i = i - k;
					blank_index_j = j + k;
				}
			}
			if (blank == 1 && cnt == 4)
				gameBoard[blank_index_i][blank_index_j].weight = MARK;
		}
	}*/
	
	return false;
}
bool Game_Board::check_blanked_3() {
	return false;
}
bool Game_Board::check_opened_3() {
	return false;
}

bool Game_Board::check_5() {
	//가로 체크
	for (int i = 0; i < Board_Size - 4; i++) {
		for (int j = 0; j < Board_Size; j++) {
			int mycnt = 0, opcnt = 0;
			for (int k = 0; k < 5; k++) {
				if (gameBoard[i + k][j].state == 1) {
					mycnt++;
				}
				else if (gameBoard[i + k][j].state == 0) {
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
				if (gameBoard[i][j + k].state == 1) {
					mycnt++;
				}
				else if (gameBoard[i][j + k].state == 0) {
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
				if (gameBoard[i + k][j + k].state == 1) {
					mycnt++;
				}
				else if (gameBoard[i + k][j + k].state == 0) {
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
				if (gameBoard[i - k][j + k].state == 1) {
					mycnt++;
				}
				else if (gameBoard[i - k][j + k].state == 0) {
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
int Game_Board::getGBdata(int r, int c) {
	return gameBoard[r][c].state;
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
			if (gameBoard[i][j].state == -1) {
				cout << "+ ";
			}
			else if (gameBoard[i][j].state == 0) {
				cout << "X ";
			}
			else if (gameBoard[i][j].state == 1) {
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
			if (gameBoard[i][j].weight < 1000) { //MARK보다 훨씬 작은 수는 MARKING이 안된 것이라는 의미이므로
				gameBoard[i][j].weight = 0;
			}
		}
	}

	for (int j = 0; j < Board_Size; j++) {
		for (int i = 0; i < Board_Size; i++) {
			if (j == 0) { // 위
				if (i == 0) {//왼쪽 위 모서리
					if (gameBoard[i][j].state == 1) { //상대 돌이면 가중치 -2
						gameBoard[i + 1][j].weight -= 2;
						gameBoard[i + 1][j + 1].weight -= 2;
						gameBoard[i][j + 1].weight -= 2;
					}
					if (gameBoard[i][j].state == 0) { //내 돌이면 가중치 +2
						gameBoard[i + 1][j].weight += 2;
						gameBoard[i + 1][j + 1].weight += 2;
						gameBoard[i][j + 1].weight += 2;
					}
				}
				else if (i == 9) { //오른쪽 위
					if (gameBoard[i][j].state == 1) { //상대 돌이면 가중치 -2
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i - 1][j].weight -= 2;
					}
					if (gameBoard[i][j].state == 0) { //내 돌이면 가중치 +2
						gameBoard[i][j - 1].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i - 1][j].weight += 2;
					}
				}
				else { //위쪽
					if (gameBoard[i][j].state == 1) { //상대 돌이면 가중치 -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i + 1][j].weight -= 2;
					}
					if (gameBoard[i][j].state == 0) { //내 돌이면 가중치 +2
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
					if (gameBoard[i][j].state == 1) { //상대 돌이면 가중치 -2
						gameBoard[i + 1][j].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
					}
					if (gameBoard[i][j].state == 0) { //내 돌이면 가중치 +2
						gameBoard[i + 1][j].weight += 2;
						gameBoard[i + 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight += 2;
					}
				}
				else if (i == 9) { //오른쪽 아래 모서리
					if (gameBoard[i][j].state == 1) { //상대 돌이면 가중치 -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
					}
					if (gameBoard[i][j].state == 0) { //내 돌이면 가중치 +2
						gameBoard[i - 1][j].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight += 2;
					}
				}
				else { //밑쪽
					if (gameBoard[i][j].state == 1) { //상대 돌이면 가중치 -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i + 1][j].weight -= 2;
					}
					if (gameBoard[i][j].state == 0) { //내 돌이면 가중치 +2
						gameBoard[i - 1][j].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight += 2;
						gameBoard[i + 1][j].weight += 2;
					}
				}
			}
			else { //가운데 부분
				if (gameBoard[i][j].state == 1) { //상대 돌이면 가중치 -2
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
				if (gameBoard[i][j].state == 0) { //내 돌이면 가중치 +2
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
			if (gameBoard[i][j].state != -1 && gameBoard[i][j].weight != 0) {
				gameBoard[i][j].weight = 0;
			}
		}
	}
}