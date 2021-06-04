//2016112109 �弮��
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
#define Board_Size 10
/*  ��ǻ�Ͱ� ���ƾ� �ϴ� �κ��� ��ŷ
    �߿䵵�� ���� mark�� �������� 
    �߿��ϸ� weight�� ũ�� �ξ� �� ���� ��ǻ�Ͱ� ���� �ΰ� ��  */
#define MARK2 9999
#define MARK3 99999 
#define MARK4 999999 
#define WIN_MARK 9999999 


typedef struct spot { //�� ������ ���°��� ����ġ
	int state; // ���°�
	int weight; // ����ġ
}spot;

class Game_Board {
public:
	//Location location; //�ٵ��� ������ ��ġ�� ��� ���� ��� ����
	spot gameBoard[Board_Size + 1][Board_Size + 1]; //�ٵ��� ����
	int turn; //1 == �� ����, 0 == ��ǻ�� ����
public:
	void chakshu(); //�� ���� ����

	void check_opened_3(); //���� 3�� Ȯ��
	void check_blanked_3(); //�� 3�� Ȯ��
	void check_closed_4(); //���� 4�� Ȯ��
	void check_blanked_4(); //�� 4�� Ȯ��
	void check_win();
	
	bool check_5(); //�¸����� Ȯ��
	int getGBstate(int r, int c); //�� ������ state �ҷ���
	int getGBweight(int r, int c); //�� ������ ����ġ �ҷ���
	void setGBstate(int r, int c, int value); //�� ������ state ����
	void setGBweight(int r, int c, int value); //�� ������ ����ġ ����
	void draw(); //������ �׸���
	void init(int); //������ �ʱ�ȭ
	void calc_weight(); // weight ���, �̸� ���� ��ǻ�Ͱ� �ùٸ� ���� ���� �� �ֵ��� ��
};

/*ó�� ���Ӻ��带 �ʱ�ȭ�ϴ� �Լ�. ��� ���� state�� -1, weight�� 0���� �ʱ�ȭ�Ѵ�.*/ 
void Game_Board::init(int select) {
	for (int i = 1; i < Board_Size + 1; i++) {
		for (int j = 1; j < Board_Size + 1; j++) {
			setGBstate(i, j, -1);
			setGBweight(i, j, 0);
		}
	}
	turn = select; //����ڿ��� �Է��� �ް� �����İ��� ���Ѵ�.
	int i = rand() % Board_Size + 1; //���� ������� �ε����� 1���� �����ϱ� ���� Board_Size + 1�� �Ͽ���.
	int j = rand() % Board_Size + 1;
	setGBweight(i, j, MARK3);
}

//���� ���� �Լ� ,�� �����ǿ� state�� ���ϴ� �Լ�
void Game_Board::chakshu() {
	int r, c; // row, col
	
	if (turn == 1) { //�� ����
		//cout << "���� �����Ͻÿ� : ";
		while (true) {
			int row, col;
			cout << "�Է����ּ���(1���� �ٵ��� ũ�����) >> ";
			cin >> row >> col;
			if (row <= 0 || row > Board_Size || col <= 0 || col > Board_Size) //�ٵ����� ������ ����� �ٽ� �Է�
				continue;
			else {
				r = row; //�Է¹��� ������ ����
				c = col;
				break;
			}
		}
		
		if (getGBstate(r, c) != -1) { //���� �Է��� ���� �̹� ���� �ִٸ� �ٽ� �Է�
			setGBweight(r, c, 0);
			cout << "�ٽ� �����ϼ���" << endl << endl;
		}
		else {
			setGBstate(r, c, turn); //�� �� ����, state == 1
			turn = 0; // ��ǻ�ͷ� ���� ������
		}
	} 
	else { //��� ����
		check_blanked_3(); // �� 3������ Ȯ��
		check_closed_4();  // ���� 4������ Ȯ��
		check_opened_3();  //���� 3������ Ȯ��
		check_blanked_4(); //�� 4������ Ȯ��
		calc_weight();     //����ġ ���
		int max = -1, index_i = 0, index_j = 0;
		for (int i = 1; i <= Board_Size; i++) {
			for (int j = 1; j <= Board_Size; j++) {
				if (getGBstate(i, j) != -1) { //���� �ִ� �� Ȥ�� ���� weight �ٽ� 0
					setGBweight(i, j, 0);
				}
				if (getGBstate(i, j) == -1 && getGBweight(i, j) >= max) { //���� ���� ����ġ�� ���� ���� ���� ã��
					max = gameBoard[i][j].weight;
					index_i = i;
					index_j = j;
				}
			}
		}
		setGBstate(index_i, index_j, turn); //��ǻ�͵� ����, state == 0
		turn = 1; //���� ������
	}
}

/* ���� 4�� �Լ� */
void Game_Board::check_closed_4() {
	//////////////////////
	//���� ���� 4 üũ////
	//////////////////////
	for (int i = 1; i <= Board_Size; i++) { //�� ���� ���η� ���� 4
		int cnt = 0;
		for (int j = 1; j <= 4; j++) {
			if (gameBoard[j][i].state == 1)
				cnt++;
		}
		if (cnt == 4) //cnt�� 4��� ���� �������� ���� 4���� �������ٴ� ��. ���� ���� Mark4�� ǥ����
			setGBweight(5, i, MARK4);
	}
	for (int i = 1; i <= Board_Size; i++) { //�� ������ ���η� ���� 4
		int cnt = 0;
		for (int j = 1; j <= 4; j++) {
			if (gameBoard[Board_Size - 1 - j][i].state == 1) //���� ���� ������ cnt++
				cnt++;
		}
		if (cnt == 4) //cnt�� 4��� ���� �������� ���� 4���� �������ٴ� ��. ���� ���� Mark4�� ǥ����
			setGBweight(Board_Size - 4, i, MARK4);
	}
	for (int j = 1; j <= Board_Size; j++) { // ��� ���η� ���� 4
		for (int i = 2; i < Board_Size - 3; i++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j) == 1)
					cnt++;
			}
			if (cnt == 4) {
				if (gameBoard[i - 1][j].state == 0 && gameBoard[i + 4][j].state == -1) /*���� �������� ���Ұ�, ������ ���������, ������ ���ƾߵ�*/
					setGBweight(i + 4, j, MARK4);
				else if (gameBoard[i - 1][j].state == -1 && gameBoard[i + 4][j].state == 0)  /*���� ������ ���Ұ�, �������� ���������, �������� ���ƾߵ�*/
					setGBweight(i - 1, j, MARK4);
			}
				
		}
	}
	///////////////////////////
	//// ���� ���� 4 üũ  ////
	///////////////////////////
	for (int i = 1; i <= Board_Size; i++) { //���� ���η� ���� 4
		int cnt = 0;
		for (int j = 1; j <= 4; j++) {
			if (getGBstate(i, j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i, 5, MARK4);
	}
	for (int i = 1; i <= Board_Size; i++) { //�Ʒ��� ���η� ���� 4
		int cnt = 0;
		for (int j = Board_Size; j < Board_Size - 4; j--) {
			if (getGBstate(i, j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i, Board_Size - 4, MARK4);
	}
	for (int i = 1; i <= Board_Size; i++) { // ��� ���η� ���� 4 -> ���ʿ� ��뵹
		for (int j = 2; j <= Board_Size - 3; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i, j + k) == 1)
					cnt++;
			}
			if (cnt == 4) {
				if(getGBstate(i, j - 1) == 0 && getGBstate(i, j + 4) == -1) //������ ���Ұ�, �Ʒ����� ������� �Ʒ��� ���ƾ� ��
					setGBweight(i, j + 4, MARK4); 
				else if(getGBstate(i, j - 1) == -1 && getGBstate(i, j + 4) == 0) //�Ʒ����� ���� ������ ������� ���� ���ƾ� ��
					setGBweight(i, j - 1, MARK4);
			}
				
		}
	}
	

	///////////////////////////////////////////////////////
	/////���� �� -> ������ �Ʒ� �밢������ ���� 4 üũ/////
	///////////////////////////////////////////////////////
	for (int i = 1; i <= Board_Size - 4; i++) { //�� ����(j == 0) �밢������ ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i + j, j + 1) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i + 4, 5, MARK4);
	}
	for (int j = 1; j <= Board_Size - 4; j++) { // �� ������(i == 0) �밢������ ���� 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(i + 1, j + i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(5, j + 4, MARK4);
	}
	for (int j = 5; j <= Board_Size; j++) { // �� ����(i == Board_Size) �밢������ ���� 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(Board_Size - i, j - i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(Board_Size - 4, j - 4, MARK4);
	}
	for (int i = 5; i <= Board_Size; i++) { //�� �Ʒ���(j == Board_size) �밢������ ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i - j, Board_Size  - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i - 4, Board_Size - 4, MARK4);
	}
	for (int i = 2; i <= Board_Size - 4; i++) { //��� �밢������ ���� 4 -> ���ʿ� ��
		for (int j = 2; j <= Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j + k) == 1)
					cnt++;
			}
			if (cnt == 4) {
				if(getGBstate(i - 1, j - 1) == 0 && getGBstate(i + 4, j + 4) == -1) //������ ���� �Ʒ����� ������� �Ʒ����� ���ƾ� ��
					setGBweight(i + 4, j + 4, MARK4);
				else if(getGBstate(i - 1, j - 1) == -1 && getGBstate(i + 4, j + 4) == 0) //�ݴ�� ������ ������� ���� ���ƾ� ��
					setGBweight(i - 1, j - 1, MARK4);
			}
		}
	}
	//////////////////////////////////////////////////////////////
	///////������ �� -> ���� �Ʒ� �밢������ ���� 4 üũ//////////
	//////////////////////////////////////////////////////////////
	for (int i = 5; i <= Board_Size; i++) { //�� ����(j == 0) �밢������ ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i - j][j + 1].state == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i - 4, 5, MARK4);
	}
	for (int j = 5; j <= Board_Size; j++) { // �� ����(i == 0) �밢������ ���� 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(i + 1, j - i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(5, j - 4, MARK4);
	}
	for (int j = 1; j <= Board_Size - 4; j++) { // �� ������(i == Board_Size) �밢������ ���� 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(Board_Size - i, j + i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(Board_Size - 4, j + 4, MARK4);
	}
	for (int i = 1; i <= Board_Size - 4; i++) { //�� �Ʒ���(j == Board_size) �밢������ ���� 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i + j, Board_Size - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i + 4, Board_Size - 4, MARK4);
	}
	for (int i = 5; i <= Board_Size - 1; i++) { //��� �밢������ ���� 4 -> ���ʿ� ��
		for (int j = 2; j <= Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i - k, j + k) == 1)
					cnt++;
			}
			if (cnt == 4) {
				if (getGBstate(i + 1, j - 1) == 0 && getGBstate(i - 4, j + 4) == -1) //������ ���� �Ʒ����� ������� �Ʒ����� ���ƾ� ��
					setGBweight(i - 4, j + 4, MARK4);
				else if(getGBstate(i + 1, j - 1) == -1 && getGBstate(i - 4, j + 4) == 0) //�ݴ�� ������ ������� ���� ���ƾ� ��
					setGBweight(i + 1, j - 1, MARK4);
			}
		}
	}
}

/* �� 4�� �Լ�, �������� �� ������ ���ӵ� 5������ �˻��Ѵ�. 
  ���� �� ������ 1�̰�, ���� ���� ���� 4���̸� �� 4���̶�� ���̴�. */
void Game_Board::check_blanked_4() {
	//����
	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size - 4; i++) {
			int blank = 0, cnt = 0, index = -1; /* blank : ����ִ� ������ ��, cnt : ���� �� ����, index = ����ִ� ������ ��ġ*/
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j) == 1) //
					cnt++;
				if ((i + k != i || i + k != i + 4) && getGBstate(i + k, j) == -1) { //�ٵ����� ������ �� �����, �ش� ��ġ�� ��������� blank++
					blank++;
					index = i + k;
				}
			}
			if (blank == 1 && cnt == 4 && index != -1) //�� ������ 1�̰�, ���� ���� 4���̸� �� 4���̶�� �ǹ��̴�. ����ġ�� �����Ѵ�.
				setGBweight(index, j, MARK4);
		}
	}
	//����
	for (int i = 1; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i, j + k) == 1)
					cnt++;
				if ((j + k != j || j + k != j + 4) && getGBstate(i, j + k) == -1) { //��������
					blank++;
					index = j + k;
				}
			}
			if (blank == 1 && cnt == 4 && index != -1)
				setGBweight(i, index, MARK4);
		}
	}
	//���� �� -> ������ �Ʒ� �밢��
	for (int i = 1; i <= Board_Size - 4;i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j + k) == 1)  //��������
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
	//������ �� -> ���� �Ʒ� �밢��
	for (int i = Board_Size; i > 4; i--) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i - k, j + k) == 1)  //��������
					cnt++;
				if (getGBstate(i - k, j + k) == -1) {
					blank++;
					index = k;
				}
			}
			if (blank == 1 && cnt == 4)
				setGBweight(i - index, j + index, MARK4);
		}
	}
} 

/*   �� 3�� �Լ�, ����� �� 4�� �Լ��� ����. �ٸ� ���ӵ� 4 ������ üũ�Ѵ�. */
void Game_Board::check_blanked_3() {
	//����
	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size - 3; i++) {
			if (getGBstate(i, j) == 1 && getGBstate(i + 3, j) == 1) {
				int cnt = 0, blank = 0, index = i;
				for (int k = 1; k < 3; k++) {
					if (getGBstate(i + k, j) == 1)
						cnt++;
					if (getGBstate(i + k, j) == -1) {
						blank++;
						index = i + k;
					}
				}
				if (blank == 1 && cnt == 1)
					setGBweight(index, j, MARK3);
			}
		}
	}
	//����
	for (int i = 1; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 3; j++) {
			if (getGBstate(i, j) == 1 && getGBstate(i, j + 3) == 1) {
				int cnt = 0, blank = 0, index = i;
				for (int k = 1; k < 3; k++) {
					if (getGBstate(i, j + k) == 1)
						cnt++;
					if (getGBstate(i, j + k) == -1) {
						blank++;
						index = j + k;
					}
				}
				if (blank == 1 && cnt == 1)
					setGBweight(i, index, MARK3);
			}
		}
	}
	//���� �� -> ������ �Ʒ� �밢��
	for (int i = 1; i <= Board_Size - 3; i++) {
		for (int j = 1; j <= Board_Size - 3; j++) {
			if (getGBstate(i, j) == 1 && getGBstate(i + 3, j + 3) == 1) {
				int cnt = 0, blank = 0, index = i;
				for (int k = 1; k < 3; k++) {
					if (getGBstate(i + k, j + k) == 1)
						cnt++;
					if (getGBstate(i + k, j + k) == -1) {
						blank++;
						index = k;
					}
				}
				if (blank == 1 && cnt == 1)
					setGBweight(i + index, j + index, MARK3);
			}
		}
	}
	//������ �� -> ���� �Ʒ� �밢�� üũ
	for (int i = 4; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 3; j++) {
			if (getGBstate(i, j) == 1 && getGBstate(i - 3, j + 3) == 1) {
				int cnt = 0, blank = 0, index = i;
				for (int k = 1; k < 3; k++) {
					if (getGBstate(i - k, j + k) == 1)
						cnt++;
					if (getGBstate(i - k, j + k) == -1) {
						blank++;
						index = k;
					}
				}
				if (blank == 1 && cnt == 1)
					setGBweight(i - index, j + index, MARK3);
			}
		}
	}
}

/* ���� 3�� �Լ�, �ٵ����� ������ ���ӵ� 5���� ������ �˻��Ѵ�. 
   �� ���� ����ְ�, ��� �������� ���� ���� 3�� ������ �� 3���̶�� ���̴�.*/
void Game_Board::check_opened_3() {
	//����
	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size - 4; i++) {
			int index = i, front = 0, end = 0, cnt = 0; //index == , front : �� ���� ������� Ȯ��, end : �� ���� ������� Ȯ��
			if (getGBstate(i, j) == -1) //�� ���� ������� Ȯ��
				front++;
			if (getGBstate(i + 4, j) == -1) //�� ���� ������� Ȯ��
				end++;
			for (int k = 1; k < 4; k++) {
				if (getGBstate(i + k, j) == 1) //���� ���� ������ ������ cnt++
					cnt++;
			}
			if (front == 1 && end == 1 && cnt == 3) { //���� ���� 3���̸�
				if (front > Board_Size / 2) setGBweight(i, j, MARK3); //�� �����ְ� ���� ���� ���� ����� ���� �����Ѵ�.
				else setGBweight(i + 4, j, MARK3);
			}
		}
	}
	//���� -> �������� ,�ٸ� �� ���� front�� ���� end�� �Ʒ��̴�.
	for (int i = 1; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int index = i, front = 0, end = 0, cnt = 0;
			if (getGBstate(i, j) == -1)
				front++;
			if (getGBstate(i, j + 4) == -1)
				end++;
			for (int k = 1; k < 4; k++) {
				if (getGBstate(i, j + k) == 1)
					cnt++;
			}
			if (front == 1 && end == 1 && cnt == 3) {
				if (front > Board_Size / 2) setGBweight(i, j, MARK3);
				else setGBweight(i, j + 4, MARK3);
			}
		}
	}
	//���� �� -> ������ �Ʒ� �밢��
	for (int i = 1; i <= Board_Size - 4; i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int index = i, front = 0, end = 0, cnt = 0;
			if (getGBstate(i, j) == -1)
				front++;
			if (getGBstate(i + 4, j + 4) == -1)
				end++;
			for (int k = 1; k < 4; k++) {
				if (getGBstate(i + k, j + k) == 1)
					cnt++;
			}
			if (front == 1 && end == 1 && cnt == 3) {
				if (front > Board_Size / 2) setGBweight(i, j, MARK3);
				else setGBweight(i + 4, j + 4, MARK3);
			}
		}
	}
	//������ �� -> ���� �Ʒ� �밢�� üũ
	for (int i = 5; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int index = i, front = 0, end = 0, cnt = 0;
			if (getGBstate(i, j) == -1)
				front++;
			if (getGBstate(i - 4, j + 4) == -1)
				end++;
			for (int k = 1; k < 4; k++) {
				if (getGBstate(i - k, j + k) == 1)
					cnt++;
			}
			if (front == 1 && end == 1 && cnt == 3) {
				if (front > Board_Size / 2) setGBweight(i, j, MARK3);
				else setGBweight(i - 4, j + 4, MARK3);
			}
		}
	}
}

//������ �ϼ��Ǿ����� Ȯ���Ѵ�, �ٵ����� ������ ���ӵ� 5������ �˻��Ѵ�.
bool Game_Board::check_5() {
	//���� üũ
	for (int i = 1; i <= Board_Size - 4; i++) {
		for (int j = 1; j <= Board_Size; j++) {
			int mycnt = 0, opcnt = 0; 
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j) == 1) { //���� ���� ������ mycnt++
					mycnt++;
				}
				else if (getGBstate(i + k, j) == 0) { //��ǻ���� ���� ������ ������ opcnt++
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
	for (int i = 1; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
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
	for (int i = 1; i <= Board_Size - 4; i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
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
	for (int i = Board_Size; i >= 5; i--) {
		for (int j = 1; j <= Board_Size - 4; j++) {
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

//�ٵ����� row�� col�� ��ġ�� �ִ� state�� weight�� ���� �ҷ����ų� ���� ���Ѵ�.
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

//�ٵ����� �׸��� �Լ�
void Game_Board::draw() {
	calc_weight();
	cout << endl;
	for (int i = 0; i <= Board_Size; i++) {
		if (i == 0)
			cout << "      ";
		else
			cout << i << " ";
	}
	cout << endl;
	for (int i = 0; i <= Board_Size + 2; i++)
		cout << "==";
	cout << endl;

	for (int j = 1; j <= Board_Size; j++) {
		if(j >= 100)
			cout << j << " | ";
		else if (j < 100 && j >= 10) 
			cout << j << "  | ";
		else
			cout << j << "   | ";
		
		for (int i = 1; i <= Board_Size; i++) {
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
	for (int i = 1; i <= Board_Size; i++) { //weight �ʱ�ȭ
		for (int j = 1; j <= Board_Size; j++) {
			if (getGBweight(i, j) < 1000) { //MARK���� �ξ� ���� ���� MARKING�� �ȵ� ���̶�� �ǹ��̹Ƿ�
				setGBweight(i, j, 0);
			}
		}
	}

	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size; i++) {
			if (j == 1) { // ��
				if (i == 1) {//���� �� �𼭸�
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
				else if (i == Board_Size) { //������ ��
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
			else if (j == Board_Size) { //��
				if (i == 1) {//���� �Ʒ� �𼭸�
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
				else if (i == Board_Size) { //������ �Ʒ� �𼭸�
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

	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size; i++) {
			int cnt = 0;
			if (getGBstate(i, j) == 0) {
				//1. ���η� ���� ���� ����ġ ���ϱ�
				int k = 0;
				while (i + k <= Board_Size || getGBstate(i + k, j) != 0) {
					cnt++;
					k++;
				}
				if (i + k > Board_Size) { //i + k�� �� �κ��� ��
					if(getGBstate(i - 1, j) == -1) //�ٸ� �� ��(i - 1)�� �����ϸ�
						setGBweight(i - 1, j, getGBweight(i - 1, j) + (2 * cnt)); //�ű�� ����ġ ����
				}
				else {
					if (getGBstate(i + k, j) != -1) { //i + k�� ���� ����������
						if (i - 1 != 0 && getGBstate(i - 1, j) == -1) //i - 1 Ȯ�� �� ����ġ ������
							setGBweight(i - 1, j, getGBweight(i - 1, j) + (2 * cnt));
					}
					else 
						setGBweight(i + k, j, getGBweight(i + k, j) + (2 * cnt)); //������ �׳� i + k�� ����ġ ������
				}
				//2. ���η� ���� ���� ����ġ ���ϱ�
				k = 0, cnt = 0;
				while (j + k <= Board_Size || getGBstate(i, j + k) != 0) {
					cnt++;
					k++;
				}
				if (j + k > Board_Size) { //j + k�� �� �κ��� ��
					if (getGBstate(i, j - 1) == -1) //�ٸ� �� ��(j - 1)�� �����ϸ�
						setGBweight(i, j - 1, getGBweight(i, j - 1) + (2 * cnt)); //�ű�� ����ġ ����
				}
				else {
					if (getGBstate(i, j + k) != -1) { //j + k�� ���� ����������
						if (j - 1 != 0 && getGBstate(i, j - 1) == -1) //j - 1 Ȯ�� �� ����ġ ������
							setGBweight(i, j - 1, getGBweight(i, j - 1) + (2 * cnt));
					}
					else
						setGBweight(i, j + k, getGBweight(i, j + k) + (2 * cnt)); //������ �׳� j + k�� ����ġ ������
				}
			}
		}
	}

	check_win(); //�ٷ� �̱� �� �ִ� ���� ���� ����ġ�� ���� ����

	for (int j = 1; j <= Board_Size; j++) { //�ٵϵ��� ������ ���� ����ġ�� ��� ���ϱ� ����, ���� 0���� �ʱ�ȭ�Ѵ�.
		for (int i = 1; i <= Board_Size; i++) {
			if (getGBstate(i, j) != -1 && gameBoard[i][j].weight != 0) {
				setGBweight(i, j, 0);
			}
		}
	}
}

//���� ��ǻ�Ͱ� ���� �θ� �ٷ� �̱� �� �ִ� ������ ã�´�.
//���⼭�� 4���� �ϼ��� ���� �ٷ� �̱� �� �ִ� �����̶�� �Ǵ��Ѵ�.
void Game_Board::check_win() {
	//���� üũ
	for (int i = 1; i <= Board_Size - 3; i++) {
		for (int j = 1; j <= Board_Size; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j) == 0)
					cnt++;
			}
			if (cnt == 4) { //��ǻ���� ���� �������� 4���̸�
				if (i == 1) { //���� ���̰�
					if (getGBstate(i + 4, j) == -1) //������ ���� �ƹ��͵� ������
						setGBweight(i + 4, j, WIN_MARK); //�̱� �� �ִ�
				}
				else if (i == Board_Size - 3) { // ������ ���̰�
					if (getGBstate(i - 1, j) == -1) //���� ���� �ƹ��͵� ������
						setGBweight(i - 1, j, WIN_MARK); //�̱� �� �ִ�.
				}
				else { //�����
					if (getGBstate(i + 4, j) == -1) //�������� �������
						setGBweight(i + 4, j, WIN_MARK); //�̱�
					else { 
						if (getGBstate(i - 1, j) == -1) //������ �������
							setGBweight(i - 1, j, WIN_MARK); //�̱�
					}
				}
			}
		}
	}
	//���� üũ -> ��������, �� �Ʒ��� üũ�Ѵ�.
	for (int i = 1; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 3; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i, j + k) == 0)
					cnt++;
			}
			if (cnt == 4) {
				if (j == 1) {
					if (getGBstate(i, j + 4) == -1)
						setGBweight(i, j + 4, WIN_MARK);
				}
				else if (j == Board_Size - 3) {
					if (getGBstate(i, j - 1) == -1)
						setGBweight(i, j - 1, WIN_MARK);
				}
				else {
					if (getGBstate(i, j + 4) == -1)
						setGBweight(i, j + 4, WIN_MARK);
					else {
						if (getGBstate(i, j - 1) == -1)
							setGBweight(i, j - 1, WIN_MARK);
					}
				}
			}
		}
	}
	//���� �� -> ������ �Ʒ� �밢�� üũ
	for (int i = 1; i <= Board_Size - 3; i++) {
		for (int j = 1; j <= Board_Size - 3; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j + k) == 0)
					cnt++;
			}
			if (cnt == 4) {
				if (i == 1) {
					if (j + 4 <= Board_Size) {
						if (getGBstate(i + 4, j + 4) == -1)
							setGBweight(i + 4, j + 4, WIN_MARK);
					}
					
				}
				else if (i == Board_Size - 3) {
					if (j - 1 >= 1) {
						if (getGBstate(i - 1, j - 1) == -1)
							setGBweight(i - 1, j - 1, WIN_MARK);
					}
				}
				else {
					if (getGBstate(i + 4, j + 4) == -1)
						setGBweight(i + 4, j + 4, WIN_MARK);
					else {
						if (getGBstate(i - 1, j - 1) == -1)
							setGBweight(i - 1, j - 1, WIN_MARK);
					}
				}
			}
		}
	}
	//������ �� -> ���� �Ʒ� �밢�� üũ
	for (int i = 4; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 3; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i - k, j + k) == 0)
					cnt++;
			}
			if (cnt == 4) {
				if (i == 4) {
					if (j + 4 <= Board_Size) {
						if (getGBstate(i - 4, j + 4) == -1)
							setGBweight(i - 4, j + 4, WIN_MARK);
					}
				}
				else if (i == Board_Size) {
					if (j - 1 >= 1) {
						if (getGBstate(i - 1, j - 1) == -1)
							setGBweight(i - 1, j - 1, WIN_MARK);
					}
				}
				else {
					if (getGBstate(i - 4, j + 4) == -1)
						setGBweight(i - 4, j + 4, WIN_MARK);
					else {
						if (getGBstate(i + 1, j - 1) == -1)
							setGBweight(i + 1, j - 1, WIN_MARK);
					}
				}
			}
		}
	}
}