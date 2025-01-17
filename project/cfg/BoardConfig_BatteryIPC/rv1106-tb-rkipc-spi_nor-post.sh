#!/bin/bash

PARTITION_MOUNT_SCRIPT=_S20linkmount

rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/auto_lane_obj_det.data
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librknnmrt_rockauto.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockauto.so

rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkAVS_genLut.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkAVS_genStitch.so

rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/sample_*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/simple_*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/rk_*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/Rk_*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/mp*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/vpu*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/rkisp*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/rkaiq*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/rga*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/dump*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/libsmartIr.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librve.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockit_tiny.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockit_full.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkAVS_stitchFor1106.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/libivs.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/*.md
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/services
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/protocols
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/lib/libstdc++.so.6.0.25-gdb.py
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/lib/libitm.so*
mv $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/S20linkmount $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/${PARTITION_MOUNT_SCRIPT}
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/S21appinit*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/rkipc
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/tb.sh
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/iqfiles/*.json
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/iqfiles/*.bin
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/hpmcu_wrap.bin

rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_cli_rtk
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_supplicant_nl80211_rtk
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_supplicant_rtk
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkdemuxer.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/j2s4b_dev
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/libdrm.so*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/modetest
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/lib/libintl.so*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/iperf
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/hostapd
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/dnsmasq

rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/insmod_ko.sh
# delete nouse ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/gcm.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/ext4.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/ccm.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/sha256_generic.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/libaes.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/libsha256.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/gf128mul.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/cmac.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/rve.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/libarc4.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/aes_generic.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/ctr.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/mac80211.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/atmb_iot_supplicant_demo
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/rk_vendor_storage.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/mtd_vendor_storage.ko

cat > $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcS <<EOF
#!/bin/sh
run_delay_task(){
	cd /oem/usr/ko
	test ! -f mtd_blkdevs.ko || insmod mtd_blkdevs.ko
	test ! -f mtdblock.ko || insmod mtdblock.ko
	test ! -f spi-nor.ko || insmod spi-nor.ko
	test ! -f dw_mmc.ko          || insmod dw_mmc.ko
	test ! -f dw_mmc-pltfm.ko    || insmod dw_mmc-pltfm.ko
	test ! -f dw_mmc-rockchip.ko || insmod dw_mmc-rockchip.ko

	sh /etc/init.d/${PARTITION_MOUNT_SCRIPT} start &

	test ! -f /oem/usr/ko/rknpu.ko || insmod /oem/usr/ko/rknpu.ko
	test ! -f snd-soc-simple-card-utils.ko || insmod snd-soc-simple-card-utils.ko
	test ! -f snd-soc-simple-card.ko || insmod snd-soc-simple-card.ko
	test ! -f snd-soc-rockchip-i2s-tdm.ko || insmod snd-soc-rockchip-i2s-tdm.ko
	test ! -f snd-soc-rv1106.ko || insmod snd-soc-rv1106.ko
	test ! -f /oem/usr/ko/insmod_wifi.sh || /oem/usr/ko/insmod_wifi.sh ${RK_ENABLE_FASTBOOT} ${RK_ENABLE_WIFI_CHIP}
	test ! -f mmc_block.ko       ||insmod mmc_block.ko
	test ! -f nls_cp936.ko       ||insmod nls_cp936.ko
	test ! -f fat.ko             ||insmod fat.ko
	test ! -f vfat.ko            ||insmod vfat.ko

	/etc/init.d/S80idb_bootconfig start
}

wait_mount_data()
{
	local timeout=0
	while true
	do
		if mount|grep -w "/userdata";then
			echo "found mount /userdata"
			break;
		else
			echo "wait mount /userdata"
			sleep .2
			timeout=\$(( timeout + 1 ))
			if [ \$timeout -gt 50 ];then
				echo "wait mount /userdata timeout"
				break
			fi
		fi
	done
}

# echo 3 > /sys/module/video_rkisp/parameters/debug
export LD_LIBRARY_PATH=\$PATH:/oem/usr/lib/
export PATH=\$PATH:/oem/usr/ko/
echo 256 > /proc/sys/kernel/threads-max
echo "start service" > /dev/kmsg
test -n "\$persist_camera_engine_log" && \
	/oem/usr/bin/fastboot_server &>/tmp/rkaiq0.log || \
	/oem/usr/bin/fastboot_server

/oem/usr/etc/init.d/S50nginx start &
/oem/usr/etc/init.d/S50fcgiwrap start &

run_delay_task
wait_mount_data
if [ -f /userdata/image.bmp ];then
    echo "image.bmp exist"
else
    test ! -f /oem/usr/share/image.bmp || cp /oem/usr/share/image.bmp /userdata/
fi

if [ -f /data/rkipc.ini ];then
    echo "rkipc.ini exist"
else
	cat /proc/cmdline | grep -w "rk_cam_w=2304" &&  cp -fa /oem/usr/share/rkipc-300w.ini /data/rkipc.ini
	cat /proc/cmdline | grep -w "rk_cam_w=2560" &&  cp -fa /oem/usr/share/rkipc-400w.ini /data/rkipc.ini
	cat /proc/cmdline | grep -w "rk_cam_w=2880" &&  cp -fa /oem/usr/share/rkipc-500w.ini /data/rkipc.ini
fi
sync
echo "start rkipc" > /dev/kmsg
test -n "\$persist_camera_engine_log" && \
	/oem/usr/bin/rkipc &>/tmp/rkaiq1.log &
test -z "\$persist_camera_engine_log" && \
	/oem/usr/bin/rkipc &

# Just for debug
(sleep 7 && test ! -f /data/do-test.sh || sh /data/do-test.sh) &
EOF
chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcS

cat > $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcK <<EOF
#!/bin/sh
echo "Start to killall task!!!"

for item in `echo rkipc fastboot_client fastboot_server`
do
	if ps |grep -v grep  |grep \$item ;then
		echo "killall -9 \$item"
		killall -9 \$item
	else
		break
	fi
	sleep .5
done

killall rkwifi_server
killall fcgiwrap
killall nginx
umount /data
echo "rcK done"
EOF
chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcK
