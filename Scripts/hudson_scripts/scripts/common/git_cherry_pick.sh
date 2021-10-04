#!/bin/bash

function cherry_pick_git()
{
  echo "**********************************"
  echo " Cherry Picking"
  echo "**********************************"
  local CL_file=$1
  local count=0
  local base_dir=$(pwd)
  local new_line=$'\n'
  local carr_ret=$'\r'
  FILENAME=$1
  while read LINE
  do
	#get the last character of the line
	operation=$LINE
	last_char="${LINE#${LINE%?}}"
	if [[ $last_char == $new_line || $last_char == $carr_ret ]]
	then
		#strip off the last character
		echo "stripping off the last char"
		operation=${LINE%%?}
	fi
	#Individual line in this file is [ProjectPath] [git cherry-pick command]
	if [ $count -eq 0 ]; then
		#First do cd to ProjectPath, skip if its root
		project_dir=$operation
		if [ "x$project_dir" != "x" ]
		then
			cd $project_dir
			#make sure its a valid path
			if [ $? -ne 0 ]; then
				echo "Invalid Cherry-pick File... Terminating Build"
				rm -f $CL_file
				exit 1
			fi
		fi
		let count=1
        else
		#Get the individual commands first and then issue them
		cherry_pick_cmd=${operation#*&&}
		fetch_cmd=${operation%&&*}
		#we don't need <username> in the fetch command, so strip if off ==> which is just before @ "<username>@"
		second_part=${fetch_cmd#*@}
		first_part="git fetch ssh://"
		fetch_cmd=$first_part$second_part
		echo "fetch: $fetch_cmd"
		$fetch_cmd
		$cherry_pick_cmd
		#next one is a project path, prepare for it
		let count=0
		cd $base_dir
	fi
  done < $FILENAME

  #Now that we have already consumed this file, delete it
  echo "rm $1"
  rm -f $CL_file
}

function main()
{
  local project_root=$1
  cd $project_root
  local CL_file=$2
  # Make sure there is a CL file to cherry pick
  if [ -f "$CL_file" ]
  then
	#Cherry-pick
	cherry_pick_git $CL_file
  fi
}

#: ${1?"Usage: $0 WORKSPACE CL_FILE_FOR_CHERRY_PICK"}
echo "Usage: $0 PROJECT_ROOT CL_FILE_FOR_CHERRY_PICK"
main "$@"
