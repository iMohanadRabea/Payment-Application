#include "../Terminal/terminal.h"
//#include "../Card/card.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

float maxTransAmount = 4000;


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {//DD/MM/YYYY

	time_t tim;
	tim = time(NULL);
	struct tm tm;
	localtime_s(&tm, &tim);
	printf("Date is: %d/%d/%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	
	//convert to string
	char tempHold[11] = { 0 };

	if (sprintf_s(tempHold, sizeof(tempHold), "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900) <= 0) {
		return WRONG_DATE;
	}
	
	strcpy_s(termData->transactionDate, sizeof(termData->transactionDate), tempHold);
	printf("Date: %s\n", tempHold);

	return TERMINAL_OK;
/*
	termData->transactionDate[0] = tempHold[0];//save DD
	termData->transactionDate[1] = tempHold[1];
	termData->transactionDate[2] = '/';

	//sprintf(tempHold, "%d", tm.tm_mon + 1);
	termData->transactionDate[3] = tempHold[0];//save MM
	termData->transactionDate[4] = tempHold[1];
	termData->transactionDate[5] = '/';
	
	//sprintf(tempHold, "%d", tm.tm_year + 1900);
	termData->transactionDate[6] = tempHold[0];//save YYYY
	termData->transactionDate[7] = tempHold[1];
	termData->transactionDate[8] = tempHold[2];
	termData->transactionDate[9] = tempHold[3];
*/	
	
/*	//todo get date from computer
	printf("Enter date in DD/MM/YYYY format: ");
	char date[11] = { 0 };
	if (scanf_s("%s", date, 11) > 0) {
		while ((getchar()) != '\n');
		return TERMINAL_OK;
	}
	else {
		printf("Error from getTransactionDate... No Input\n");
		while ((getchar()) != '\n');
		return WRONG_DATE;
	}
*/
	
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	
	char cardDate[2] = { 0 };
	char termDate[2] = { 0 };
	int card = 0, term = 0;
	//will try string compare function later
	
	//first compare years
	cardDate[0] = cardData->cardExpirationDate[3];//MM/YY
	cardDate[1] = cardData->cardExpirationDate[4];

	termDate[0] = termData->transactionDate[8];//DD/MM/YYYY
	termDate[1] = termData->transactionDate[9];

	card = atoi(cardDate);
	term = atoi(termDate);
	//Now we subtract years
	if ((card - term) > 0)
		return TERMINAL_OK;
	else if (card - term == 0) {
		//Will compare months
		cardDate[0] = cardData->cardExpirationDate[0];//MM/YY
		cardDate[1] = cardData->cardExpirationDate[1];

		termDate[0] = termData->transactionDate[3];//DD/MM/YYYY
		termDate[1] = termData->transactionDate[4];

		card = atoi(cardDate);
		term = atoi(termDate);

		//Now we subtract months
		if ((card - term) > 0)
			return TERMINAL_OK;
		else
			return EXPIRED_CARD;
	}
	else
		return EXPIRED_CARD;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {
	//Luhn check
	printf("Checking PAN...\n");
	//int PANC[20];
	int digits = 0;
	
	//count digits in card
	for (int i = 0; i < sizeof(cardData->primaryAccountNumber) - 1; i++) {
		if (cardData->primaryAccountNumber[i] == '\0') {
			break;//count is finished
		}
		digits++;
	}

	//now make an array to store the reverse order PAN. i.e. 123456789 -> 987654321
	
	char* revPAN = NULL;
	revPAN = (char*)malloc(digits*sizeof(char)); //so if the pan is 16 digits, we now created a 16 char array
	if (revPAN == NULL) {
		printf("Error allocating memory for PAN checking...\n");
		return INVALID_CARD;
	}

	for (int i = 0; i < digits; i++) {
		revPAN[digits - 1 - i] = cardData->primaryAccountNumber[i];
		//Now we have the reversed PAN
	}
	
	//will start the operation

	int dummyvar = 0;
	int checksum = 0;
	char dummychar = '\0';
	for (int i = 1; i < digits; i += 2) {
		dummychar = revPAN[i];
		dummyvar = atoi(&dummychar) * 2; //convert every char to int then multiply by 2
		if (dummyvar >= 10) {
			dummyvar -= 9;
		}
		checksum += dummyvar;
	}

	for (int i = 0; i < digits; i += 2) {
		dummychar = revPAN[i];
		checksum += atoi(&dummychar);
	}

	if (checksum % 10 == 0) {//the number is divisible by 10
		printf("Vaild PAN.\n");
		free(revPAN);
		return TERMINAL_OK;
	}
	else {
		printf("Invaild PAN.\n");
		free(revPAN);
		return INVALID_CARD;
	}
		

	//checksum += atoi(revPAN[0]); //add the first digit (check digit) to the sum
	
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {

	//Ask user for trans amount
	printf("Please enter the transaction amount: ");
	float transam = 0.0;
	scanf_s("%f", &transam);
	while ((getchar()) != '\n');

	if (transam > 0) {
		termData->transAmount = transam;
		return TERMINAL_OK;
	}
	else {
		return INVALID_AMOUNT;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {

	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	else
		return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {

	if (maxTransAmount <= 0)
		return INVALID_MAX_AMOUNT;
	else {
		termData->maxTransAmount = maxTransAmount;
		return TERMINAL_OK;
	}

}