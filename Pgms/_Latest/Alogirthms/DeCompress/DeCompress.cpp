# include <iostream>
using namespace std;

typedef unsigned char  TUint8;
typedef unsigned short TUint16;

/********************************************************************************
	Function Name		:  Decompress ()
	Inputs:
			TSRC*  src  :  Source Data Type ( 8, 16, 32, 64 bits Data)
			TDEST* dest :  Destination Data Type ( 8, 16, 32, 64 bits Data)
			scrSize     :  Source Data Bits Size
			destSize    :  Destination Data Bits Size
			len			:  No of Data in Source
	Return Value:
			Length of Destintion Data
 ********************************************************************************/

template<class TSRC, class TDEST>
int Decompress(TSRC* src, TDEST* dest, int srcSize, int destSize, int len){

	int		srcStartIndex, srcEndIndex, destIndex=0;
	int		rightShiftCnt;
	int		destDataBit;
	int		prevValue;
	int		available;
	int		prevBits;
	int		i, j;

	TSRC	data;
	TSRC	data1;

	for(srcStartIndex=0;srcStartIndex<len;srcStartIndex+=destSize) {
		srcEndIndex = srcStartIndex+(destSize-1);
		if(srcEndIndex > len)
			srcEndIndex = len;

		rightShiftCnt = destSize;
		prevValue = 0;
		prevBits=0;

		for(i=srcStartIndex;i<=srcEndIndex;i++){
			data = src [i];
			available=srcSize+prevBits;

			for(;available>destSize;available-=destSize) {
				rightShiftCnt = destSize-prevBits;
				for(destDataBit=1,j=1;j<rightShiftCnt;j++)
					destDataBit = (destDataBit<<1) | 1;

				data1 = data & destDataBit;
				data = data >> rightShiftCnt;

				data1 = data1 << prevBits;
				data1 = data1 | prevValue;

				dest[destIndex++]=(TDEST)data1;
				prevValue=0;
				prevBits=0;
			}
			prevValue = data;
			prevBits = available;
		}
		if(i-srcStartIndex==destSize)
			dest[destIndex++]=prevValue;
	}
	dest[--destIndex] = '\0';
	return destIndex;
}

/*******************************************************************************
	Function Name		:  Compress ()
	Inputs:
			TSRC*  src  :  Source Data Type ( 8, 16, 32, 64 bits Data)
			TDEST* dest :  Destination Data Type ( 8, 16, 32, 64 bits Data)
			scrSize     :  Source Data Bits Size
			destSize    :  Destination Data Bits Size
			len			:  No of Data in Source
	Return Value:
			Length of Destintion Data
 ********************************************************************************/

template<class TSRC, class TDEST>
int Compress(TSRC* src, TDEST* dest, int srcSize, int destSize, int len){

	int		srcStartIndex, destIndex=0;
	int		remainingBits=destSize;
	int		prevBits=0;
	int		shiftCount = 0;

	TDEST	prevValue=0;
	TDEST	data=0;
	TDEST	data1=0;

	for(srcStartIndex=0;srcStartIndex<len;) {
		data=0;
		shiftCount = 0;

		for(remainingBits=destSize;remainingBits>=srcSize;){
			if(prevBits == 0) {
				data1 = src[srcStartIndex++];
				data1 = (data1 << shiftCount);
				data = data | data1;
				shiftCount += srcSize;
				remainingBits-=srcSize;
			}
			else {
				data1 = prevValue;
				data1 = (data1 << shiftCount);
				data = data | data1;
				shiftCount += prevBits;
				remainingBits-=prevBits;
				prevValue = 0;
				prevBits = 0;
			}
		}
		if(remainingBits>0) {
			data1 = src[srcStartIndex++];
			prevValue = (data1 >> remainingBits);
			prevBits = (srcSize-remainingBits);
			data1 = (data1 << shiftCount);
			data = data | data1;
		}
		dest[destIndex++] = data;
	}

	if(prevBits > 0)
		dest[destIndex++]=prevValue;

	dest[destIndex]='\0';
	return destIndex;
}

int main(){
	//TUint8  src[11] = {0xC8,0x32,0x9B, 0xFD, 0x66, 0x81, 0xEE, 0x6F, 0x39, 0x9B, 0x0C};
	TUint16  src[4] = {0xD256,0x25CA,0xC601, 0x3A69};
	TUint8  dest[20];

	int index=0;
	int lastIndex;
	int len = 4;//11;

	lastIndex = Decompress(src, dest, 16, 7, len);
	//lastIndex = Decompress(src, dest, 16, 8, len);
	//lastIndex = Decompress(src, dest, 8, 7, len);
	cout<<endl<<"After: Dest Data : "<<dest<<endl;
	
	return 0;
}


