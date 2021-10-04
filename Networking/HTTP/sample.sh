#!/bin/bash

declare -a fileList=("include/media/MediaCodecInfo.h"
                     "include/media/MediaCrypto.h"
                     "include/media/MediaFormat.h"
                     "media/libmedia/Android.mk"
                     "media/libmedia/MediaCodecAPI.cpp"
                     "media/libmedia/MediaCrypto.cpp"
                     "media/libmedia/MediaFormat.cpp"
                     "media/libmedia/tests_mediadrm/Android.mk"
                     "media/libmedia/tests_mediadrm/MediaDrmAPITest.cpp"
                     "media/libmedia/tests_mediadrm/TestVectors.cpp"
                     "media/libmedia/tests_mediadrm/TestVectors.h"
                     "media/libstagefright/rtsp/MyHandler.h"
                     "media/ndk/NdkMediaCodec.cpp")

backup()
{
    echo "Backing up to $1..."
    mkdir "$1/temp"
    ## now loop through the above array
    for i in "${fileList[@]}"
    do
       echo "$i"
       # or do whatever with individual element of the array
    done
}

restore()
{
    echo "Restoring back from $1..."
}

if [ "$1" = "-b" ]
then
    backup $2
elif [ "$1" = "-r" ]
then
    restore $2
else
    echo "Unknown option!!"
fi
