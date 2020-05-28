#!/bin/bash 

function gen_readme_file()
{
  local mmddyy=`date "+%m%d%y_%H%M%S"`
  local out_file=$1
  local num_projs=0

  echo "Date Tag: $mmddyy" >$out_file

  # Get the details of Android Project now
  echo "Project: Android JB LEO:" >>$out_file

  local base_dir=$(pwd)
  projs=$( { repo forall -c "echo \$REPO_PATH"; } 2>&1 )
  if [ "x${projs/error */error}" == "xerror" ]; then echo "Wrong directory..!!"; exit 1; fi

  #Now goto individual projects and update the project check-in details
  for proj in $projs; do
        cd $proj
        if [ $? -eq 0 ]; then
                #It's a directory, now we can look for git log
		#MSG=$( { git log --oneline --after=2013-04-01 -n 1; } 2>&1 )
                MSG=$( { git log --oneline -n 1; } 2>&1 )
                if [ "x${MSG/fatal */fatal}" != "xfatal" ]; then
                        echo "" >>$out_file
                        echo "$proj:" >>$out_file
                        echo "$MSG" >>$out_file
                        let num_projs+=1
                fi
        fi
        cd $base_dir
  done

  #Try to see is there any check-ins since last build
  #echo "" >>$out_file
  #local last_build_info_file=$jb_root"/../last_build_info.txt"
  #local last_build_date=$(head -1 $last_build_info_file)
  #local git_log_command="pwd && git log --oneline --after={$last_build_date}"
  #local git_log_command="pwd && git log --oneline -n 1"
  #{ repo forall -c "$git_log_command"; } 2>&1 >> $out_file

  echo "" >>$out_file
  if [ $num_projs -gt 0 ]; then echo "Generated $out_file for Android JB Project"; fi
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
