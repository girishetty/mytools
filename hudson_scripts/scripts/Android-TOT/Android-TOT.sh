#!/bin/bash

#Script that drives Android/JB Build Process

GitTag=$1 #"dev_tags/sandra/1002/BG2CD_2013-02-26_B"

echo "Hudson Env Variables"
echo "BUILD_NUMBER=$BUILD_NUMBER"
echo "BUILD_ID=$BUILD_ID"
echo "JOB_NAME=$JOB_NAME"
echo "WORKSPACE=$WORKSPACE"
echo "HUDSON_URL=$HUDSON_URL"
echo "JOB_URL=$JOB_URL"
echo "HUDSON_USER=$HUDSON_USER"

JB_ROOT="JB_ROOT"
ANDROID_JBSDK="ANDROID_JBSDK"
SBT_VER="1055_"
JobDir="/tftpboot/hudson/$JOB_NAME"
BaseDir="$JobDir/$BUILD_NUMBER"
ScriptDir="/home/$USER/scripts/$JOB_NAME"

UNANDimg="vendor/marvell/build/Release_image/u2nandimg/uNAND.img"
Dest_uNANDimg="bg2cd_uNAND_$BUILD_NUMBER.img"
Dest_OTA_Package="bg2cd_nand-ota_$BUILD_NUMBER.zip"
Dest_Marvell_Release_SDK="bg2cd_jbsdk_$BUILD_NUMBER.tgz"

Dest_uNANDimg_MC="bg2cd_uNAND_MC_$BUILD_NUMBER.img_MC"
Dest_OTA_Package_MC="bg2cd_nand-ota_MC_$BUILD_NUMBER.zip"
Dest_Marvell_Release_SDK_MC="bg2cd_jbsdk_MC_$BUILD_NUMBER.tgz"

mkdir -p $JobDir 2>/dev/null

SCRIPT_LOG=`pwd`"/script.log"
echo "script begin" > $SCRIPT_LOG
date >> $SCRIPT_LOG
ls -la >> $SCRIPT_LOG
pwd >> $SCRIPT_LOG
echo >> $SCRIPT_LOG

echo "Hudson Env Variables" >> $SCRIPT_LOG
echo "BUILD_NUMBER=$BUILD_NUMBER" >> $SCRIPT_LOG
echo "BUILD_ID=$BUILD_ID" >> $SCRIPT_LOG
echo "JOB_NAME=$JOB_NAME" >> $SCRIPT_LOG
echo "WORKSPACE=$WORKSPACE" >> $SCRIPT_LOG
echo "HUDSON_URL=$HUDSON_URL" >> $SCRIPT_LOG
echo "JOB_URL=$JOB_URL" >> $SCRIPT_LOG
echo "HUDSON_USER=$HUDSON_USER" >> $SCRIPT_LOG
echo >> $SCRIPT_LOG
echo "BaseDir=$BaseDir" >> $SCRIPT_LOG

echo "repo" >> $SCRIPT_LOG
# do repo int if there is no JB_ROOT manifest
$ScriptDir/init_android_git.sh $JB_ROOT $GitTag && echo "init git" >> $SCRIPT_LOG

CURR_DIR=`pwd`
echo "generate_read_me.sh" >> $SCRIPT_LOG
cd $JB_ROOT/
# generate ReadMe_1055_mmddyy.sh
$ScriptDir/generate_read_me.sh $BaseDir "$SBT_VER_$BUILD_NUMBER"

#********************************************
# Build Complete JBSDK
#********************************************
echo "============Building Complete SDK==============" >> $SCRIPT_LOG
date >> $SCRIPT_LOG
#cleanup the SDK to save some space in the server
rm bg2cd_dmp_nand.release/*

#Copy the local copy of build_jb which doesn't build ota
cp ../build_jb vendor/marvell/build/configs/bg2cd_dmp_nand/
cp vendor/marvell/build/configs/bg2cd_dmp_nand/* vendor/marvell/build/
BuildSDK="./vendor/marvell/build/build_jb"
[ ! -f "$BuildSDK" ] && echo "No build script" && exit 1
echo "$BuildSDK" >> $SCRIPT_LOG
"$BuildSDK"
#Verify if the Build Succeeded
echo "check if the Build Succeeded" >> $SCRIPT_LOG
[ ! -f "$UNANDimg" ] && echo "Build Failed!!" && exit 1
echo "Success ---- $BuildSDK" >> $SCRIPT_LOG

#Updated the last successful build info details to keep track of commit messages
last_build_info_file="$WORKSPACE/last_build_info.txt"
yesterday_mon=`date "+%Y-%m-"`
yesterday=`date "+%d"`
let yesterday-=1
#delibarately make it as yesterday as we are looking for commits "after" yesterday
last_build_date=$yesterday_mon$yesterday
echo "$last_build_date" > $last_build_info_file

#********************************************
# Build Release SDK
#********************************************
echo "============Building Release SDK ==============" >> $SCRIPT_LOG
date >> $SCRIPT_LOG
BuildReleaseSDK="./vendor/marvell/development/tools/release_jb.sh"
BLD_OPTIONS="-ry -eMIRACAST_SRC -sprebuild_for_release,deploy_release_env,tar_release_package"
"$BuildReleaseSDK" "$BLD_OPTIONS"
echo "Success ---- $BuildReleaseSDK $BLD_OPTIONS" >> $SCRIPT_LOG

#Verify the Build
echo "Verify the Build" >> $SCRIPT_LOG
TODAY=`date "+%Y%m%d"`
MARVELL_RELEASE_SDK="$CURR_DIR/$JB_ROOT/bg2cd_dmp_nand.release/jbsdk.$TODAY.tgz"
echo "check if $MARVELL_RELEASE_SDK exist" >> $SCRIPT_LOG
[ ! -f "$MARVELL_RELEASE_SDK" ] && echo "Marvell Release SDK without Miracast Not Created!!" && exit 1
echo >> $SCRIPT_LOG

#Now setup Android AOSP JBSDK
echo "============Now setup Android AOSP JBSDK==============" >> $SCRIPT_LOG
date >> $SCRIPT_LOG
cd $CURR_DIR
echo "setup_android_aosp_jbsdk.sh" >> $SCRIPT_LOG
$ScriptDir/setup_android_aosp_jbsdk.sh $ANDROID_JBSDK

#Now Add Marvell Release SDK ontop of Android SDK, build and Copy Image, OTA, SDK
echo "===========Now Add Marvell Release SDK ontop of Android SDK, build and Copy Image, OTA, SDK=========" >> $SCRIPT_LOG
date >> $SCRIPT_LOG
cd $CURR_DIR
OPTION="f"  #build full
echo "build_copy_android_aosp_jbsdk.sh" >> $SCRIPT_LOG
$ScriptDir/build_copy_android_aosp_jbsdk.sh $ANDROID_JBSDK $MARVELL_RELEASE_SDK $BaseDir $Dest_uNANDimg $Dest_OTA_Package $Dest_Marvell_Release_SDK $BUILD_NUMBER $SCRIPT_LOG $OPTION
echo >> $SCRIPT_LOG
date >> $SCRIPT_LOG
echo >> $SCRIPT_LOG

ls -al $BaseDir >> $SCRIPT_LOG
ls -al $BaseDir
echo "ftp the generated files to cloud storage" >> $SCRIPT_LOG
echo "$ScriptDir/ftp_pkg_to_storage.sh $ScriptDir $BaseDir $JOB_NAME $BUILD_NUMBER $JOB_URL$BUILD_NUMBER/" >> $SCRIPT_LOG
$ScriptDir/ftp_pkg_to_storage.sh $ScriptDir $BaseDir $JOB_NAME $BUILD_NUMBER "$JOB_URL$BUILD_NUMBER/"
[ $? -gt 0 ] && exit 1

echo "cleanup after proper upload" >> $SCRIPT_LOG
echo "$ScriptDir/cleanup_post_build.sh $BaseDir $JOB_NAME $BUILD_NUMBER" >> $SCRIPT_LOG
$ScriptDir/cleanup_post_build.sh $BaseDir $JOB_NAME $BUILD_NUMBER

