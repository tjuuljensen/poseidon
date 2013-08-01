#!/bin/bash
#

lfile=/home/pi/poseidon/log/10_most_mem_consuming_PID.txt
echo "" >> $lfile
echo "$(date "+%d-%m-%Y %T") : Memory check" >> $lfile
ps axo rss,%mem,comm,pid,euser | sort -nr | head -n 5 >> $lfile
ps axo rss,%mem,comm,pid,euser | grep poseidon >> $lfile
ps axo rss,%mem,comm,pid,euser | grep poseidon
