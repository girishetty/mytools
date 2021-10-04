#!/bin/bash

function init_git()
{
  echo "Initializing Android JBSDK....."
  repo init -u https://android.googlesource.com/platform/manifest -b android-4.2.2_r1
  repo sync
}

function update_git()
{
  echo "Updaring Android JBSDK....."
  echo 'repo forall -c "pwd && git clean -dfqx && git reset --hard && git remote update && git checkout"'
  repo forall -c "pwd && git clean -dfqx && git reset --hard && git remote update && git checkout"
  repo sync
}

function main()
{
  ANDROID_JBSDK=${1-"ANDROID_JBSDK"} #default is ANDROID_JBSDK
  [ ! -d "$ANDROID_JBSDK" ] && mkdir -p "$ANDROID_JBSDK"
  [ ! -d "$ANDROID_JBSDK" ] && echo "Invalid $ANDROID_JBSDK" && exit 1 # exit for bad path
  ANDROID_JBSDK=`cd $ANDROID_JBSDK; pwd` # get a good path

  cd $ANDROID_JBSDK
  [ ! -d "jbsdk" ] && mkdir -p "jbsdk"
  cd jbsdk

  M=$( { repo list; } 2>&1 ) # chk repo
  [ ".${M/error*/error}" == ".error" ] && init_git # manifest not installed, install it
  update_git
}

#: ${1?"Usage: $0 ANDROID_JBSDK "}
echo "Usage: $0 ANDROID_JBSDK "
main "$@"

