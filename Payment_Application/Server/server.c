#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "terminal.h"

int transSeqNum = 1111;
//int accountIndex = 0;

ST_accountsDB_t AccountDB[10] = { { 2000.0,   RUNNING, "8989374615436851" },
								  { 100000.0, BLOCKED, "5807007076043875" },
								  { 75000.0,  RUNNING, "4023598424680518" },
								  { 1200.0,   RUNNING, "4146546105799620" },
								  { 68000.0,  BLOCKED, "4717580093842489" },
								  { 50000.0,  RUNNING, "4298246897299006" },
								  { 150000.0, BLOCKED, "4259414159062333" },
								  { 10000.0,  RUNNING, "4813186286025560" },
								  { 112000.0, BLOCKED, "4961298112065015" },
								  { 21000.0,  RUNNING, "4775272632546925" } };

ST_transaction_t transDataBase[10] = { 0 };//TODO: make it dynamic so we don't have to allocate unneccessary memory places, use realloc()


EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	
	ST_accountsDB_t* accountReference = NULL;

	ST_cardData_t* transCard = NULL;
	transCard = &transData->cardHolderData;

	if (isValidAccount(transCard, &accountReference) != SERVER_OK) {
		printf("ERROR from receiveTransDtata, Account not found.\n");
		transData->transState = FRAUD_CARD;
		if (saveTransaction(transData) != SERVER_OK) {
			printf("ERROR: Saving Failure...\n");
			return INTERNAL_SERVER_ERROR;
		}
		return FRAUD_CARD;
	}

	//is blocked account?
	if (isBlockedAccount(accountReference) != RUNNING) {
		printf("ERROR: Stolen Card.\n");
		transData->transState = DECLINED_STOLEN_CARD;
		if (saveTransaction(transData) != SERVER_OK) {
			printf("ERROR: Saving Failure...\n");
			return INTERNAL_SERVER_ERROR;
		}
		return DECLINED_STOLEN_CARD;
	}

	ST_terminalData_t* transTerm = NULL;
	transTerm = &transData->terminalData;

	//is amount available?
	if (isAmountAvailable(transTerm, accountReference) != SERVER_OK) {
		printf("ERROR: Insuff. fund.\n");
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		if (saveTransaction(transData) != SERVER_OK) {
			printf("ERROR: Saving Failure...\n");
			return INTERNAL_SERVER_ERROR;
		}
		return DECLINED_INSUFFECIENT_FUND;
	}
	
	//If this line is reached that means that the account is VALID, RUNNING and has SUFFICIENT FUND
	
	if (accountReference != NULL) {//just a small protection
		printf("Balance successfully updated.\n");
		transData->transState = APPROVED;
		accountReference->balance -= transTerm->transAmount; //transData->terminalData.transAmount;  ////update account balance
		if (saveTransaction(transData) != SERVER_OK) {//Save transaction
			printf("ERROR: Saving Failure...\n");
			return INTERNAL_SERVER_ERROR;
		}
	}
	else {
		printf("ERROR: Trying to write to a NULL ptr...\n");
		return INTERNAL_SERVER_ERROR;
	}

	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t** accountRefrence) {

	//Using linear search algorithm
	for (int i = 0; i < sizeof(AccountDB)/sizeof(AccountDB[0]); i++) {

		if (strcmp(cardData->primaryAccountNumber, AccountDB[i].primaryAccountNumber) == 0) {//if PAN has a match
			*accountRefrence = &AccountDB[i];
			return SERVER_OK;
		}
	}
	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	if (accountRefrence->state == RUNNING) {
		return RUNNING;
	}
	else
		return BLOCKED;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
	if (termData->transAmount > accountRefrence->balance) {
		return LOW_BALANCE;
	}
	else
		return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	//Save cardData, terminalData, TransData and transactionSequenceNumber all to the transactions database
	
	//check the last sequence number in the transactions database
	unsigned int seqNum = 0;
	for (int i = 0; i < sizeof(transDataBase) / sizeof(transDataBase[0]); i++) {
		if (transDataBase[i].transactionSequenceNumber > seqNum)
			seqNum = transDataBase[i].transactionSequenceNumber;
	}

	//increment sequence number
	seqNum++;

	//give transaction the new sequence number
	transData->transactionSequenceNumber = seqNum;

	//Now the transData has every element (All elements) ready to be saved

	//save it in the next available place
	for (int i = 0; i < sizeof(transDataBase) / sizeof(transDataBase[0]); i++) {
		if (transDataBase[i].transactionSequenceNumber == 0) {
			transDataBase[i] = *transData;
			break;
		}
	}

	//check if successful save using the getTransaction function, if not return SAVING_FAILED
	ST_transaction_t transFound = { 0 };
	ST_transaction_t* transFoundPTR = NULL;
	transFoundPTR = &transFound;

	if (getTransaction(seqNum, &transFoundPTR) != SERVER_OK) {
		printf("ERROR: Saving Failure, CANNOT find the saved transaction...\n");
		return INTERNAL_SERVER_ERROR;
	}
	
	return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t** transData) {
	
	for (int i = 0; i < sizeof(transDataBase)/sizeof(transDataBase[0]); i++) {
		if (transactionSequenceNumber == transDataBase[i].transactionSequenceNumber) {
			printf("Successfully found transaction in database!\n");
			*transData = &transDataBase[i];
			return SERVER_OK;
		}
	}
	
	return TRANSACTION_NOT_FOUND;
}
