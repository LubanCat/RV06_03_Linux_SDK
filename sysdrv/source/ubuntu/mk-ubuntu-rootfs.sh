#!/bin/bash -e

# Directory contains the target rootfs
TARGET_ROOTFS_DIR="binary"
# Ubuntu release version
UBUNTU_RELEASE="jammy" # focal, jammy, noble
# Rootfs arch 
ARCH=arm

if [ "$UBUNTU_RELEASE" == "focal" ]; then
	UBUNTU_VERSION='20.04.5'
elif [ "$UBUNTU_RELEASE" == "jammy" ]; then
	UBUNTU_VERSION='22.04.5'
elif [ "$UBUNTU_RELEASE" == "noble" ]; then
	UBUNTU_VERSION='24.04.1'
fi
echo -e "\033[47;36m set UBUNTU_VERSION=$UBUNTU_VERSION ...... \033[0m"


case "${ARCH:-$1}" in
    arm|arm32|armhf)
        ARCH=armhf
        ;;
    *)
        ARCH=arm64
        ;;
esac
echo -e "\033[47;36m Building for $ARCH \033[0m"


if [ ! -e ubuntu-$UBUNTU_RELEASE-base-$ARCH-*.tar.gz ]; then
    echo "\033[41;36m Run mk-base-ubuntu.sh first \033[0m"
    ./mk-base-ubuntu.sh
fi

finish() {
    sudo umount $TARGET_ROOTFS_DIR/dev
    exit -1
}
trap finish ERR

echo -e "\033[47;36m Extract image \033[0m"
sudo rm -rf $TARGET_ROOTFS_DIR
sudo mkdir -p $TARGET_ROOTFS_DIR
sudo tar -xpf ubuntu-$UBUNTU_RELEASE-base-$ARCH-*.tar.gz -C $TARGET_ROOTFS_DIR

# overlay folder
sudo cp -rpf overlay/* $TARGET_ROOTFS_DIR/

# overlay-firmware folder
sudo cp -rpf overlay-firmware/* $TARGET_ROOTFS_DIR/

## hack the serial
sudo cp -f overlay/usr/lib/systemd/system/serial-getty@.service $TARGET_ROOTFS_DIR/lib/systemd/system/serial-getty@.service

echo -e "\033[47;36m Change root.....................\033[0m"
if [ "$ARCH" == "armhf" ]; then
    sudo cp /usr/bin/qemu-arm-static $TARGET_ROOTFS_DIR/usr/bin/
elif [ "$ARCH" == "arm64"  ]; then
    sudo cp /usr/bin/qemu-aarch64-static $TARGET_ROOTFS_DIR/usr/bin/
fi

./ch-mount.sh -m $TARGET_ROOTFS_DIR

ID=$(stat --format %u $TARGET_ROOTFS_DIR)

cat << EOF | sudo chroot $TARGET_ROOTFS_DIR

# Fixup owners
if [ "$ID" -ne 0 ]; then
    find / -user $ID -exec chown -h 0:0 {} \;
fi
for u in \$(ls /home/); do
    chown -h -R \$u:\$u /home/\$u
done

export LC_ALL=C.UTF-8

apt-get update
apt-get upgrade -y

chmod o+x /usr/lib/dbus-1.0/dbus-daemon-launch-helper
chmod +x /etc/rc.local

export DEBIAN_FRONTEND=noninteractive
export APT_INSTALL="apt-get install -fy --allow-downgrades"

echo -e "\033[47;36m ---------- LubanCat -------- \033[0m"
\${APT_INSTALL} u-boot-tools logrotate bluez bluez-tools v4l-utils

echo -e "\033[47;36m ------- Custom Script ------- \033[0m"
systemctl mask systemd-networkd-wait-online.service
systemctl mask NetworkManager-wait-online.service
systemctl disable hostapd
rm /lib/systemd/system/wpa_supplicant@.service

echo -e "\033[47;36m  ---------- Clean ----------- \033[0m"
if [ -e "/usr/lib/arm-linux-gnueabihf/dri" ] ;
then
        cd /usr/lib/arm-linux-gnueabihf/dri/
        cp kms_swrast_dri.so swrast_dri.so rockchip_dri.so /
        rm /usr/lib/arm-linux-gnueabihf/dri/*.so
        mv /*.so /usr/lib/arm-linux-gnueabihf/dri/
elif [ -e "/usr/lib/aarch64-linux-gnu/dri" ];
then
        cd /usr/lib/aarch64-linux-gnu/dri/
        cp kms_swrast_dri.so swrast_dri.so rockchip_dri.so /
        rm /usr/lib/aarch64-linux-gnu/dri/*.so
        mv /*.so /usr/lib/aarch64-linux-gnu/dri/
        rm /etc/profile.d/qt.sh
fi

rm -rf /home/$(whoami)
rm -rf /var/lib/apt/lists/*
rm -rf /var/cache/
rm -rf /boot/*

EOF

./ch-mount.sh -u $TARGET_ROOTFS_DIR

cd $TARGET_ROOTFS_DIR
sudo tar zcf ../ubuntu-$UBUNTU_RELEASE-rootfs-$ARCH.tar.gz ./*

# source ./mk-image.sh 
