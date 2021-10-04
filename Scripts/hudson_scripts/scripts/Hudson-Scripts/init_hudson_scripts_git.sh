#!/bin/bash

function init_git()
{
  echo "**********************************"
  echo " Fetching code for the first time"
  echo "**********************************"
  git clone ssh://sc-debu-git.marvell.com:29418/by-wks/hudson
}

function cherry_pick_git()
{
  common_script_path="/home/$USER/scripts/common"
  proj_path=$1
  CL_file=$2
  git clean -dfqx
  git reset --hard
  $common_script_path/git_cherry_pick.sh $proj_path $CL_file
}

function update_git()
{
  echo "**********************************"
  echo " Updating to TOT"
  echo "**********************************"
  git clean -dfqx
  git reset --hard
  git remote update
  git rebase remotes/origin/master
}


function main()
{
  local workspace=$1
  local hudson_root=$2
  local cherry_pick_file="$workspace/uploads/cherrypicks.txt"
  # if "git" is not created yet, do init now
  [ ! -d "$hudson_root/.git" ] && init_git

  cd $hudson_root
  # Now, see is there any cherry-pickings to be done or pull the TOT
  if [ -f "$cherry_pick_file" ]
  then
	#Cherry-pick
	cherry_pick_git $(pwd) $cherry_pick_file
  else
	# get the TOT
	update_git
  fi
}

#: ${1?"Usage: $0 WORKSPACE HUDSON_ROOT "}
echo "Usage: $0 WORKSPACE HUDSON_ROOT "
main "$@"
