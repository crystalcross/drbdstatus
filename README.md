# drbdstatus

** DRBDSTATUS ** - is a quick utility I threw together for one purpose alone.   It allows you to get a formatted status of DRBD9 connections.

Previously *DRBD9* no longer suppored the /proc/drbd status.   This was a problem when trying to create any type of web based application safely which displayed the current status.   The /sys/kernel/debug/drbd files existed but are limited to root access.  This output is also not formatted for easy ingestion into a php engine.   

To solve this problem I created the "*drbdstatus*" utility.  This runs as a root (sticky bit), pulls the needed information and then returns it in a formatted output.

I have not yet added the "-h" help function yet.   Call me lazy, I've only worked on this so far for a few hours today.   But will make it a bit more robust as I go.   Also I plan to include some php files for pulling the information from the '*drbdstatus*' utility.

So far this has been tested with *DRBD9* in a multi-node environment under *ubuntu* 18.4.

