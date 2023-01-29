#!/bin/sh

logger Factory Mode

busybox telnetd -p 23 -l /bin/sh &

ifconfig eth0 up
sleep 1

#This is nromally commented out. It starts the wifi. Lets have it back :)
/usr/sbin/net_manage.sh &

#This pisses around with things like unmounting the SD card
#daemon

#This actualy runs the camaera server. It talks to base. Lets leave it alone 
#/usr/sbin/anyka_ipc.sh start

/mnt/hack/busybox httpd -h /mnt/hack/www/

#But we owuld like to know the time please
source /etc/jffs2/time_zone.sh

while ! ping -c 1 -W 1 192.168.13.254; do
    echo "Waiting for 1.2.3.4 - network interface might be down..."
    sleep 1
done

logger setting time
ntpd -N -p uk.pool.ntp.org
