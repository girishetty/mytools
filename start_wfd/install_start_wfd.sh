#!/bin/bash

cd /data/start_wfd
cp start_wfd /system/bin/
chmod 777 /system/bin/start_wfd
cp /system/lib/libwfdPESink.so /system/lib/libwfdPESink.so-save
cp libwfdPESink.so /system/bin/libwfdPESink.so
chmod 777 /system/lib/libwfdPESink.so

