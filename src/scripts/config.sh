#!/bin/sh
logger Custom Firmware Mode

#start a telnet daemon
busybox telnetd -p 23 -l /bin/sh &

#bring up the network
ifconfig eth0 up
sleep 1

#This is normally commented out in the "official" firmware. But it starts the wifi... Lets have it back :)
/usr/sbin/net_manage.sh &

#start a lightweight http server that can do stuff like turn the lights on & off
/mnt/bin/busybox httpd -h /mnt/www/ -p 8080

#Sort out the timezone using the "offical" method
source /etc/jffs2/time_zone.sh

#Wait for network connectivity (check google dns servers)
while ! ping -c 1 -W 1 192.168.13.254; do
    echo "Waiting for router - network interface might be down..."
    sleep 1
done

#Set the time
logger setting time
ntpd -N -p 192.168.13.254
