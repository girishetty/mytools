#!/bin/bash

BuildScript=${1?"Usage: BuildScript BaseDir MDY"}
BaseDir=${2?"Usage: BuildScript BaseDir MDY"}
MDY=${3?"Usage: BuildScript BaseDir MDY"}

echo '#!/bin/bash'> $BuildScript
echo >> $BuildScript
echo "MDY=\${1-'$MDY'}" >> $BuildScript
echo "BaseDir='$BaseDir'" >> $BuildScript
echo '
MyDir=`pwd`
TarBall="linux_sdk_${MDY}_bg2cd_dngle.tgz"
[ ! -f "$TarBall" ] && echo "No Tar Ball: $TarBall" && exit 1

mkdir Build 2>/dev/null
cd Build

echo "tar xf ../$TarBall"
tar xf ../$TarBall

Tag="1055_"`date "+%m%d%y"`
SrcNAND="./MRVL/Release_image/u2nandimg/uNAND_bg2cd_dngle.img"
if [ "x$Tag" == "x$MDY" ]; then
	DstNand="$BaseDir/uNAND_bg2cd_dngle_${MDY}.img"
else
	DstNand="$BaseDir/uNAND_bg2cd_dngle.img"
fi
PKG="Anchovy_BG2CD_A0.release"
cd $PKG

ToolChain="`pwd`/MRVL/build_scripts/toplevel_scripts/arm_unknown_toolchain/bin"
export PATH="$ToolChain:$PATH"
export PROFILE="Anchovy_BG2CD_A0"
BuildScript="./MRVL/build_scripts/build_pod"
[ ! -f "$BuildScript" ] && echo "Not exist: `pwd`/$BuildScript" && exit 1
$BuildScript -gbg2cd -ddngle
[ ! -f "$SrcNAND" ] && echo "Not exist: `pwd`/$SrcNAND" && exit 1

mkdir -p ${DstNand%/*} 2>/dev/null
echo "cp $SrcNAND $DstNand"
cp "$SrcNAND" "$DstNand"
[ ! -f "$DstNand" ] && echo "Not exist: $DstNand" && exit 1

cd "$MyDir"
rm -rf Build
' >> $BuildScript

chmod +x $BuildScript
echo "Generated $BuildScript"

