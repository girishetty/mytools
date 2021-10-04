#!/bin/bash

GitBranch=$1
BuildSrcScript=$2
ForceInitGit=$3
DoRelease=$4

echo "Hudson Env Variables"
echo "BUILD_NUMBER=$BUILD_NUMBER"
echo "BUILD_ID=$BUILD_ID"
echo "JOB_NAME=$JOB_NAME"
echo "WORKSPACE=$WORKSPACE"
echo "HUDSON_URL=$HUDSON_URL"
echo "JOB_URL=$JOB_URL"
echo "HUDSON_USER=$HUDSON_USER"

MRVL_DIR="$WORKSPACE/MRVL"
BOOT_FLOW_DIR="$WORKSPACE/boot_flow"
BRANCH_VER="2000"
JobDir="/tftpboot/hudson/$JOB_NAME"
BaseDir="$JobDir/$BUILD_NUMBER"
ScriptDir="/home/$USER/scripts/$JOB_NAME"
TarBall="linux_sdk_${BRANCH_VER}_${BUILD_NUMBER}_bg2cdp_dngle.tgz"
BuildScript="$BaseDir/build_${BRANCH_VER}_${BUILD_NUMBER}.sh"

echo "TarBall=$TarBall"
echo "BuildScript=$BuildScript"
echo "GitBranch=$GitBranch"
echo "BuildSrcScript=$BuildSrcScript"

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

	$ScriptDir/init_anchovy_cdp_git.sh $MRVL_DIR $GitBranch $BOOT_FLOW_DIR $ForceInitGit

	echo "generate_read_me.sh" #>> $SCRIPT_LOG
	# generate ReadMe_<branch_tag>_<build_number>.sh
	$ScriptDir/generate_read_me.sh $BaseDir "${BRANCH_VER}_${BUILD_NUMBER}" $MRVL

	cd $WORKSPACE

	BuildSDK="$MRVL_DIR/build_scripts/$BuildSrcScript"
	[ ! -f "$BuildSDK" ] && echo "No build script: $BuildSDK" && exit 1
	echo "$BuildSDK" >> $SCRIPT_LOG
	"$BuildSDK" $DoRelease $BUILD_NUMBER
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
$ScriptDir/generate_build_script.sh $BuildScript $BaseDir "${BRANCH_VER}_${BUILD_NUMBER}"
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
