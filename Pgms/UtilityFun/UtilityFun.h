#ifndef UTILITYFUN_H

bool IsPowerOf2(int aNum) {
	if(aNum == 0)
		return true;
	else {
		//If its -ve number, make it +ve
		if(aNum < 0)
			aNum = -aNum;
		return (!((aNum) & (aNum-1)));
	}
}

bool IsOddNumber(int aNum) {
	return ((aNum & 0x01));
}

#endif //UTILITYFUN_H