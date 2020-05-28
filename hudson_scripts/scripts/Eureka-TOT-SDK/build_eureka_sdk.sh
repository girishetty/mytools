#!/bin/bash

function main()
{
	SCRIPTS_DIR=$1
	BLD_TYPE=${2-"anchovy"}   #use "anchovy" for BG2CD and "berlin" for BG2
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "BLD_TYPE=$BLD_TYPE"

#########################################################################
#hack  - to be deleted later
#CD2Loc="Mrvl/sdk/prebuiltfiles/Anchovy_BG2CD_A0/lib/libDRM"
#cd Mrvl/sdk/prebuiltfiles/Anchovy_BG2CD_A0/lib/libDRM/
#[ ! -f "$CD2Loc/libDRM-old.so" ] && cp $CD2Loc/libDRM.so $CD2Loc/libDRM-old.so
#cp $CD2Loc/libDRM.so-eureka $CD2Loc/libDRM.so

#echo `ls -l $CD2Loc`


#CD2Loc="Mrvl/sdk/GaloisSoftware/build/configs/libCrypto/prebuilt/lib"
#cd Mrvl/sdk/GaloisSoftware/build/configs/libCrypto/prebuilt/lib
#[ ! -f "$CD2Loc/libCrypto-old.so" ] && cp $CD2Loc/libCrypto.so $CD2Loc/libCrypto-old.so
#cp $CD2Loc/libCrypto.so-eureka $CD2Loc/libCrypto.so

#echo `ls -l $CD2Loc`

#########################################################################

	#Build Marvell SDK
	cd Mrvl/sdk
	BuildMrvlSDK="./build_scripts/build_all.sh"
	[ ! -f "$BuildMrvlSDK" ] && echo "No build script" && exit 1
	echo "Starting ---- $BuildMrvlSDK" 
	"$BuildMrvlSDK" $BLD_TYPE ../../Eureka
	[ $? -gt 0 ] && exit 1
	echo "Success ---- $BuildMrvlSDK"

	#Build Eureka SDK
	cd ../../Eureka
	#be sure to remove the out directory after updating any of the prebuilt binaries as timestamp differences may prevent the Makefiles from using the updated binaries.
	rm -rf out
	echo "Building Google's Eureka SDK"
	source build/envsetup.sh
	echo "lunch $BLD_TYPE-eng"
	lunch $BLD_TYPE-eng
	echo "make -j22"
	make -j22
	[ $? -gt 0 ] && exit 1
	echo "Success Building Google's Eureka SDK"
	
	echo "Building OTApackage"
	make otapackage -j22
	[ $? -gt 0 ] && exit 1
	echo "Success Building OTApackage"

	$SCRIPTS_DIR/setup_permissions.sh $SCRIPTS_DIR

	echo "Building UNANDimg"
	make u2nandimg -j22
	[ $? -gt 0 ] && exit 1
	echo "Success Building UNANDimg"
	cd ..
}

#: ${1?"Usage: $0 BLD_TYPE "}
echo "Usage: $0 SCRIPTS_DIR BLD_TYPE "
main "$@"

