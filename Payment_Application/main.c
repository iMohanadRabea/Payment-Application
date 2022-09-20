/****************************************************/
/*..............Author: Mohanad Rabea...............*/
/****************************************************/
#include <stdio.h>
#include "Application/app.h"
#include "Card/card.h"
#include "Terminal/terminal.h"
#include "Server/server.h"


int main() {

	char exit = '0';
	printf("Hello!\n");

	do {
	
		appStart();
		
		printf("\nDo you want to make another transaction? Y/N ");
		if (scanf_s("%c", &exit, 1) > 0) {//todo make fool proof
			
			if (exit == 'N' || exit == 'n')
				break;
		
		}
		else {
			printf("No input detected, Please try again...\n");
		}
		while ((getchar()) != '\n');
	} while (1);
	printf("\nHave a nice day :D\n");

	return 0;
}