#!/bin/sh
#$1 - project direcory : anchovy-sdk, eureka, flex-sdk
#$2 - Hudson JOB_NAME
#$3 - Hudeson BUILD_NUMBER 
#$4 - Hudson Build URL 
#start the ftp session

FTP_SERVER=10.83.130.11
#FTP_SERVER=cloud.marvell.com
USERID=hudson
PASSWD=marvell88
SHARE_DIR=hudson

#INDEX_FILE=anchovy-sdk-index
INDEX_FILE=$2.index

#build the index file details
cd $1
NAND_MAGE=`ls *.img`
maillist=`cat maillist`
echo "$FTP_SERVER" > $INDEX_FILE
echo "$NAND_MAGE" >> $INDEX_FILE
echo "$SHARE_DIR" >> $INDEX_FILE
echo "$2" >> $INDEX_FILE
echo "$3" >> $INDEX_FILE
echo "$4" >> $INDEX_FILE
echo "$maillist" >> $INDEX_FILE

#copy all the build directory
ftp -n <<EOF 2>&1 >ftp.log
open $FTP_SERVER
user $USERID $PASSWD
prompt off
binary
cd $SHARE_DIR
mkdir $2
cd $2
mkdir $3
cd $3
mkdir logs
mput *
bye
EOF

#now copy the index file to trigger the auto program
ftp -n <<EOF 2>&1 >>ftp.log
open $FTP_SERVER
user $USERID $PASSWD
prompt off
binary
put $INDEX_FILE
bye
EOF


