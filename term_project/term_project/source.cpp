#include <iostream>
#include <string>
#include<ctime>
#include "Gameboard.h"
using namespace std;

int main() {
	srand((unsigned int)time(NULL));
	int select;
	while (true) {
		cout << "���� : 1, �İ� : 0 �� �Է����ּ��� >> ";
		cin >> select;
		if (select == 1) {
			cout << "�����Դϴ�" << endl;
			break;
		}
		else if (select == 0) {
			cout << "�İ��Դϴ�" << endl;
			break;
		}
		else {
			cout << "�ٽ� �Է����ּ���" << endl;
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

