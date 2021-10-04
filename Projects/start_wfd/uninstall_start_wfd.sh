#!/bin/bash

rm /system/bin/start_wfd
cp /system/lib/libwfdPESink.so-save /system/lib/libwfdPESink.so
chmod 777 /system/lib/libwfdPESink.so

