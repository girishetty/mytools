#!/bin/bash

function init_Mrvl_git()
{
  echo 'Fetching Mrvl Code.....'
  repo init -b anchovy-22oct2012 -m marvellsdk.xml -u https://eureka-partner.googlesource.com/partner/manifest
  repo sync -j20
}

function init_Eureka_git()
{
  echo 'Fetching Eureka Code.....'
  repo init -u https://eureka-partner.googlesource.com/partner/manifest
  repo sync -j20
}

function update_git()
{
  echo 'repo forall -c "pwd && git clean -dfqx && git reset --hard && git remote update && git checkout"'
  repo forall -c "pwd && git clean -dfqx && git reset --hard && git remote update && git checkout"
  repo sync -j20
}


function main()
{
  MRVL_DIR=${1-"MRVL"} #default is MRVL
  [ ! -d "$MRVL_DIR" ] && mkdir -p "$MRVL_DIR"
  [ ! -d "$MRVL_DIR" ] && echo "Invalid $MRVL_DIR" && exit 1 # exit for bad path
  MRVL_DIR=`cd $1; pwd` # get a good path

  echo "cd $MRVL_DIR"
  cd $MRVL_DIR
  [ ! -d "Mrvl" ] && mkdir -p "Mrvl"
  [ ! -d "Eureka" ] && mkdir -p "Eureka"
  echo "cd Mrvl"
  cd Mrvl
  M=$( { repo list; } 2>&1 ) # chk repo
  [ ".${M/error*/error}" == ".error" ] && init_Mrvl_git # manifest not installed, install it
  echo "Updating Mrvl Project..."
  update_git # get the tagged tree

  cd $MRVL_DIR
  cd Eureka
  M=$( { repo list; } 2>&1 ) # chk repo
  [ ".${M/error*/error}" == ".error" ] && init_Eureka_git # manifest not installed, install it
  #cleanup the "out" folder if its existing already, to avoid build errors
  rm -rf out
  echo "Updating Eureka Project..."
  update_git # get the tagged tree

  #For some reasons, some of the directories are not checked-out now, do it manually now, before giving a build
  cp -r /home/hudson-sc-debu/PURU/Eureka/chromium/src/third_party/sfntly/cpp chromium/src/third_party/sfntly/.
  cp -r /home/hudson-sc-debu/PURU/Eureka/chromium/src/third_party/jsoncpp/source chromium/src/third_party/jsoncpp/.
  echo "done copying..."
  cd $MRVL_DIR
}

#: ${1?"Usage: $0 MRVL_DIR "}
echo "Usage: $0 MRVL_DIR"
main "$@"
