#!/bin/bash

# file permissions as in the file
# vi Eureka/system/core/include/private/android_filesystem_config.h


function update_file_permissions()
{
#mount -o,remount rw /system

#[ ! -f "$BuildMrvlSDK" ] && echo "No build script" && exit 1
echo "Check if $1 exists?"
if [ -f $1 ]; then 
	echo "$1 exists !!!"
	cp $1 $1-org

	echo "update_file_permissions $1"

	echo "############ Marvell Build Script Modified #########" >> $1
	echo "" >> $1

#	echo "mount system system /system rw remount" >> $1
	echo "mount -o,remount rw /system" >> $1
	echo "" >> $1
	echo "" >> $1
	echo "chmod 00755 /system/bin/*" >> $1
	echo "chown root.root /system/bin/*" >> $1
	echo "chmod 00600 /system/boot/*" >> $1
	echo "chown root.root /system/boot/*" >> $1
	echo "chmod 00600 /system/chrome/*" >> $1
	echo "chown chrome.chrome /system/chrome/*" >> $1
	echo "chmod 00700 /system/etc/dhcpcd/*" >> $1
	echo "chown dhcp.dhcp /system/etc/dhcpcd/*" >> $1
	echo "chmod 00755 /system/chrome/osd_images" >> $1
	echo "chmod 00500 /system/chrome/osd_images/*" >> $1
	echo "chown chrome.chrome /system/chrome/osd_images/*" >> $1
	echo "chmod 00500 /system/chrome/setup/http/jquery.min.js" >> $1
	echo "chown chrome.chrome /system/chrome/setup/http/jquery.min.js" >> $1
	echo "chmod 00500 /system/chrome/setup/http/setup.html" >> $1
	echo "chown chrome.chrome /system/chrome/setup/http/setup.html" >> $1
	echo "" >> $1
	echo "chmod 00600 /system/build.prop" >> $1
	echo "chown root.root /system/build.prop" >> $1
	echo "chmod 00700 /system/chrome/content_shell" >> $1
	echo "chown chrome.chrome /system/chrome/content_shell" >> $1
	echo "chmod 00700 /system/chrome/eureka_shell" >> $1
	echo "chown chrome.chrome /system/chrome/eureka_shell" >> $1
	echo "chmod 00700 /system/chrome/update_engine" >> $1
	echo "chown updater.updater /system/chrome/update_engine" >> $1
	echo "chmod 04755 /system/chrome/chrome_sandbox" >> $1
	echo "chown root.chrome /system/chrome/chrome_sandbox" >> $1
	echo "chmod 04755 /system/chrome/chrome-sandbox" >> $1
	echo "chown root.chrome /system/chrome/chrome-sandbox" >> $1
	echo "chmod 04750 /system/bin/dumpstate" >> $1
	echo "chown root.chrome /system/bin/dumpstate" >> $1
	echo "" >> $1
	echo "" >> $1
	echo "chmod 00755  /system/home/galois/*" >> $1
	echo "chown root.root  /system/home/galois/*" >> $1
	echo "" >> $1
	echo "chmod 00700 /system/netflix/bin/*" >> $1
	echo "chown netflix.netflix /system/netflix/bin/*" >> $1
	echo "chmod 04750 /system/netflix/bin/netflix_init" >> $1
	echo "chown netflix.netflix /system/netflix/bin/netflix_init" >> $1
	echo "chmod 04750 /system/netflix/bin/netflix_sandbox" >> $1
	echo "chown netflix.netflix /system/netflix/bin/netflix_sandbox" >> $1
	echo "chmod 00750 /system/netflix/*" >> $1
	echo "chown chrome.chrome /system/netflix/*" >> $1
	echo "chmod 00750 /system/netflix" >> $1
	echo "chown chrome.chrome /system/netflix" >> $1
	echo "" >> $1
	echo "chmod 00755  /system/usr/bin/*" >> $1
	echo "chown root.root /system/usr/bin/*" >> $1
	echo "chmod 00755  /system/xbin/*" >> $1
	echo "chown root.root /system/xbin/*" >> $1
	echo "" >> $1
	echo "chmod 04755 /system/chrome/chrome_sandbox" >> $1
	echo "chmod 04755 /system/chrome/chrome-sandbox" >> $1
	echo "chmod 00755 /system/chrome/wifi_check" >> $1

	echo "mount -o,remount ro /system" >> $1
#	echo "mount system system / ro remount" >> $1

	echo "############ Marvell Build Script Modified #########" >> $1
	echo "############ copy client keys #########" >> $1
	echo "cp /data/client.nd.bin /cache/client1.bin" >> $1
fi
}

function copy_client_files()
{
  echo "############ copy client files to $1 $2 #########"
  cp $1/client.crt $2
  cp $1/client.key.nd.bin $2
  cp $1/client.key.nd.bin $2/client.key.bin
}

function main()
{
  SCRIPTS_DIR=$1
#  update_file_permissions out/target/product/anchovy/root/sbin/update_bootid.sh
  update_file_permissions out/target/product/anchovy/root/sbin/update_bootid_and_urandom.sh
  copy_client_files $SCRIPTS_DIR out/target/product/anchovy/root/data
}

main "$@"

