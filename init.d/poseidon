#! /bin/sh
### BEGIN INIT INFO
# Provides: poseidon
# Required-Start: $all
# Required-Stop: $all
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Description: Poseidon tank monitor startup script
### END INIT INFO

case "$1" in
'start')
/home/pi/poseidon/bin/poseidon
;;

'stop')
pkill poseidon
;;

'restart')
pkill -HUP poseidon
;;

esac

exit 
