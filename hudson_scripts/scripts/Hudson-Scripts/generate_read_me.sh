#!/bin/bash

function gen_readme_file()
{
  local mmddyy=`date "+%m%d%y_%H%M%S"`
  local out_file=$1

  echo "Date Tag: $mmddyy" >$out_file

  # Get the details of HudsonScripts Project now
  echo "Project: HudsonBuildScripts :" >>$out_file

  #Update the project check-in detailsby lookig for git logs
  #MSG=$( { git log --oneline --after=2013-04-15; } 2>&1 )
  MSG=$( { git log --oneline; } 2>&1 )
  if [ "x${MSG/fatal */fatal}" != "xfatal" ]; then
     echo "" >>$out_file
     echo "$proj:" >>$out_file
     echo "$MSG" >>$out_file
  fi

  #Try to see is there any check-ins since last build
  #echo "" >>$out_file
  #local last_build_info_file=$jb_root"/../last_build_info.txt"
  #local last_build_date=$(head -1 $last_build_info_file)
  #local git_log_command="pwd && git log --oneline --after={$last_build_date}"
  #local git_log_command="pwd && git log --oneline -n 1"
  #{ repo forall -c "$git_log_command"; } 2>&1 >> $out_file

  echo "" >>$out_file
  echo "Generated $out_file for HudsonBuildScripts Project"
}

function main()
{
  BaseDir=$1
  BLD_NO=${2-"git"}

  [ ! -d "$BaseDir" ] && mkdir -p "$BaseDir"
  [ ! -d "$BaseDir" ] && echo "Invalid $BaseDir" && exit 1 # exit for bad path

  ReadMeFile="$BaseDir/ReadMe_${BLD_NO}.txt"

  echo "$ReadMefile"
  local curr_dir=$(pwd)
  echo "Current Dir : $curr_dir"
  gen_readme_file $ReadMeFile
}

#: ${1?"Usage: $0 BaseDir BUILD_NUMBER"}
main "$@"
