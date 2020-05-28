#!/bin/bash 

function gen_update_git_sh()
{
  local mmddyy=`date "+%m%d%y_%H%M%S"`
  local out_file=$1
  local num_projs=0
  local cur_dir=`pwd`
  local mrvl_dir="Mrvl"
  local eureka_dir="Eureka"

  echo "Date Tag: $mmddyy" >$out_file

  # Get the details of Mrvl Project now
  echo "Mrvl Project:" >>$out_file
  cd $mrvl_dir
  local base_dir=$(pwd)
  projs=$( { repo forall -c "echo \$REPO_PATH"; } 2>&1 )
  if [ "x${projs/error */error}" == "xerror" ]; then echo "Wrong directory..!!"; exit 1; fi

  for proj in $projs; do
	cd $proj
	if [ $? -eq 0 ]; then 
		#It's a directory, now we can look for git log
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
  echo "" >>$out_file

  if [ $num_projs -gt 0 ]; then echo "Generated $out_file for Mrvl Project"; fi

  # Get the details of Eureka Project now
  echo "Eureka Project:" >>$out_file
  cd ..
  cd $eureka_dir
  base_dir=$(pwd)
  projs=$( { repo forall -c "echo \$REPO_PATH"; } 2>&1 )
  if [ "x${projs/error */error}" == "xerror" ]; then echo "Wrong directory..!!"; exit 1; fi
  num_projs=0

  for proj in $projs; do
	if [ "x${proj/skipping */skipping}" != "xskipping" ]; then 
		cd $proj
		if [ $? -eq 0 ]; then 
			#It's a directory, now we can look for git log
			MSG=$( { git log --oneline -n 1; } 2>&1 )
			if [ "x${MSG/fatal */fatal}" != "xfatal" ]; then 
				echo "" >>$out_file
				echo "$proj:" >>$out_file
				echo "$MSG" >>$out_file
				let num_projs+=1
			fi
		fi
		cd $base_dir
	fi
  done
  echo "" >>$out_file

  if [ $num_projs -gt 0 ]; then echo "Generated $out_file for Eureka Project"; fi

  cd ..
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
  gen_update_git_sh $ReadMeFile
}

#: ${1?"Usage: $0 BaseDir BUILD_NUMBER"}
main "$@"
