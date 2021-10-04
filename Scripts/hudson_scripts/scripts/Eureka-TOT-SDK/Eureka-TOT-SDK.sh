#!/bin/bash

echo "Hudson Env Variables"
echo "BUILD_NUMBER=$BUILD_NUMBER"
echo "BUILD_ID=$BUILD_ID"
echo "JOB_NAME=$JOB_NAME"
echo "WORKSPACE=$WORKSPACE"
echo "HUDSON_URL=$HUDSON_URL"
echo "JOB_URL=$JOB_URL"
echo "HUDSON_USER=$HUDSON_USER"

#Some more Variables
MRVL="MRVL"
#use "anchovy" for BG2CD and "berlin" for BG2
BLD_TYPE="anchovy" 
JobDir="/tftpboot/hudson/$JOB_NAME"
BaseDir="$JobDir/$BUILD_NUMBER"
ScriptDir="/home/$USER/scripts/$JOB_NAME"
#OTAPackage="$MRVL/Eureka/out/target/product/$BLD_TYPE/${BLD_TYPE}-ota-eng.${USER}.zip"
OTAPackage="$MRVL/Eureka/out/target/product/$BLD_TYPE/${BLD_TYPE}-ota-${BUILD_NUMBER}.zip"
UNANDimg="$MRVL/Eureka/out/target/product/$BLD_TYPE/uNAND_eureka-b1.img"
Dest_uNANDimg="uNAND_eureka-b1-$BUILD_NUMBER.img"

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

YMD=`date "+%Y_%m%d_%H%M%S"`
echo "YMD=$YMD" >> $SCRIPT_LOG
echo "BaseDir=$BaseDir" >> $SCRIPT_LOG
echo "OTAPackage=$OTAPackage" >> $SCRIPT_LOG
echo "UNANDimg=$UNANDimg" >> $SCRIPT_LOG

[ ! -d "$JobDir" ] && mkdir -p $JobDir 2>/dev/null
[ ! -d "$BaseDir" ] && mkdir -p "$BaseDir"
[ ! -d "$BaseDir" ] && echo "Invalid $BaseDir" && exit 1 # exit for bad path

#we are going to build the image for every BUILD Trigger
curr_dir=$(pwd)
echo "repo" >> $SCRIPT_LOG
# do repo int if there is no MRVL manifest
$ScriptDir/init_eureka_git.sh $MRVL && echo "init git" >> $SCRIPT_LOG

cd $MRVL
echo "generate_read_me.sh" >> $SCRIPT_LOG
# generate ReadMe_1055_mmddyy.sh
$ScriptDir/generate_read_me.sh $BaseDir $BUILD_NUMBER

#Build Marvell & Eureka SDK
#JIC - update the PATH to have cmake path avalilable for the tools
export PATH="/usr/cmake/bin:$PATH"
$ScriptDir/build_eureka_sdk.sh $ScriptDir $BLD_TYPE 
echo "Successfully Built Eureka Project" >> $SCRIPT_LOG
cd $curr_dir
echo "Done Building.."

echo "check if $OTAPackage exist" >> $SCRIPT_LOG
[ ! -f "$OTAPackage" ] && echo "No OTA Package created!" && exit 1
echo "OTA Package is available @ $OTAPackage" >> $SCRIPT_LOG
echo "cp $OTAPackage $BaseDir" >> $SCRIPT_LOG
cp "$OTAPackage" "$BaseDir"

echo "check if $UNANDimg exist" >> $SCRIPT_LOG
[ ! -f "$UNANDimg" ] && echo "No UNANDimg created!" && exit 1
echo "UNANDimg is available @ $UNANDimg" >> $SCRIPT_LOG
echo "cp $UNANDimg $BaseDir/$Dest_uNANDimg" >> $SCRIPT_LOG
cp "$UNANDimg" "$BaseDir/$Dest_uNANDimg"

ls -al $BaseDir >> $SCRIPT_LOG
ls -al $BaseDir 
echo "ftp the generated files to storage" >> $SCRIPT_LOG
echo "$ScriptDir/ftp_pkg_to_storage.sh $ScriptDir $BaseDir $JOB_NAME $BUILD_NUMBER "$JOB_URL$BUILD_NUMBER/"" >> $SCRIPT_LOG
$ScriptDir/ftp_pkg_to_storage.sh $ScriptDir $BaseDir $JOB_NAME $BUILD_NUMBER "$JOB_URL$BUILD_NUMBER/"
[ $? -gt 0 ] && exit 1

echo "cleanup after proper upload" >> $SCRIPT_LOG
echo "$ScriptDir/cleanup_post_build.sh $BaseDir $JOB_NAME $BUILD_NUMBER" >> $SCRIPT_LOG
#$ScriptDir/cleanup_post_build.sh $BaseDir $JOB_NAME $BUILD_NUMBER

