#!/bin/bash

local forever=0
JB_ROOT="JB_ROOT"
while [ $forever -eq 0 ]
do
	echo "Trying to see is there any updates and build the JB project.."
	cd /var/lib/hudson/jobs/Android-TOT/workspace
	/home/hudson-sc-debu/scripts/Android-TOT/poll_android_git.sh $JB_ROOT
	#Capture the poll status/return code
	status=$?
	if [ $status -eq 1 ]
	then
		#We have triggered a build now, so wait for atleast 60 mins (time to complete the build successfully)
		#before trying to poll for git update
		echo "Triggered a BUILD...Going to sleep for 60 mins"
		sleep 60m
	else
		#There was no git update since the last build. So no build was issued.
		#But, wait for 30 minutes before polling for git update again
		echo "No Git Update...So going to sleep for 30 mins before the next poll"
		sleep 30m
	fi
done
echo "We shouldn't be here..!! Restart the daemon again by calling <sudo sh ./daemon_poll_build.sh>"

