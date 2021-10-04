#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#ifdef USE_DEQUE
#include <deque>
#endif
#include <string>
using namespace std;


#define MAX_WRITE_LIMIT 5000
#define MAX_LIST_SIZE (MAX_WRITE_LIMIT + 100)

struct UserData {
	string iName;
	string iLocation;
	string iItem;
	double iPrice;

	UserData() {}

	UserData(const string& aName, const string& aLocation, const string& aItem, const double aPrice) :
		iName(aName), iLocation(aLocation), iItem(aItem), iPrice(aPrice) { }

	UserData(const UserData& aData) :
		iName(aData.iName), iLocation(aData.iLocation), iItem(aData.iItem), iPrice(aData.iPrice) { }

	UserData& operator=(const UserData& aData) {
		iName = aData.iName;
		iLocation = aData.iLocation;
		iItem = aData.iItem;
		iPrice = aData.iPrice;

		return *this;
	}
};

class CircularList {
public:
	CircularList(int aMaxEntry = MAX_LIST_SIZE) : iMaxSize(aMaxEntry-1) {
		iList = new UserData[aMaxEntry];
		clear();
	}

	void push_back(const UserData& aUserData);
	const UserData pop_front();
	const UserData front();

	inline int size() const {
		return iSize;
	}
	
	void clear() {
		iSize = 0;
		iFront = -1;
		iRear = -1;
	}

	bool empty() const {
		return (iSize == 0);
	}

private:
	UserData* iList;
	int iMaxSize;
	volatile int iSize;
	volatile int iFront;
	volatile int iRear;
};

class UserDataRecord {

public:

	UserDataRecord() : iWriteDone(FALSE), iReadDone(FALSE) {}

	inline void Reset() {
		iUserDataRecords.clear();
		iWriteDone = FALSE;
		iReadDone = FALSE;
	}

	inline void GetCount() const {
		iUserDataRecords.size();
	}

	inline void Write(const UserData& aUserData) {
		while (iUserDataRecords.size() >= MAX_WRITE_LIMIT) {
			Sleep(100);
		}
		iUserDataRecords.push_back(aUserData);
	}

	UserData Read();

	inline void FinishedWriting() {
		iWriteDone = TRUE;
		if (iUserDataRecords.empty()) {
			FinishedReading();
		}
	}

	inline BOOL ReadDone() {
		return iReadDone;
	}

	void ProcessUserData(string& anEntry);

private:
	inline void FinishedReading() {
		iReadDone = TRUE;
	}

private:
#ifdef USE_DEQUE
	deque<UserData> iUserDataRecords;
#else
	CircularList iUserDataRecords;
#endif
	volatile BOOL iWriteDone;
	volatile BOOL iReadDone;
};

class Tokenizer {

public:
	Tokenizer(const string& anEntry, const char* pDelimiter) : iIndex(0), iEntry(anEntry) {
		strcpy_s(iTokenDelimiter, pDelimiter);
		iDelimiterSize = strlen(iTokenDelimiter);
		iEntrySize = iEntry.size();
	}

	inline string GetToken() {
		iIndex = 0;
		return ParseToken();
	}

	inline string GetNextToken() {
		return ParseToken();
	}

private:
	string  ParseToken();

private:
	int iIndex;
	int iDelimiterSize;
	int iEntrySize;
	string iEntry;
	char iTokenDelimiter[10];
};

#endif
