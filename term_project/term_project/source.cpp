#include <iostream>
#include <string>
#include<ctime>
#include "Gameboard.h"
using namespace std;

int main() {
	srand((unsigned int)time(NULL));
	Game_Board g;
	g.init();
	while (true) {
		if (g.check_5() == true) {
			g.draw()
			return 0;;
		}
		g.chakshu();
		if(g.turn == 1)
			g.draw();
	}
}