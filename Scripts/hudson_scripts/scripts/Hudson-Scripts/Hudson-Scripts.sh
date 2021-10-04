#!/bin/bash

echo "Hudson Env Variables"
echo "BUILD_NUMBER=$BUILD_NUMBER"
echo "BUILD_ID=$BUILD_ID"
echo "JOB_NAME=$JOB_NAME"
echo "WORKSPACE=$WORKSPACE"
echo "HUDSON_URL=$HUDSON_URL"
echo "JOB_URL=$JOB_URL"
echo "HUDSON_USER=$HUDSON_USER"

ScriptDir="/home/$USER/scripts/$JOB_NAME"
JobDir="/tftpboot/hudson/$JOB_NAME"
BaseDir="$JobDir/$BUILD_NUMBER"
SCRIPT_LOG=`pwd`"/script.log"
HUDSON_ROOT="hudson"

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

echo "repo" >> $SCRIPT_LOG
# do repo int if there is no HUDSON_ROOT manifest
$ScriptDir/init_hudson_scripts_git.sh $WORKSPACE $HUDSON_ROOT && echo "init git" >> $SCRIPT_LOG

cd $HUDSON_ROOT
echo "generate_read_me.sh" >> $SCRIPT_LOG
$ScriptDir/generate_read_me.sh $BaseDir $BUILD_NUMBER

#Any build related activities here

#Updated the last successful build info details
last_build_info_file="$WORKSPACE/last_build_info.txt"
yesterday_mon=`date "+%Y-%m-"`
yesterday=`date "+%d"`
let yesterday-=1
last_build_date=$yesterday_mon$yesterday
echo "$last_build_date" > $last_build_info_file
