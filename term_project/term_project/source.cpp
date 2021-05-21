#include <iostream>
#include <string>
#include<ctime>
#include "Gameboard.h"
using namespace std;

int main() {
	srand((unsigned int)time(NULL));
	int select;
	while (true) {
		cout << "선공 : 1, 후공 : 0 을 입력해주세요 >> ";
		cin >> select;
		if (select == 1) {
			cout << "선공입니다" << endl;
			break;
		}
		else if (select == 0) {
			cout << "후공입니다" << endl;
			break;
		}
		else {
			cout << "다시 입력해주세요" << endl;
		}
	}

	Game_Board g;
	g.init(select);
	while (true) {
		if (g.check_5() == true) {
			if(g.turn == 0)
				g.draw();
			return 0;
		}
		g.chakshu();
		if(g.turn == 1)
			g.draw();
	}
}

