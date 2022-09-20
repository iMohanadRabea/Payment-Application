#include "card.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {

	printf("Enter card holder's name: ");
	//char state;
	char tempName[25] = { 0 };
	//state = getLine("Enter card holder's name: ", tempName, sizeof(tempName));
		
	if (scanf_s("%[^\n]", tempName, 25) > 0) { //state == TOO_LONG || state == TOO_SHRT || state == NO_INPUT
		for (int i = 0; i < sizeof(tempName); i++) {
			if (tempName[i] == '\0') {
				if (i < 20) {
					printf("ERROR: Name less than 20 characters.\n");
					while ((getchar()) != '\n');
					return WRONG_NAME;
				}
			}
			else {
				//Nothing
			}
		}
		strcpy_s(cardData->cardHolderName, sizeof(cardData->cardHolderName) , tempName);//use strcpy fun
		//fflush(stdin);//flush buffer
		while ((getchar()) != '\n');
		return CARD_OK;
	}
	else {
		//fflush(stdin);
		while ((getchar()) != '\n');
		return WRONG_NAME;
	}
		
	//char exit = 0;
	/*do {
		scanf("% s", tempName);
		
		if (sizeof(tempName) > 24) {
			printf("The name entered is too long.. Please try again: ");

		}
		else if (sizeof(tempName) < 13) {//I made it 13 to make naming easier
			printf("The name entered is too short.. Please try again: ");

		}else
	} while(!exit);*/
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	printf("Enter card expiry date: ");
	char tempDate[6] = { 0 };
	char tc1, tc2;
	//instead of scanf()
	if (scanf_s("%[^\n]", tempDate, 6) > 0) {//MM/YY
		
		if (tempDate[2] != '/') {
			return WRONG_EXP_DATE;
		}
		for (int i = 0; i < sizeof(tempDate); i++) {
			if (tempDate[i] == '\0') {
				if (i < 5) {
					printf("ERROR: Date less than 5 characters.\n");
					return WRONG_EXP_DATE;
				}
			}
			else {
				//Nothing
			}
		}
		
		tc1 = tempDate[0];
		tc2 = tempDate[1];

		while ((getchar()) != '\n');
		
		if (atoi(&tc1) > 1) {//not more than 12 months
			//printf("ERROR: wrong expiry date.. Months not more than 12.\n");
			//while ((getchar()) != '\n');
			return WRONG_EXP_DATE;
		}
		else if (atoi(&tc1) == 1 && atoi(&tc2) > 2) {//no more than 12 months
			//printf("ERROR: wrong expiry date.. Months not more than 12000.\n");
			//while ((getchar()) != '\n');
			return WRONG_EXP_DATE;
		}
		else if (atoi(&tc1) != 1 && atoi(&tc2) == 0) {
			//printf("ERROR: wrong expiry date.. Months not less than 1.\n");
			//while ((getchar()) != '\n');
			return WRONG_EXP_DATE;
		}
		else {
			strcpy_s(cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), tempDate);
			//printf("Correct date.\n");
			//while ((getchar()) != '\n');
			return CARD_OK;
		}
		
	}
	else {
		//while ((getchar()) != '\n');
		return WRONG_EXP_DATE;
	}


	//test conditions returning wrong values
	/*if (atoi(&d) > 1) {//not more than 12 months
			printf("ERROR: wrong expiry date.. Months not more than 12.\n");
			return WRONG_EXP_DATE;
		}
		else if (atoi(&tempDate[0]) == 1 && atoi(&tempDate[1]) > 2) {//no more than 12 months
			printf("ERROR: wrong expiry date.. Months not more than 12000.\n");
			return WRONG_EXP_DATE;
		}
		else if (atoi(&tempDate[0]) != 1 && atoi(&tempDate[1]) == 0) {
			printf("ERROR: wrong expiry date.. Months not less than 1.\n");
			return WRONG_EXP_DATE;
		}
		else {
			printf("Correct date.\n");
			return CARD_OK;
		}*/


}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	//todo: protect input from non-numbers chars 
	printf("Enter card PAN: ");
	char tempPAN[20] = { 0 };
	int len = 0;
	
	if (scanf_s("%s", tempPAN, sizeof(tempPAN)) > 0) {
		for (int i = 0; i < sizeof(tempPAN); i++) {
			if (tempPAN[i] == '\0') {
				if (len < 16) {
					printf("ERROR: PAN less than 16 number.\n");
					return WRONG_PAN;
				}
			}
			else {
				len++;
			}
		}
		strcpy_s(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), tempPAN);
		while ((getchar()) != '\n');
		return CARD_OK;
	}
	else {
		while ((getchar()) != '\n');
		return WRONG_PAN;
	}

}