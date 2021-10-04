#!/bin/bash

function init_git()
{
  mrvl_dir=$1
  bootflow_dir=$2
  git_branch=$3

  # force delete repo/git to clean checkout and build
  rm -rf $mrvl_dir
  rm -rf .repo
  rm -rf $bootflow_dir

  # init git for MRVL
  mrvl_git_init="repo init -u ssh://sc-debu-git.marvell.com:29418/manifest -b $git_branch -m ampsdk.xml"
  echo $mrvl_git_init
  $mrvl_git_init
  repo sync
  # clean any local modified files due to repo sync
  repo forall -c "git reset HEAD --hard; git clean -f -d"

  # init git for boot_flow
  bootflow_git_init="git clone ssh://sc-debu-git.marvell.com:29420/mrvl/boot_flow"
  echo $bootflow_git_init
  $bootflow_git_init
}

function update_git()
{
  mrvl_dir=$1
  bootflow_dir=$2

  repo forall -c "git reset HEAD --hard; git clean -f -d"
  repo sync
  # clean any local modified files due to repo sync
  repo forall -c "git reset HEAD --hard; git clean -f -d"

  cd $bootflow_dir
  git reset HEAD --hard
  git clean -f -d
  git pull origin
}

function main()
{
  mrvl_dir=${1-"MRVL"} #default is MRVL
  branch=${2-"dev_branch/2000"} #default is trunk
  bootflow_dir=${3-"boot_flow"}
  force_init_git=${4-"true"}

  echo "MRVL_DIR=$mrvl_dir"
  echo "BRANCH=$branch"
  echo "BOOT_FLOW_DIR=$bootflow_dir"
  echo "FORCE_INIT_GIT=$force_init_git"

  mrvl_inited="true"
  bootflow_inited="true"

  cd $WORKSPACE

  git_status=$( { repo list; } 2>&1 ) # check MRVL repo
  [ ".${git_status/error*/error}" == ".error" ] && mrvl_inited="false"

  cd $bootflow_dir

  git_status=$( { git status; } 2>&1 ) # check boot_flow git
  [ ".${git_status/fatal*/fatal}" == ".fatal" ] && bootflow_inited="false"

  [ "x$mrvl_inited" == "xfalse" ] || [ "x$bootflow_inited" == "xfalse" ] && force_init_git="true"

  echo "force_init_git=$force_init_git"

  cd $WORKSPACE

  if [ "x$force_init_git" == "xtrue" ]; then
    init_git $mrvl_dir $bootflow_dir $branch
  else
    update_git $mrvl_dir $bootflow_dir
  fi

  cd $WORKSPACE
}

#: ${1?"Usage: $0 MRVL_DIR [TAG]"}
echo "Usage: $0 MRVL_DIR [BRANCH] [TAG]"
main "$@"
