#include "stdafx.h"
#include "UserData.h"


void CircularList::push_back(const UserData& aUserData)
{
	while (iFront == 0 && iRear == iMaxSize || iFront == iRear + 1) {
		//Wait until someone consumes the data
		Sleep(10);
	}
	
	if (iFront == -1 && iRear == -1) {
		iFront = 0;
		iRear = 0;
	} else if (iRear == iMaxSize) {
		iRear = 0;
	} else {
		iRear++;
	}
	iList[iRear] = aUserData;
	iSize++;
}

const UserData CircularList::pop_front() {
	int frontIndex = 0;
	UserData userData;

	if (iFront == -1 && iRear == -1) {
		iSize = 0;
		Sleep(10);
	}

	if (iFront != -1 || iRear != -1) {
		frontIndex = iFront;
		if (iFront == iRear) {
			iFront = -1;
			iRear = -1;
		}
		else if (iFront == iMaxSize) {
			iFront = 0;
		}
		else {
			iFront++;
		}

		userData = iList[frontIndex];
		iSize--;
	}

	return userData;
}

const UserData CircularList::front() {
	int frontIndex = 0;
	UserData userData;

	if (iFront == -1 && iRear == -1) {
		iSize = 0;
		Sleep(10);
	}

	if (iFront != -1 || iRear != -1) {
		frontIndex = iFront;
		userData = iList[frontIndex];
	}

	return userData;
}

UserData UserDataRecord::Read()
{
	UserData userData;

	//Once reading all the data, update read complition
	if (iWriteDone && iUserDataRecords.empty()) {
		FinishedReading();
	}
	if (!iReadDone) {
		if (iUserDataRecords.empty()) {
			//There isn't enough data to be read. Wait before trying to read
			Sleep(50);
		}
		if (!iUserDataRecords.empty()) {
			//We have some data avilable now
#ifdef USE_DEQUE
			userData = iUserDataRecords.front();
			iUserDataRecords.pop_front();
#else
			userData = iUserDataRecords.pop_front();
#endif
		}
	}

	return userData;
}

string Tokenizer::ParseToken()
{
	int index = 0;
	int foundIndex = -1;
	string token;

	if (iIndex < iEntrySize) {

		//Look for all the possible delimiters
		for (; index < iDelimiterSize; index++) {
			foundIndex = iEntry.find(iTokenDelimiter[index], iIndex);
			if (foundIndex != -1) {
				break;
			}
		}

		if (foundIndex != -1) {
			token = iEntry.substr(iIndex, foundIndex - iIndex);
			iIndex = foundIndex + 1;
			//Move the index if we have multiples of same delimiter
			while (iIndex < iEntrySize && iEntry.at(iIndex) == iTokenDelimiter[index]) {
				iIndex++;
			}
		}
		else {
			token = iEntry.substr(iIndex);
			iIndex = iEntrySize;
		}
	}

	return token;
}
