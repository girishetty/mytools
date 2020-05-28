#!/bin/bash

GitTag=$1 #"dev_tags/sandra/1002/BG2CD_2013-02-26_B"
GitBranch=$2 #GitBranch="trunk"
BuildSDKScript=$3

echo "Hudson Env Variables"
echo "BUILD_NUMBER=$BUILD_NUMBER"
echo "BUILD_ID=$BUILD_ID"
echo "JOB_NAME=$JOB_NAME"
echo "WORKSPACE=$WORKSPACE"
echo "HUDSON_URL=$HUDSON_URL"
echo "JOB_URL=$JOB_URL"
echo "HUDSON_USER=$HUDSON_USER"

MRVL="MRVL"
SBT_VER="1055_"
JobDir="/tftpboot/hudson/$JOB_NAME"
BaseDir="$JobDir/$BUILD_NUMBER"
ScriptDir="/home/$USER/scripts/$JOB_NAME"
TarBall="linux_sdk_$SBT_VER${BUILD_NUMBER}_bg2cd_dngle.tgz"
BuildScript="$BaseDir/build_$SBT_VER$BUILD_NUMBER.sh"

echo "TarBall=$TarBall"
echo "BuildScript=$BuildScript"
echo "GitBranch=$GitBranch"
echo "GitTag=$GitTag"
echo "BuildSDKScript=$BuildSDKScript"

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
echo "TarBall=$TarBall" >> $SCRIPT_LOG
echo "BuildScript=$BuildScript" >> $SCRIPT_LOG
echo "TarBall=$TarBall"

# If there is no tar ball, then build it
if [ ! -f "$TarBall" ]; then
	echo "repo" >> $SCRIPT_LOG
	#forece delete for clean checkout and build.
        rm -rf $MRVL
	rm -rf .repo
	# do repo int if there is no MRVL manifest
	$ScriptDir/init_anchovy_git.sh $MRVL $GitBranch $GitTag && echo "init git" >> $SCRIPT_LOG

	echo "generate_read_me.sh" #>> $SCRIPT_LOG
	# generate ReadMe_1055_<build_number>.sh
	$ScriptDir/generate_read_me.sh $BaseDir "$SBT_VER$BUILD_NUMBER" $MRVL

#	BuildSDK="./$MRVL/build_scripts/toplevel_scripts/batch_anchovy_bg2cd.sh"
#	BuildSDK="./$MRVL/build_scripts/toplevel_scripts/batch_anchovy_bg2cd_linux38.sh"
	BuildSDK="./$MRVL/build_scripts/toplevel_scripts/$BuildSDKScript"
	[ ! -f "$BuildSDK" ] && echo "No build script" && exit 1
	echo "$BuildSDK" >> $SCRIPT_LOG
	"$BuildSDK" $BUILD_NUMBER
	[ $? -gt 0 ] && exit 1
	echo "Success ---- $BuildSDK" >> $SCRIPT_LOG
fi

echo "check if $TarBall exist" >> $SCRIPT_LOG
[ ! -f "$TarBall" ] && echo "No tar ball" && exit 1
echo "destination is $BaseDir" >> $SCRIPT_LOG
mkdir -p $BaseDir 2>/dev/null
echo "cp $TarBall to $BaseDir" >> $SCRIPT_LOG
cp $TarBall $BaseDir

echo "generate_build_script" >> $SCRIPT_LOG
$ScriptDir/generate_build_script.sh $BuildScript $BaseDir "$SBT_VER$BUILD_NUMBER"
echo "run build script $BuildScript" >> $SCRIPT_LOG
$BuildScript
[ $? -gt 0 ] && exit 1

ls -al $BaseDir >> $SCRIPT_LOG
ls -al $BaseDir 
echo "ftp the generated files to cloud storage" >> $SCRIPT_LOG
echo "$ScriptDir/ftp_pkg_to_storage.sh $ScriptDir $BaseDir $JOB_NAME $BUILD_NUMBER $JOB_URL$BUILD_NUMBER/" >> $SCRIPT_LOG
$ScriptDir/ftp_pkg_to_storage.sh $ScriptDir $BaseDir $JOB_NAME $BUILD_NUMBER "$JOB_URL$BUILD_NUMBER/"
[ $? -gt 0 ] && exit 1

echo "cleanup after proper upload" >> $SCRIPT_LOG
echo "$ScriptDir/cleanup_post_build.sh $BaseDir $JOB_NAME $BUILD_NUMBER" >> $SCRIPT_LOG
$ScriptDir/cleanup_post_build.sh $BaseDir $JOB_NAME $BUILD_NUMBER
