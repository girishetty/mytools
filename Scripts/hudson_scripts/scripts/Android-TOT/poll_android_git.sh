#!/bin/bash 

HUDSON_BLD_TRIGGER_URL="http://hudson.marvell.com:8080/job/Android-TOT/build?token=TimeToBuild"

latestGitCommit=""

function get_latest_git_commit()
{
  #Try to see is there any check-ins today
  local jb_root=$1
  local git_log_dump_file=$jb_root"/../git_log_dump.txt"
  local last_build_info_file=$jb_root"/../last_build_info.txt"
  local last_build_date=$(head -1 $last_build_info_file)
  local git_log_command="git log --oneline --after={$last_build_date}"
  echo "$git_log_command"
  { repo forall -c "$git_log_command"; } 2>&1 > $git_log_dump_file

  #Update the last commit details into the file, which is in the last line of the output file
  latestGitCommit=$(head -1 $git_log_dump_file)
}

function check_for_git_update()
{
  local need_build=0
  local jb_root=$(pwd)
  local lastCommitFileName=$jb_root"/../last_commit_file.txt"

  if [ ! -f "$lastCommitFileName" ] 
  then
	#We don't have a last_commit_file.txt yet, which means we need to build now ==> First time
  	need_build=1
  fi

  #Get the latest git commit log
  get_latest_git_commit $jb_root

  if [ $need_build -eq 0 ]
  then
  	#Now check the current latest Git Commit log with the saved last latest git commit log
	last_latestGitCommit=$(head -1 $lastCommitFileName)
	#we have a candidate for build if the commits are not the same
	if [ "$last_latestGitCommit" != "$latestGitCommit" ]
	then
		need_build=1
	fi
  fi

  if [ $need_build -eq 1 ]
  then
  	#Create/update the last_commit_file.txt with the commit info that we have got
  	echo "$latestGitCommit" > $lastCommitFileName
  fi

  return "$need_build"
}

function main()
{
  local JBROOT="JB_ROOT"
  cd $JBROOT

  local need_build=0
  check_for_git_update
  #Capture the return code of previous operation
  need_build=$?
  return "$issued_build"
}

#: ${0?"Usage: $0 "}
main "$@"
