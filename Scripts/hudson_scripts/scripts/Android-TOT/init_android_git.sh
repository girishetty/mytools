#!/bin/bash

#TOT="remotes/sc-debu-git/dev_branch/1002"
#TOT="dev_tags/sandra/1002/BG2CD_2013-02-26_B"
gerrit_svr="sc-debu-git.marvell.com"
Git_Branch="os/android/0901"

function init_git()
{
  echo "**********************************"
  echo " Fetching code for the first time"
  echo "**********************************"
  repo init -u ssh://$gerrit_svr/git/by-os/android/manifest.git --repo-url=ssh://$gerrit_svr/git/repo.git -b $Git_Branch
  repo sync
}

function cherry_pick_git()
{
  common_script_path="/home/$USER/scripts/common"
  proj_path=$1
  CL_file=$2
  repo forall -c "pwd && git clean -dfqx && git reset --hard"
  $common_script_path/git_cherry_pick.sh $proj_path $CL_file
}

function update_git()
{
  echo "**********************************"
  echo " Updating to TOT"
  echo "**********************************"
  echo 'repo forall -c "pwd && git clean -dfqx && git reset --hard && git remote update && git checkout"'
  repo forall -c "pwd && git clean -dfqx && git reset --hard && git remote update && git checkout"
  repo sync
}

function main()
{
  JB_ROOT=${1-"JB_ROOT"} #default is MRVL
  TAG=${2-"remotes/sc-debu-git/dev_branch/1002"} # use TOT if TAG not specified
  local workspace=$(pwd)
  local cherry_pick_file="$workspace/uploads/cherrypicks.txt"

  [ ! -d "$JB_ROOT" ] && mkdir -p "$JB_ROOT"
  [ ! -d "$JB_ROOT" ] && echo "Invalid $JB_ROOT" && exit 1 # exit for bad path
  JB_ROOT=`cd $1; pwd` # get a good path
  MY_DIR=`pwd`

  cd $JB_ROOT
  M=$( { repo list; } 2>&1 ) # chk repo
  [ ".${M/error*/error}" == ".error" ] && init_git # manifest not installed, install it

  # Now, see is there any cherry-pickings to be done or pull the TOT
  if [ -f "$cherry_pick_file" ]
  then
        #Cherry-pick
        cherry_pick_git $(pwd) $cherry_pick_file
  else
        # get the TOT
        update_git $TAG
  fi

  cd $MY_DIR #done and out
}

#: ${1?"Usage: $0 JB_ROOT [TAG]"}
echo "Usage: $0 JB_ROOT [TAG]"
main "$@"
