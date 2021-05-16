#include <iostream>
#include <string>
#include <cstdlib>
#include "location.h"
using namespace std;
#define Board_Size 10
#define MARK 99999

typedef struct spot { //�� ������ ���°��� ����ġ
	int state; // ���°�
	int weight; // ����ġ
}spot;

class Game_Board {
public:
	Location location; //�ٵ��� ������ ��ġ�� ��� ���� ��� ����
	spot gameBoard[Board_Size][Board_Size]; //�ٵ��� ����
	int turn; //1 == �� ����, 0 == ��ǻ�� ����
public:
	void chakshu(); //�� ���� ����
	void check_closed_4(); //���� 4�� Ȯ��
	bool check_blanked_4(); //�� 4�� Ȯ��
	bool check_blanked_3(); //�� 3�� Ȯ��
	bool check_opened_3(); //���� 3�� Ȯ��
	bool check_5(); //�¸����� Ȯ��
	int getGBstate(int r, int c); //�� spot�� state �ҷ���
	int getGBweight(int r, int c); //�� spot�� ����ġ �ҷ���
	void draw(); //������ �׸���
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
	
	if (turn) { //�� ����
		cout << "���� �����Ͻÿ� : ";
		cin >> r >> c;
		if (getGBdata(r, c) != -1) {
			gameBoard[r][c].weight = 0;
			cout << "�ٽ� �����ϼ���" << endl << endl;
		}
		else {
			location.set(r, c);
			gameBoard[r][c].state = turn; //�� �� ����
			turn = !turn; //���� ������
		}
	} else { //��� ����
		check_closed_4();
		check_blanked_4();
		check_opened_3();
		int max = -1, index_i = 0, index_j = 0;
		for (int i = 0; i < Board_Size; i++) {
			for (int j = 0; j < Board_Size; j++) {
				if (getGBstate(i, j) != -1) { //���� �ִ� �� Ȥ�� ���� weight �ٽ� 0
					gameBoard[i][j].weight = 0;
				}
				if (getGBstate(i, j) == -1 && getGBweight(i, j) >= max) { //���� ���� ����ġ�� ���� ���� ���� ã��
					max = gameBoard[i][j].weight;
					index_i = i;
					index_j = j;
				}
			}
		}
		gameBoard[index_i][index_j].state = turn; //��뵹 ����
		turn = !turn; //���� ������
	}
}
void Game_Board::check_closed_4() {
	//////////////////////
	//���� ���� 4 üũ////
	//////////////////////
	for (int i = 0; i < Board_Size; i++) { //�� ���� ���η� ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[j][i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[4][i].weight = MARK;
	}
	for (int i = 0; i < Board_Size; i++) { //�� ������ ���η� ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[Board_Size - 1 - j][i].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[Board_Size - 5][i].weight = MARK;
	}
	for (int j = 0; j < Board_Size; j++) { // ��� ���η� ���� 4 -> ���ʿ� ��뵹
		for (int i = 1; i < Board_Size - 4; i++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i - 1][j].state == 0 && gameBoard[i + 4][j].state == -1 && getGBstate(i + k, j) == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i + 4][j].weight = MARK;
		}
	}
	for (int j = 0; j < Board_Size; j++) { // ��� ���η� ���� 4 -> �����ʿ� ��뵹
		for (int i = Board_Size - 2; i >= 5; i--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (gameBoard[i + 1][j].state == 0 && gameBoard[i - 4][j].state == -1 && getGBstate(i - k, j) == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i - 4][j].weight = MARK;
		}
	}
	/////////////////////////
	//���� ���� 4 üũ////
	/////////////////////////
	for (int i = 0; i < Board_Size; i++) { //���� ���η� ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i, j) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i][4].weight = MARK;
	}
	for (int i = 0; i < Board_Size; i++) { //�Ʒ��� ���η� ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i, Board_Size - 1 - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i][Board_Size - 5].weight = MARK;
	}
	for (int i = 0; i < Board_Size; i++) { // ��� ���η� ���� 4 -> ���ʿ� ��뵹
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i, j - 1) == 0 && getGBstate(i,j + 4) == -1 && getGBstate(i, j + k) == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i][j + 4].weight = MARK;
		}
	}
	for (int i = 0; i < Board_Size; i++) { // ��� ���η� ���� 4 -> �Ʒ��ʿ� ��뵹
		for (int j = Board_Size - 2; j >= 5; j--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i, j + 1) == 0 && getGBstate(i, j - 4) == -1 && getGBstate(i, j - k) == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i][j - 4].weight = MARK;
		}
	}
	

	///////////////////////////////////////////////////////
	/////���� �� -> ������ �Ʒ� �밢������ ���� 4 üũ/////
	///////////////////////////////////////////////////////
	for (int i = 0; i < Board_Size - 4; i++) { //�� ����(j == 0) �밢������ ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i + j, j) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i + 4][4].weight = MARK;
	}
	for (int j = 0; j < Board_Size - 4; j++) { // �� ������(i == 0) �밢������ ���� 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(i, j + i) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[4][j + 4].weight = MARK;
	}
	for (int j = 4; j < Board_Size; j++) { // �� ����(i == Board_Size) �밢������ ���� 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(Board_Size - 1 - i, j - i) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[Board_Size - 5][j - 4].weight = MARK;
	}
	for (int i = 4; i < Board_Size; i++) { //�� �Ʒ���(j == Board_size) �밢������ ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i - j, Board_Size - 1 - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i - 4][Board_Size - 5].weight = MARK;
	}
	for (int i = 1; i < Board_Size - 4; i++) { //��� �밢������ ���� 4 -> ���ʿ� ��
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i - 1, j - 1) == 0 && getGBstate(i + 4, j + 4) == -1 && getGBstate(i + k, j + k) == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i + 4][j + 4].weight = MARK;
		}
	}
	for (int i = Board_Size - 2; i > 4; i--) { //��� �밢������ ���� 4 -> �Ʒ��ʿ� ��
		for (int j = Board_Size - 2; j > 4; j--) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + 1, j + 1) == 0 && getGBstate(i - 4, j - 4) == -1 && getGBstate(i - k, j - k) == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i - 4][j - 4].weight = MARK;
		}
	}
	//////////////////////////////////////////////////////////////
	///////������ �� -> ���� �Ʒ� �밢������ ���� 4 üũ//////////
	//////////////////////////////////////////////////////////////
	for (int i = 4; i < Board_Size; i++) { //�� ����(j == 0) �밢������ ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i - j][j].state == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i - 4][4].weight = MARK;
	}
	for (int j = 4; j < Board_Size; j++) { // �� ������(i == 0) �밢������ ���� 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(j - i, i) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[j - 4][4].weight = MARK;
	}
	for (int j = 0; j < Board_Size - 4; j++) { // �� ����(i == Board_Size) �밢������ ���� 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(Board_Size - 1 - i, j + i) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[Board_Size - 5][j + 4].weight = MARK;
	}
	for (int i = 0; i < Board_Size - 4; i++) { //�� �Ʒ���(j == Board_size) �밢������ ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i + j, Board_Size - 1 - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			gameBoard[i + 4][Board_Size - 5].weight = MARK;
	}
	for (int i = Board_Size - 2; i > 4; i--) { //��� �밢������ ���� 4 -> ���ʿ� ��
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + 1, j - 1) == 0 && getGBstate(i - 4, j + 4) == -1 && getGBstate(i - k, j + k) == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i - 4][j + 4].weight = MARK;
		}
	}
	for (int i = 1; i < Board_Size - 4; i++) { //��� �밢������ ���� 4 -> �Ʒ��ʿ� ��
		for (int j = 1; j < Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i - 1, j + 1) == 0 && getGBstate(i + 4, j - 4) == -1 && getGBstate(i + k, j - k) == 1)
					cnt++;
			}
			if (cnt == 4)
				gameBoard[i + 4][j - 4].weight = MARK;
		}
	}
}
bool Game_Board::check_blanked_4() {
	//����
	for (int j = 0; j < Board_Size; j++) {
		for (int i = 0; i < Board_Size - 4; i++) {
			int blank = 0, cnt = 0, blank_index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j) == 1)
					cnt++;
				if ((i + k != i || i + k != i + 4) && getGBstate(i + k, j) == -1) {
					blank++;
					blank_index = i + k;
				}
			}
			if (blank == 1 && cnt == 4 && blank_index != -1)
				gameBoard[blank_index][j].weight = MARK;
		}
	}
	//����
	for (int i = 0; i < Board_Size; i++) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int blank = 0, cnt = 0, blank_index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i, j + k) == 1)
					cnt++;
				if ((j + k != j || j + k != j + 4) && getGBstate(i, j + k) == -1) {
					blank++;
					blank_index = j + k;
				}
			}
			if (blank == 1 && cnt == 4 && blank_index != -1)
				gameBoard[i][blank_index].weight = MARK;
		}
	}
	//���� �� -> ������ �Ʒ� �밢��
	for (int i = 0; i < Board_Size - 4;i++) {
		for (int j = 0; j < Board_Size - 4; j++) {
			int blank = 0, cnt = 0, blank_index_i = -1, blank_index_j = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j + k) == 1)
					cnt++;
				if (getGBstate(i + k, j + k) == -1) {
					blank++;
					blank_index_i = i + k;
					blank_index_j = j + k;
				}
			}
			if (blank == 1 && cnt == 4)
				gameBoard[blank_index_i][blank_index_j].weight = MARK;
		}
	}
	//������ �� -> ���� �Ʒ� �밢��
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
				gameBoard[blank_index_i][blank_index_j].weight = MARK;
		}
	}
	
	return false;
}
bool Game_Board::check_blanked_3() {
	return false;
}
bool Game_Board::check_opened_3() {
	//����
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
						gameBoard[i + 3][j].weight = MARK;
					else
						gameBoard[i - 1][j].weight = MARK;
				}
			}
		}
	}
	//����
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
						gameBoard[i][j + 3].weight = MARK;
					else
						gameBoard[i][j - 1].weight = MARK;
				}
			}
		}
	}
	//�밢��1
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
						gameBoard[i + 3][j + 3].weight = MARK;
					else
						gameBoard[i - 1][j - 1].weight = MARK;
				}
			}
		}
	}
	//�밢��2
	for (int i = Board_Size - 1; i > 3; i--) {
		for (int j = 1; j < Board_Size - 3; j++) {
			if (getGBstate(i + 1, j - 1) == -1 && getGBstate(i - 3, j + 3) == -1) {
				int cnt = 0;
				for (int k = 0; k < 3; k++) {
					if (getGBstate(i - k, j + k) == 1)
						cnt++;
				}
				if (cnt == 3) {
					if (i < Board_Size / 2)
						gameBoard[i - 3][j + 3].weight = MARK;
					else
						gameBoard[i + 1][j - 1].weight = MARK;
				}
			}
		}
	}
	return false;
}

bool Game_Board::check_5() {
	//���� üũ
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
				cout << "���� �̱�" << endl;
				return true;
			}
			else if (opcnt == 5) {
				cout << "���� ��" << endl;
				return true;
			}
		}
	}
	//���� üũ
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
				cout << "���� �̱�" << endl;
				return true;
			}
			else if (opcnt == 5) {
				cout << "���� ��" << endl;
				return true;
			}
		}
	}
	//���� �� -> ������ �Ʒ� �밢�� üũ
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
				cout << "���� �̱�" << endl;
				return true;
			}
			else if (opcnt == 5) {
				cout << "���� ��" << endl;
				return true;
			}
		}
	}
	//������ �� -> ���� �Ʒ� �밢�� üũ
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
				cout << "���� �̱�" << endl;
				return true;
			}
			else if (opcnt == 5) {
				cout << "���� ��" << endl;
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

void Game_Board::calc_weight() { //����ġ ���ϱ�
	for (int i = 0; i < Board_Size; i++) { //weight �ʱ�ȭ
		for (int j = 0; j < Board_Size; j++) {
			if (gameBoard[i][j].weight < 1000) { //MARK���� �ξ� ���� ���� MARKING�� �ȵ� ���̶�� �ǹ��̹Ƿ�
				gameBoard[i][j].weight = 0;
			}
		}
	}

	for (int j = 0; j < Board_Size; j++) {
		for (int i = 0; i < Board_Size; i++) {
			if (j == 0) { // ��
				if (i == 0) {//���� �� �𼭸�
					if (getGBstate(i, j) == 1) { //��� ���̸� ����ġ -2
						gameBoard[i + 1][j].weight -= 2;
						gameBoard[i + 1][j + 1].weight -= 2;
						gameBoard[i][j + 1].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //�� ���̸� ����ġ +2
						gameBoard[i + 1][j].weight += 2;
						gameBoard[i + 1][j + 1].weight += 2;
						gameBoard[i][j + 1].weight += 2;
					}
				}
				else if (i == 9) { //������ ��
					if (getGBstate(i, j) == 1) { //��� ���̸� ����ġ -2
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i - 1][j].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //�� ���̸� ����ġ +2
						gameBoard[i][j - 1].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i - 1][j].weight += 2;
					}
				}
				else { //����
					if (getGBstate(i, j) == 1) { //��� ���̸� ����ġ -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i + 1][j].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //�� ���̸� ����ġ +2
						gameBoard[i - 1][j].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight += 2;
						gameBoard[i + 1][j - 1].weight += 2;
						gameBoard[i + 1][j].weight += 2;
					}
				}
			}
			else if (j == 9) { //��
				if (i == 0) {//���� �Ʒ� �𼭸�
					if (getGBstate(i, j) == 1) { //��� ���̸� ����ġ -2
						gameBoard[i + 1][j].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //�� ���̸� ����ġ +2
						gameBoard[i + 1][j].weight += 2;
						gameBoard[i + 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight += 2;
					}
				}
				else if (i == 9) { //������ �Ʒ� �𼭸�
					if (getGBstate(i, j) == 1) { //��� ���̸� ����ġ -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //�� ���̸� ����ġ +2
						gameBoard[i - 1][j].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight += 2;
					}
				}
				else { //����
					if (getGBstate(i, j) == 1) { //��� ���̸� ����ġ -2
						gameBoard[i - 1][j].weight -= 2;
						gameBoard[i - 1][j - 1].weight -= 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight -= 2;
						gameBoard[i + 1][j].weight -= 2;
					}
					if (getGBstate(i, j) == 0) { //�� ���̸� ����ġ +2
						gameBoard[i - 1][j].weight += 2;
						gameBoard[i - 1][j - 1].weight += 2;
						gameBoard[i][j - 1].weight -= 2;
						gameBoard[i + 1][j - 1].weight += 2;
						gameBoard[i + 1][j].weight += 2;
					}
				}
			}
			else { //��� �κ�
				if (getGBstate(i, j) == 1) { //��� ���̸� ����ġ -2
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
				if (getGBstate(i, j) == 0) { //�� ���̸� ����ġ +2
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

	for (int j = 0; j < Board_Size; j++) { //�ٵϵ��� ������ ���� ����ġ�� ��� ���ϱ� ����
		for (int i = 0; i < Board_Size; i++) {
			if (getGBstate(i, j) != -1 && gameBoard[i][j].weight != 0) {
				gameBoard[i][j].weight = 0;
			}
		}
	}
}