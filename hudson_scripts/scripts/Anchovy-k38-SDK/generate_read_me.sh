#!/bin/bash 

function main()
{
  BaseDir=${1?"Usage: $0 BaseDir MDY MRVL"}
  MDY=${2-"git"}
  get_mrvl_dir $3

  ReadMeFile="ReadMe_${MDY}.txt"

  # make full path of script name
  [ "x${ReadMeFile##/*}" != "x" ] && ReadMeFile="$BaseDir/$ReadMeFile"

  #echo "MRVL_DIR is $MRVL_DIR"
  #echo "ReadMeFile is $ReadMeFile"

  mkdir -p "${ReadMeFile%/*}" 2>/dev/null
  gen_update_git_sh $MRVL_DIR $MDY $ReadMeFile
}

function get_mrvl_dir()
{
  local chk_dir="build_scripts"
  local mrvl_dir
  #local mrvl_dir=`pwd`"/MRVL"

  if [ "x$1" != "x" ]; then
	mrvl_dir=`cd $1; pwd`
	if [ -d $mrvl_dir/$chk_dir ]; then
		MRVL_DIR=$mrvl_dir
		return;
	fi
  fi

  #echo "try immediate child first"
  local try_path=`ls */$chk_dir -d`
  for p in $try_path; do
	p=`pwd`"/$p"
	mrvl_dir=${p%/$chk_dir*}
	if [ -d $mrvl_dir/$chk_dir ]; then
		MRVL_DIR=$mrvl_dir
		return;
	fi
  done

  #echo "traverse current and parent folders"
  try_path=`pwd`
  mrvl_dir=${try_path%/$chk_dir*}
  while [ ! -d $mrvl_dir/$chk_dir ]; do
	try_path=${mrvl_dir%/*}
	mrvl_dir=${try_path%/$chk_dir*}
	#echo "next try: $mrvl_dir"
	if [ "x$mrvl_dir" == "x" ]; then
		echo "Not found: MRVL folder";
		exit 1;
	fi
  done

  #echo "mrvl_dir is $mrvl_dir"
  MRVL_DIR=$mrvl_dir
}

function gen_update_git_sh()
{
  local mrvl_dir=$1
  local mmddyy=$2
  local out_file=$3
  local num_projs=0
  local cur_dir=`pwd`

  #[ "x$out_file" == "x" ] && out_file=$ReadMeFile
  #[ "x$mrvl_dir" == "x" ] && mrvl_dir=$MRVL_DIR

  cd $mrvl_dir
  projs=$( { repo forall -c "echo \$REPO_PROJECT"; } 2>&1 )

  echo "Date Tag: $mmddyy" >$out_file

  for proj in $projs; do
	#echo $proj
	cd "$mrvl_dir/$proj"
	#MSG=$( { git checkout remotes/sc-debu-git/dev_branch/1002; } 2>&1 )
	#if [ "x${MSG/*HEAD */HEAD}" == "xHEAD" ]; then 
	MSG=$( { git log --oneline -n 1; } 2>&1 )
	if [ "x${MSG/fatal */fatal}" != "xfatal" ]; then 
		echo "" >>$out_file
		echo "$proj:" >>$out_file
		echo "$MSG" >>$out_file
		let num_projs+=1
	fi
	cd ..
  done
  echo "" >>$out_file

  if [ $num_projs -gt 0 ]; then echo "Generated $out_file"; fi
  cd $cur_dir
}

#: ${1?"Usage: $0 MRVL_DIR RESULT_SCRIPT"}
main "$@"
