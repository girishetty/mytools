#!/bin/bash

#TOT="remotes/sc-debu-git/dev_branch/1002"
#TOT="dev_tags/sandra/1002/BG2CD_2013-02-26_B"
gerrit_svr="sc-debu-git.marvell.com"

function init_git()
{
  GitBranch=$1
#  echo 'repo init -u ssh://$gerrit_svr//home/gerrit2/review_site/git/manifest.git --repo-url=ssh://$gerrit_svr/home/gerrit2/review_site/git/repo.git -b $GitBranch'
#  repo init -u ssh://$gerrit_svr//home/gerrit2/review_site/git/manifest.git --repo-url=ssh://$gerrit_svr/home/gerrit2/review_site/git/repo.git -b $GitBranch
  echo 'repo init -u ssh://$gerrit_svr:29418/manifest -b $GitBranch'
  repo init -u ssh://$gerrit_svr:29418/manifest -b $GitBranch
  repo sync
}

function update_git()
{
  echo 'repo forall -c "'pwd' && git clean -dfqx && git reset --hard && git remote update && git checkout $1"'
  repo forall -c "pwd && git clean -dfqx && git reset --hard && git remote update && git checkout $1"
  repo sync
}


function main()
{
  MRVL_DIR=${1-"MRVL"} #default is MRVL
  BRANCH=${2-"trunk"} #default is trunk
  TAG=${3-"remotes/sc-debu-git/dev_branch/1002"} # use TOT if TAG not specified

  echo "MRVL_DIR=$MRVL_DIR"
  echo "BRANCH=$BRANCH"
  echo "TAG=$TAG"

#this creates MRVL unsder MRVL directory as the repo is creating MRVL dir again.
#  [ ! -d "$MRVL_DIR" ] && mkdir -p "$MRVL_DIR"
#  [ ! -d "$MRVL_DIR" ] && echo "Invalid $MRVL_DIR" && exit 1 # exit for bad path
  MRVL_DIR=`cd $1; pwd` # get a good path
  MY_DIR=`pwd`
  echo "MRVL_DIR=$MRVL_DIR"
  echo "MY_DIR=$MY_DIR"

#  cd $MRVL_DIR
  M=$( { repo list; } 2>&1 ) # chk repo
  [ ".${M/error*/error}" == ".error" ] && init_git $BRANCH # manifest not installed, install it
#  update_git $TAG # get the tagged tree
  cd $MY_DIR #done and out
}

#: ${1?"Usage: $0 MRVL_DIR [TAG]"}
echo "Usage: $0 MRVL_DIR [BRANCH] [TAG]"
main "$@"
