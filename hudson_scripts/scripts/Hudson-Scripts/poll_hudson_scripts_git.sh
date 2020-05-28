#!/bin/bash

function check_for_git_update()
{
  local need_build=0
  local hudson_root=$(pwd)
  local git_log_dump_file=$hudson_root"/../git_log_dump.txt"
  local git_log_command="git log --oneline master..remotes/origin/master"
  $git_log_command 2>&1 > $git_log_dump_file

  #Get the latest commit details, if there is one from the output file
  local latestGitCommit=$(head -1 $git_log_dump_file)

  #we have a candidate for build if the git log has any contents
  if [ "x$latestGitCommit" != "x" ]; then
	need_build=1
  fi

  return "$need_build"
}

function main()
{
  hudson_root="hudson"
  cd $hudson_root

  local need_build=0
  check_for_git_update
  #Capture the return code of previous operation
  need_build=$?
  return "$need_build"
}

#: ${0?"Usage: $0 "}
main "$@"
