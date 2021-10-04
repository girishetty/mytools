#!/bin/bash

function add_marvell_sdk()
{
    local MARVELL_SDK_PATH=$1
    local SCRIPT_LOG=$2
    echo "remove any pre-existing Marvell SDK from Android JBSDK....." >> $SCRIPT_LOG
    rm -rf jbsdk/vendor/marvell
    rm -rf jbsdk/vendor/marvell-sdk
    echo "Untar MARVELL SDK ontop of Android JBSDK....." >> $SCRIPT_LOG
    tar -zxf $MARVELL_SDK_PATH
}

function build_android_sdk()
{
    local SCRIPT_LOG=$1
    local OPTION=$2
    local BuildSDK="./vendor/marvell/build/build_jb"

    echo "Now build the Android AOSP JBSDK" >> $SCRIPT_LOG
    cd jbsdk
    cp vendor/marvell/build/configs/bg2cd_dmp_nand/* vendor/marvell/build/
    [ ! -f "$BuildSDK" ] && echo "No build script" && exit 1

    if [ "is$OPTION" = "isf" ]; then
        #cleanup out/target and build everything
        rm -rf jbsdk/out/target/product
        echo "$BuildSDK" >> $SCRIPT_LOG
        "$BuildSDK"
    else
        #build android only
        STEPS="-sandroid,image,ota,copy_result"
        echo "$BuildSDK $STEPS" >> $SCRIPT_LOG
        "$BuildSDK" "$STEPS"
    fi
    echo "Success ---- $BuildSDK" >> $SCRIPT_LOG
}

function copy_sdk_ota_image()
{
    local MARVELL_RELEASE_SDK=$1
    local BaseDir=$2
    local Dest_uNANDimg=$3
    local Dest_OTA_Package=$4
    local Dest_Marvell_Release_SDK=$5
    local BUILD_NUMBER=$6
    local SCRIPT_LOG=$7

    local UNANDimg="vendor/marvell/build/Release_image/u2nandimg/uNAND.img"
    local OTA_Package="vendor/marvell/build/Release_image/online_upgrade_img/bg2cd_dmp_nand-ota-$BUILD_NUMBER.zip"

    #Copy uNAND, OTA, marvell_sdk to FTP Server
    echo "Copy uNAND, OTA, marvell_sdk to FTP Server" >> $SCRIPT_LOG
    echo "destination is $BaseDir" >> $SCRIPT_LOG
    mkdir -p $BaseDir 2>/dev/null

    echo "check if $UNANDimg exist" >> $SCRIPT_LOG
    [ ! -f "$UNANDimg" ] && echo "No UNANDimg created!" && exit 1
    echo "UNANDimg is available @ $UNANDimg" >> $SCRIPT_LOG
    echo "cp $UNANDimg $BaseDir/$Dest_uNANDimg" >> $SCRIPT_LOG
    cp "$UNANDimg" "$BaseDir/$Dest_uNANDimg"

    #Commenting out OTA part as OTA doesnt seem to work now!
    #echo "check if $OTA_Package exist" >> $SCRIPT_LOG
    #echo "check if $OTA_Package exist"
    #[ ! -f "$OTA_Package" ] && echo "No OTA Package created!" && exit 1
    #echo "OTA Package is available @ $OTA_Package" >> $SCRIPT_LOG
    #echo "cp $OTA_Package $BaseDir/$Dest_OTA_Package" >> $SCRIPT_LOG
    #cp "$OTA_Package" "$BaseDir/$Dest_OTA_Package"

    echo "cp $MARVELL_RELEASE_SDK $BaseDir/$Dest_Marvell_Release_SDK" >> $SCRIPT_LOG
    cp "$MARVELL_RELEASE_SDK" "$BaseDir/$Dest_Marvell_Release_SDK"
}

function main()
{
    ANDROID_JBSDK=${1-"ANDROID_JBSDK"} #default is ANDROID_JBSDK
    ANDROID_JBSDK=`cd $ANDROID_JBSDK; pwd` # get a good path
    cd $ANDROID_JBSDK

    #Add the Marvell Release SDK
    add_marvell_sdk $2 $8

    #Now build the Android AOSP JBSDK
    build_android_sdk $8 $9

    #Now Copy the release-sdk, OTA and Image to Ftp Server
    copy_sdk_ota_image $2 $3 $4 $5 $6 $7 $8
}

#: ${1?"Usage: $0 ANDROID_JBSDK MARVELL_SDK BaseDir Dest_uNANDimg Dest_OTA_Package Dest_Marvell_Release_SDK BUILD_NUMBER SCRIPT_LOG OPTION "}
echo "Usage: $0 ANDROID_JBSDK MARVELL_SDK BaseDir Dest_uNANDimg Dest_OTA_Package Dest_Marvell_Release_SDK BUILD_NUMBER SCRIPT_LOG OPTION"
main "$@"
