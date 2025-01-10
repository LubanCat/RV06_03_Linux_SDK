#!/bin/bash -e
### BEGIN INIT INFO
# Provides:          rockchip
# Required-Start:
# Required-Stop:
# Default-Start:
# Default-Stop:
# Short-Description:
# Description:       Setup rockchip platform environment
### END INIT INFO

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

# first boot configure
if [ ! -e "/usr/local/first_boot_flag" ] ;
then
    echo "It's the first time booting."
    echo "The rootfs will be configured."

    Mem_Size=$(free -m | grep Mem | awk '{print $2}')
    if [ '256' -gt $Mem_Size  ] ;
    then
        echo 'Mem_Size =' $Mem_Size 'MB , make swap memory '
        swapoff -a
        dd if=/dev/zero of=/var/swapfile bs=1M count=128
        chmod 0600 /var/swapfile
        mkswap /var/swapfile
        swapon /var/swapfile
        echo "/var/swapfile swap swap defaults 0 0" >> /etc/fstab
    fi

    for dir in /*; do
        case "$dir" in
            /proc|/dev|/sys|/run) continue ;;  # 排除的目录
            *) chown 0:0 -R "$dir" ;;      # 处理其他目录
        esac
    done
    chown 1000:1000 -R /home/cat

    # Force rootfs synced
    mount -o remount,sync /

    if [ -e /usr/bin/gst-launch-1.0 ]; then
        setcap CAP_SYS_ADMIN+ep /usr/bin/gst-launch-1.0
    fi

    if [ -e "/dev/rfkill" ] ; then
       rm /dev/rfkill
    fi

    # The base target does not come with lightdm/rkaiq_3A
    if [ -e /etc/gdm3/daemon.conf ]; then
        systemctl restart gdm3.service || true
    elif [ -e /etc/lightdm/lightdm.conf ]; then
        systemctl restart lightdm.service || true
    fi

    if [ -e /usr/lib/systemd/system/rkisp_3A.service ]; then
        systemctl restart rkisp_3A.service || true
    elif [ -e /usr/lib/systemd/system/rkaiq_3A.service ]; then
        systemctl restart rkaiq_3A.service || true
    fi

    touch /usr/local/first_boot_flag
fi

# Create dummy video node for chromium V4L2 VDA/VEA with rkmpp plugin
echo dec > /dev/video-dec0
echo enc > /dev/video-enc0
chmod 660 /dev/video-*
chown root:video /dev/video-*

# The chromium using fixed pathes for libv4l2.so
ln -rsf /usr/lib/*/libv4l2.so /usr/lib/
[ -e /usr/lib/aarch64-linux-gnu/ ] && ln -Tsf lib /usr/lib64

# sync system time
# hwclock --systohc
