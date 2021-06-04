//2016112109 장석운
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
#define Board_Size 10
/*  컴퓨터가 막아야 하는 부분을 마킹
    중요도에 따라 mark를 나누었음 
    중요하면 weight를 크게 두어 그 곳에 컴퓨터가 수를 두게 함  */
#define MARK2 9999
#define MARK3 99999 
#define MARK4 999999 
#define WIN_MARK 9999999 


typedef struct spot { //한 지점의 상태값과 가중치
	int state; // 상태값
	int weight; // 가중치
}spot;

class Game_Board {
public:
	//Location location; //바둑판 위에서 위치를 잡기 위한 멤버 변수
	spot gameBoard[Board_Size + 1][Board_Size + 1]; //바둑판 선언
	int turn; //1 == 내 차례, 0 == 컴퓨터 차례
public:
	void chakshu(); //한 점에 착수

	void check_opened_3(); //열린 3목 확인
	void check_blanked_3(); //띈 3목 확인
	void check_closed_4(); //닫힌 4목 확인
	void check_blanked_4(); //띈 4목 확인
	void check_win();
	
	bool check_5(); //승리조건 확인
	int getGBstate(int r, int c); //한 지점의 state 불러옴
	int getGBweight(int r, int c); //한 지점의 가중치 불러옴
	void setGBstate(int r, int c, int value); //한 지점의 state 설정
	void setGBweight(int r, int c, int value); //한 지점의 가중치 설정
	void draw(); //오목판 그리기
	void init(int); //오목판 초기화
	void calc_weight(); // weight 계산, 이를 통해 컴퓨터가 올바른 수를 놓을 수 있도록 함
};

/*처음 게임보드를 초기화하는 함수. 모든 점의 state는 -1, weight는 0으로 초기화한다.*/ 
void Game_Board::init(int select) {
	for (int i = 1; i < Board_Size + 1; i++) {
		for (int j = 1; j < Board_Size + 1; j++) {
			setGBstate(i, j, -1);
			setGBweight(i, j, 0);
		}
	}
	turn = select; //사용자에게 입력을 받고 선공후공을 정한다.
	int i = rand() % Board_Size + 1; //보드 사이즈는 인덱스를 1부터 참조하기 위해 Board_Size + 1로 하였다.
	int j = rand() % Board_Size + 1;
	setGBweight(i, j, MARK3);
}

//돌을 놓는 함수 ,즉 오목판에 state를 정하는 함수
void Game_Board::chakshu() {
	int r, c; // row, col
	
	if (turn == 1) { //내 차례
		//cout << "점을 선택하시오 : ";
		while (true) {
			int row, col;
			cout << "입력해주세요(1부터 바둑판 크기까지) >> ";
			cin >> row >> col;
			if (row <= 0 || row > Board_Size || col <= 0 || col > Board_Size) //바둑판의 범위를 벗어나면 다시 입력
				continue;
			else {
				r = row; //입력받은 지점을 선택
				c = col;
				break;
			}
		}
		
		if (getGBstate(r, c) != -1) { //만약 입력한 곳에 이미 돌이 있다면 다시 입력
			setGBweight(r, c, 0);
			cout << "다시 선택하세요" << endl << endl;
		}
		else {
			setGBstate(r, c, turn); //내 돌 놓기, state == 1
			turn = 0; // 컴퓨터로 차례 돌리기
		}
	} 
	else { //상대 차례
		check_blanked_3(); // 빈 3목인지 확인
		check_closed_4();  // 닫힌 4목인지 확인
		check_opened_3();  //열린 3목인지 확인
		check_blanked_4(); //빈 4목인지 확인
		calc_weight();     //가중치 계산
		int max = -1, index_i = 0, index_j = 0;
		for (int i = 1; i <= Board_Size; i++) {
			for (int j = 1; j <= Board_Size; j++) {
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
		setGBstate(index_i, index_j, turn); //컴퓨터돌 놓기, state == 0
		turn = 1; //차례 돌리기
	}
}

/* 닫힌 4목 함수 */
void Game_Board::check_closed_4() {
	//////////////////////
	//가로 닫힌 4 체크////
	//////////////////////
	for (int i = 1; i <= Board_Size; i++) { //맨 왼쪽 가로로 닫힌 4
		int cnt = 0;
		for (int j = 1; j <= 4; j++) {
			if (gameBoard[j][i].state == 1)
				cnt++;
		}
		if (cnt == 4) //cnt가 4라는 것은 연속으로 돌이 4개가 놓여졌다는 것. 따라서 끝에 Mark4로 표기함
			setGBweight(5, i, MARK4);
	}
	for (int i = 1; i <= Board_Size; i++) { //맨 오른쪽 가로로 닫힌 4
		int cnt = 0;
		for (int j = 1; j <= 4; j++) {
			if (gameBoard[Board_Size - 1 - j][i].state == 1) //나의 수가 있으면 cnt++
				cnt++;
		}
		if (cnt == 4) //cnt가 4라는 것은 연속으로 돌이 4개가 놓여졌다는 것. 따라서 끝에 Mark4로 표기함
			setGBweight(Board_Size - 4, i, MARK4);
	}
	for (int j = 1; j <= Board_Size; j++) { // 가운데 가로로 닫힌 4
		for (int i = 2; i < Board_Size - 3; i++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j) == 1)
					cnt++;
			}
			if (cnt == 4) {
				if (gameBoard[i - 1][j].state == 0 && gameBoard[i + 4][j].state == -1) /*만약 오른족을 막았고, 왼쪽이 비어있으면, 왼쪽을 막아야됨*/
					setGBweight(i + 4, j, MARK4);
				else if (gameBoard[i - 1][j].state == -1 && gameBoard[i + 4][j].state == 0)  /*만약 왼쪽을 막았고, 오른족이 비어있으면, 오른족을 막아야됨*/
					setGBweight(i - 1, j, MARK4);
			}
				
		}
	}
	///////////////////////////
	//// 세로 닫힌 4 체크  ////
	///////////////////////////
	for (int i = 1; i <= Board_Size; i++) { //위쪽 세로로 닫힌 4
		int cnt = 0;
		for (int j = 1; j <= 4; j++) {
			if (getGBstate(i, j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i, 5, MARK4);
	}
	for (int i = 1; i <= Board_Size; i++) { //아래쪽 세로로 닫힌 4
		int cnt = 0;
		for (int j = Board_Size; j < Board_Size - 4; j--) {
			if (getGBstate(i, j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i, Board_Size - 4, MARK4);
	}
	for (int i = 1; i <= Board_Size; i++) { // 가운데 세로로 닫힌 4 -> 위쪽에 상대돌
		for (int j = 2; j <= Board_Size - 3; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i, j + k) == 1)
					cnt++;
			}
			if (cnt == 4) {
				if(getGBstate(i, j - 1) == 0 && getGBstate(i, j + 4) == -1) //위쪽을 막았고, 아래쪽이 비었으면 아래를 막아야 함
					setGBweight(i, j + 4, MARK4); 
				else if(getGBstate(i, j - 1) == -1 && getGBstate(i, j + 4) == 0) //아래쪽을 막고 위쪽의 비었으면 위를 막아야 함
					setGBweight(i, j - 1, MARK4);
			}
				
		}
	}
	

	///////////////////////////////////////////////////////
	/////왼쪽 위 -> 오른쪽 아래 대각선으로 닫힌 4 체크/////
	///////////////////////////////////////////////////////
	for (int i = 1; i <= Board_Size - 4; i++) { //맨 위쪽(j == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i + j, j + 1) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i + 4, 5, MARK4);
	}
	for (int j = 1; j <= Board_Size - 4; j++) { // 맨 오른쪽(i == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(i + 1, j + i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(5, j + 4, MARK4);
	}
	for (int j = 5; j <= Board_Size; j++) { // 맨 왼쪽(i == Board_Size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(Board_Size - i, j - i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(Board_Size - 4, j - 4, MARK4);
	}
	for (int i = 5; i <= Board_Size; i++) { //맨 아래쪽(j == Board_size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i - j, Board_Size  - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i - 4, Board_Size - 4, MARK4);
	}
	for (int i = 2; i <= Board_Size - 4; i++) { //가운데 대각선으로 닫힌 4 -> 위쪽에 돌
		for (int j = 2; j <= Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j + k) == 1)
					cnt++;
			}
			if (cnt == 4) {
				if(getGBstate(i - 1, j - 1) == 0 && getGBstate(i + 4, j + 4) == -1) //위쪽을 막고 아래쪽이 비었으면 아래쪽을 막아야 함
					setGBweight(i + 4, j + 4, MARK4);
				else if(getGBstate(i - 1, j - 1) == -1 && getGBstate(i + 4, j + 4) == 0) //반대로 위쪽이 비었으면 위를 막아야 함
					setGBweight(i - 1, j - 1, MARK4);
			}
		}
	}
	//////////////////////////////////////////////////////////////
	///////오른쪽 위 -> 왼쪽 아래 대각선으로 닫힌 4 체크//////////
	//////////////////////////////////////////////////////////////
	for (int i = 5; i <= Board_Size; i++) { //맨 위쪽(j == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (gameBoard[i - j][j + 1].state == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i - 4, 5, MARK4);
	}
	for (int j = 5; j <= Board_Size; j++) { // 맨 왼쪽(i == 0) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(i + 1, j - i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(5, j - 4, MARK4);
	}
	for (int j = 1; j <= Board_Size - 4; j++) { // 맨 오른쪽(i == Board_Size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int i = 0; i < 4; i++) {
			if (getGBstate(Board_Size - i, j + i) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(Board_Size - 4, j + 4, MARK4);
	}
	for (int i = 1; i <= Board_Size - 4; i++) { //맨 아래쪽(j == Board_size) 대각선으로 닫힌 4
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (getGBstate(i + j, Board_Size - j) == 1)
				cnt++;
		}
		if (cnt == 4)
			setGBweight(i + 4, Board_Size - 4, MARK4);
	}
	for (int i = 5; i <= Board_Size - 1; i++) { //가운데 대각선으로 닫힌 4 -> 위쪽에 돌
		for (int j = 2; j <= Board_Size - 4; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i - k, j + k) == 1)
					cnt++;
			}
			if (cnt == 4) {
				if (getGBstate(i + 1, j - 1) == 0 && getGBstate(i - 4, j + 4) == -1) //위쪽을 막고 아래쪽이 비었으면 아래쪽을 막아야 함
					setGBweight(i - 4, j + 4, MARK4);
				else if(getGBstate(i + 1, j - 1) == -1 && getGBstate(i - 4, j + 4) == 0) //반대로 위쪽이 비었으면 위를 막아야 함
					setGBweight(i + 1, j - 1, MARK4);
			}
		}
	}
}

/* 빈 4목 함수, 오목판을 쭉 훑으며 연속된 5지점을 검사한다. 
  만약 빈 지점이 1이고, 나의 돌의 수가 4개이면 빈 4목이라는 것이다. */
void Game_Board::check_blanked_4() {
	//가로
	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size - 4; i++) {
			int blank = 0, cnt = 0, index = -1; /* blank : 비어있는 지점의 수, cnt : 나의 수 개수, index = 비어있는 지점의 위치*/
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j) == 1) //
					cnt++;
				if ((i + k != i || i + k != i + 4) && getGBstate(i + k, j) == -1) { //바둑판의 범위를 안 벗어나고, 해당 위치가 비어있으면 blank++
					blank++;
					index = i + k;
				}
			}
			if (blank == 1 && cnt == 4 && index != -1) //빈 지점이 1이고, 나의 수가 4개이면 빈 4목이라는 의미이다. 가중치를 설정한다.
				setGBweight(index, j, MARK4);
		}
	}
	//세로
	for (int i = 1; i <= Board_Size; i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i, j + k) == 1)
					cnt++;
				if ((j + k != j || j + k != j + 4) && getGBstate(i, j + k) == -1) { //마찬가지
					blank++;
					index = j + k;
				}
			}
			if (blank == 1 && cnt == 4 && index != -1)
				setGBweight(i, index, MARK4);
		}
	}
	//왼쪽 위 -> 오른쪽 아래 대각선
	for (int i = 1; i <= Board_Size - 4;i++) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j + k) == 1)  //마찬가지
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
	for (int i = Board_Size; i > 4; i--) {
		for (int j = 1; j <= Board_Size - 4; j++) {
			int blank = 0, cnt = 0, index = -1;
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i - k, j + k) == 1)  //마찬가지
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

/*   빈 3목 함수, 방법은 빈 4목 함수와 같다. 다만 연속된 4 지점을 체크한다. */
void Game_Board::check_blanked_3() {
	//가로
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
	//세로
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
	//왼쪽 위 -> 오른쪽 아래 대각선
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
	//오른쪽 위 -> 왼쪽 아래 대각선 체크
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

/* 열린 3목 함수, 바둑판을 훑으며 연속된 5개의 지점을 검사한다. 
   양 쪽이 비어있고, 가운데 연속으로 나의 수가 3개 나오면 빈 3목이라는 것이다.*/
void Game_Board::check_opened_3() {
	//가로
	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size - 4; i++) {
			int index = i, front = 0, end = 0, cnt = 0; //index == , front : 앞 쪽이 비었는지 확인, end : 끝 쪽이 비었는지 확인
			if (getGBstate(i, j) == -1) //앞 쪽이 비었는지 확인
				front++;
			if (getGBstate(i + 4, j) == -1) //뒤 쪽이 비었는지 확인
				end++;
			for (int k = 1; k < 4; k++) {
				if (getGBstate(i + k, j) == 1) //나의 돌이 놓여져 있으면 cnt++
					cnt++;
			}
			if (front == 1 && end == 1 && cnt == 3) { //만약 열린 3목이면
				if (front > Board_Size / 2) setGBweight(i, j, MARK3); //좀 균형있게 수를 놓기 위해 가운데로 수를 유도한다.
				else setGBweight(i + 4, j, MARK3);
			}
		}
	}
	//세로 -> 마찬가지 ,다만 이 때는 front가 위고 end가 아래이다.
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
	//왼쪽 위 -> 오른쪽 아래 대각선
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
	//오른쪽 위 -> 왼쪽 아래 대각선 체크
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

//오목이 완성되었는지 확인한다, 바둑판을 훑으며 연속된 5지점을 검사한다.
bool Game_Board::check_5() {
	//가로 체크
	for (int i = 1; i <= Board_Size - 4; i++) {
		for (int j = 1; j <= Board_Size; j++) {
			int mycnt = 0, opcnt = 0; 
			for (int k = 0; k < 5; k++) {
				if (getGBstate(i + k, j) == 1) { //나의 수가 있으면 mycnt++
					mycnt++;
				}
				else if (getGBstate(i + k, j) == 0) { //컴퓨터의 수가 놓여져 있으면 opcnt++
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

//바둑판의 row와 col의 위치에 있는 state와 weight의 값을 불러오거나 값을 정한다.
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

//바둑판을 그리는 함수
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

void Game_Board::calc_weight() { //가중치 구하기
	for (int i = 1; i <= Board_Size; i++) { //weight 초기화
		for (int j = 1; j <= Board_Size; j++) {
			if (getGBweight(i, j) < 1000) { //MARK보다 훨씬 작은 수는 MARKING이 안된 것이라는 의미이므로
				setGBweight(i, j, 0);
			}
		}
	}

	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size; i++) {
			if (j == 1) { // 위
				if (i == 1) {//왼쪽 위 모서리
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
				else if (i == Board_Size) { //오른쪽 위
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
			else if (j == Board_Size) { //밑
				if (i == 1) {//왼쪽 아래 모서리
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
				else if (i == Board_Size) { //오른쪽 아래 모서리
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

	for (int j = 1; j <= Board_Size; j++) {
		for (int i = 1; i <= Board_Size; i++) {
			int cnt = 0;
			if (getGBstate(i, j) == 0) {
				//1. 가로로 많은 곳에 가중치 더하기
				int k = 0;
				while (i + k <= Board_Size || getGBstate(i + k, j) != 0) {
					cnt++;
					k++;
				}
				if (i + k > Board_Size) { //i + k가 끝 부분일 때
					if(getGBstate(i - 1, j) == -1) //다른 한 쪽(i - 1)이 가능하면
						setGBweight(i - 1, j, getGBweight(i - 1, j) + (2 * cnt)); //거기다 가중치 더함
				}
				else {
					if (getGBstate(i + k, j) != -1) { //i + k에 돌이 놓여졌으면
						if (i - 1 != 0 && getGBstate(i - 1, j) == -1) //i - 1 확인 후 가중치 더해줌
							setGBweight(i - 1, j, getGBweight(i - 1, j) + (2 * cnt));
					}
					else 
						setGBweight(i + k, j, getGBweight(i + k, j) + (2 * cnt)); //나머진 그냥 i + k에 가중치 더해줌
				}
				//2. 세로로 많은 곳에 가중치 더하기
				k = 0, cnt = 0;
				while (j + k <= Board_Size || getGBstate(i, j + k) != 0) {
					cnt++;
					k++;
				}
				if (j + k > Board_Size) { //j + k가 끝 부분일 때
					if (getGBstate(i, j - 1) == -1) //다른 한 쪽(j - 1)이 가능하면
						setGBweight(i, j - 1, getGBweight(i, j - 1) + (2 * cnt)); //거기다 가중치 더함
				}
				else {
					if (getGBstate(i, j + k) != -1) { //j + k에 돌이 놓여졌으면
						if (j - 1 != 0 && getGBstate(i, j - 1) == -1) //j - 1 확인 후 가중치 더해줌
							setGBweight(i, j - 1, getGBweight(i, j - 1) + (2 * cnt));
					}
					else
						setGBweight(i, j + k, getGBweight(i, j + k) + (2 * cnt)); //나머진 그냥 j + k에 가중치 더해줌
				}
			}
		}
	}

	check_win(); //바로 이길 수 있는 곳은 가장 가중치를 높게 잡음

	for (int j = 1; j <= Board_Size; j++) { //바둑돌이 놓여진 곳의 가중치는 고려 안하기 때문, 전부 0으로 초기화한다.
		for (int i = 1; i <= Board_Size; i++) {
			if (getGBstate(i, j) != -1 && gameBoard[i][j].weight != 0) {
				setGBweight(i, j, 0);
			}
		}
	}
}

//만약 컴퓨터가 수를 두면 바로 이길 수 있는 지점을 찾는다.
//여기서는 4목이 완성된 곳을 바로 이길 수 있는 지점이라고 판단한다.
void Game_Board::check_win() {
	//가로 체크
	for (int i = 1; i <= Board_Size - 3; i++) {
		for (int j = 1; j <= Board_Size; j++) {
			int cnt = 0;
			for (int k = 0; k < 4; k++) {
				if (getGBstate(i + k, j) == 0)
					cnt++;
			}
			if (cnt == 4) { //컴퓨터의 수가 연속으로 4개이면
				if (i == 1) { //왼쪽 끝이고
					if (getGBstate(i + 4, j) == -1) //오른쪽 끝에 아무것도 없으면
						setGBweight(i + 4, j, WIN_MARK); //이길 수 있다
				}
				else if (i == Board_Size - 3) { // 오른쪽 끝이고
					if (getGBstate(i - 1, j) == -1) //왼쪽 끝에 아무것도 없으면
						setGBweight(i - 1, j, WIN_MARK); //이길 수 있다.
				}
				else { //가운데에
					if (getGBstate(i + 4, j) == -1) //오른쪽이 비었으면
						setGBweight(i + 4, j, WIN_MARK); //이김
					else { 
						if (getGBstate(i - 1, j) == -1) //왼쪽이 비었으면
							setGBweight(i - 1, j, WIN_MARK); //이김
					}
				}
			}
		}
	}
	//세로 체크 -> 마찬가지, 위 아래로 체크한다.
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
	//왼쪽 위 -> 오른쪽 아래 대각선 체크
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
	//오른쪽 위 -> 왼쪽 아래 대각선 체크
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