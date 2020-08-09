
#include "Server.h"

#include <conio.h>


int main(int argc, char* argv[]) {

	Server s;
	s.Create(8000);
	s.Start();

	while (true) {
		if (_kbhit()) {
			break;
		}
		Sleep(1000);
	}

	system("pause");
	s.Stop();
	return 0;
}
