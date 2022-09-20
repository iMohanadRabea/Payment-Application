#include <stdio.h>
#include <time.h>
#include <string.h>
#include "app.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"



void appStart(void) {


	//make a dummy variable to store card data
	ST_cardData_t card = { 0, 0, 0 };
	ST_cardData_t* cardPTR = NULL;
	cardPTR = &card;

	//make a dummy variable to store terminal data
	ST_terminalData_t terminal = {0.0, 0.0, 0};
	ST_terminalData_t* termPTR = NULL;
	termPTR = &terminal;

	//make a dummy variable to store transaction data
	ST_transaction_t transaction = {0};//todo check if this initialization is correct
	ST_transaction_t* transPTR = NULL;
	transPTR = &transaction;


	/*----------------------------------------------CARD section-----------------------------------------*/
	
	//get card holder name
	if (getCardHolderName(cardPTR) != CARD_OK) {
		printf("ERROR: Wrong name entry.. The name should be 20-24 characters long.\n\n");
		//return;
	}
	//get card expiry date
	if (getCardExpiryDate(cardPTR) != CARD_OK) {
		printf("ERROR: wrong expiry date.. the date should be in MM/YY format.\n\n");
	}

	//get card PAN
	if (getCardPAN(cardPTR) != CARD_OK) {
		printf("ERROR: wrong PAN.. the PAN should be 16-20 numbers long.\n\n");
	}

	/*----------------------------------------------TERMINAL section-----------------------------------------*/

	//get card data

	//get transaction date
	if (getTransactionDate(termPTR) != TERMINAL_OK) {
		printf("ERROR: Wrong date.. the date should be in DD/MM/YYYY format.\n\n");
	}
	
	//check if card is expired
	if (isCardExpired(cardPTR, termPTR) != TERMINAL_OK) {
		printf("ERROR: Expired Card.\n\n");
		return;
	}

	//check PAN
	if (isValidCardPAN(cardPTR) != TERMINAL_OK) {
		printf("ERROR: Invalid PAN.\n\n");
	}

	//todo ask for pin number--------------------------------------------------------------------------------

	//get transaction amount
	if (getTransactionAmount(termPTR) != TERMINAL_OK) {
		printf("ERROR: Invalid Amount.\n\n");
	}

	if (setMaxAmount(termPTR) != TERMINAL_OK) {
		printf("ERROR: Cannot set max amount.\n\n");
	}

	//is exceeded amount?
	if (isBelowMaxAmount(termPTR) != TERMINAL_OK) {
		printf("ERROR: Max Amount Exceeded.\n\n");
		return;
	}

	//Fill in transaction data before sending it to the server
	transPTR->cardHolderData = card;
	transPTR->terminalData = terminal;
	
	/*----------------------------------------------SERVER section-----------------------------------------*/

	//check account validity
	int reply;
	//strcpy(reply, recieveTransactionData(transPTR)); 
	reply = recieveTransactionData(transPTR);
	if (reply == FRAUD_CARD) {
		printf("FRAUD CARD...\n\n");
	}
	else if (reply == DECLINED_INSUFFECIENT_FUND) {
		printf("Insuffecient fund...\n\n");
	}
	else if (reply == DECLINED_STOLEN_CARD) {
		printf("Stolen Card...\n\n");
	}
	else if (reply == INTERNAL_SERVER_ERROR) {
		printf("Internal server error...\n\n");
	}
	else if (reply == APPROVED) {
		printf("Approved...\n\n");
	}
	else {
		printf("Didn't enter any condition...\n\n");
	}


	//When transaction is finished, wipe any temporary data
	strcpy_s(card.primaryAccountNumber, sizeof(card.primaryAccountNumber) , "0000000000000000");
	strcpy_s(card.cardExpirationDate, sizeof(card.cardExpirationDate) , "xx/xx");
	strcpy_s(card.cardHolderName, sizeof(card.cardHolderName) , "xxxxxxxxxxxxxxxx");
}

