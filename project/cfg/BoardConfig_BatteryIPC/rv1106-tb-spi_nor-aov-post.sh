#!/bin/bash

PARTITION_MOUNT_SCRIPT=_S20linkmount
cd $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/
shopt -s extglob
if [ -n "$RK_AOV_DEFAULT_APP" ]; then
rm !($RK_AOV_DEFAULT_APP|fastboot_server|fastboot_client|rk_mpi_uvc|rk_mpi_venc_test|usb_config.sh|resume*)
else
rm !(sample_aov_*|fastboot_server|fastboot_client|rk_mpi_uvc|rk_mpi_venc_test|usb_config.sh|resume*)
fi
shopt -u extglob
cd -
mv $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkaiq.so $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/lib
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkrawstream.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkAVS_genLut.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkyuvscaler.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockit_full.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librknnmrt_rockauto.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockit_tiny.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockauto.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkAVS_genStitch.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/share/meta*.img
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/share/rtthread_*.bin
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/auto_lane_obj_det.data

if [ "${RK_AOV_APP_BUILD_STATIC}x" = "yx" ];then
	rm -vf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockit*
	rm -vf $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/lib/librkaiq*
	rm -vf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockiva*
	rm -vf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librknnmrt*
	rm -vf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librockchip_mpp*
	rm -vf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librga*
	rm -vf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/libaec_bf_process.so
	rm -vf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkaudio*
fi

rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/*.md
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/flash*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/services
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/protocols
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/lib/libstdc++.so.6.0.25-gdb.py
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/lib/libitm.so*
mv $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/S20linkmount $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/${PARTITION_MOUNT_SCRIPT}
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/S21appinit*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/rkipc
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/iqfiles/*.json
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/librkdemuxer.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/libjpeg*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/libpng16*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/libiconv.so*
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/iqfiles/*.bin
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/hpmcu_wrap.bin

rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_cli
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_cli_rtk
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_supplicant
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_supplicant_nl80211_rtk
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/wpa_supplicant_rtk
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/wpa_supplicant.conf
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/libwpa_client.so
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin/modetest
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/lib/libwpa_client.so
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
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/libarc4.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/aes_generic.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/ctr.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/mac80211.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/atmb_iot_supplicant_demo
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/rk_vendor_storage.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/mtd_vendor_storage.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/jbd2.ko
rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/ko/mbcache.ko

cat > $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcS <<EOF
#!/bin/sh
export PATH=\$PATH:/oem/usr/ko/:/oem/usr/bin
export LD_LIBRARY_PATH=/oem/usr/lib:/oem/lib
echo 256 > /proc/sys/kernel/threads-max

set -x

if [ -n "\$tb_aov_loop" ];then
	tb_test_opt="--aov_loop_count \$tb_aov_loop"
fi

if [ -z "$RK_AOV_DEFAULT_APP" ];then
	if [ -n "\$tb_aov_venc" ];then
		if [ -n "\$persist_camera_engine_log" ];then
			sample_aov_vi_venc --wrap_lines 0 -r 1 -w \$rk_cam_w -h \$rk_cam_h \$tb_test_opt &>/tmp/rkaiq0.log &
		else
			sample_aov_vi_venc --wrap_lines 0 -r 1 -w \$rk_cam_w -h \$rk_cam_h \$tb_test_opt &
		fi
	fi
fi

test ! -f /oem/usr/ko/rknpu.ko || insmod /oem/usr/ko/rknpu.ko

test -n "\$tb_aov_vi"  || \
	test -n "\$tb_aov_venc" || \
	test -n "\$tb_aov_aiisp" || \
	test -n "\$NoAuto" || \
	test -n "$RK_AOV_DEFAULT_APP" || \
	sample_aov_vi_iva_venc --wrap_lines 0 -r 1 -w \$rk_cam_w -h \$rk_cam_h \$tb_test_opt &

test -z "$RK_AOV_DEFAULT_APP" || \
	test -n "\$NoAuto" || \
	$RK_AOV_DEFAULT_APP --wrap_lines 0 -r 1 -w \$rk_cam_w -h \$rk_cam_h \$tb_test_opt &

# NOTICE: AI-ISP NOT SUPPORT WRAP
test ! -f /oem/usr/ko/rve.ko || insmod /oem/usr/ko/rve.ko
test -n "$RK_AOV_DEFAULT_APP" || \
	test -z "\$tb_aov_aiisp"  || \
	sample_aov_aiisp_iva_venc -w \$rk_cam_w -h \$rk_cam_h \$tb_test_opt &

function run_delay_task() {
	cd /oem/usr/ko
	test ! -f mtd_blkdevs.ko || insmod mtd_blkdevs.ko
	test ! -f mtdblock.ko || insmod mtdblock.ko
	test ! -f spi-nor.ko || insmod spi-nor.ko
	test ! -f dw_mmc.ko          || insmod dw_mmc.ko
	test ! -f dw_mmc-pltfm.ko    || insmod dw_mmc-pltfm.ko
	test ! -f dw_mmc-rockchip.ko || insmod dw_mmc-rockchip.ko
	sh /etc/init.d/${PARTITION_MOUNT_SCRIPT} start &

	# TODO
	# test ! -f snd-soc-simple-card-utils.ko || insmod snd-soc-simple-card-utils.ko
	# test ! -f snd-soc-simple-card.ko || insmod snd-soc-simple-card.ko
	# test ! -f snd-soc-rockchip-i2s-tdm.ko || insmod snd-soc-rockchip-i2s-tdm.ko
	# test ! -f snd-soc-rv1106.ko || insmod snd-soc-rv1106.ko

	test ! -f mmc_block.ko || insmod mmc_block.ko
	test ! -f nls_cp936.ko || insmod nls_cp936.ko
	test ! -f fat.ko       || insmod fat.ko
	test ! -f vfat.ko      || insmod vfat.ko

	/etc/init.d/S80idb_bootconfig start
	# test ! -f /oem/usr/ko/insmod_wifi.sh  || /oem/usr/ko/insmod_wifi.sh ${RK_ENABLE_FASTBOOT} ${RK_ENABLE_WIFI_CHIP}

	# start to detect sdcard
	sdcar_dev=\$(ls /dev/mmcblk*p*)
	if [ -z "\$sdcar_dev" ];then
		sdcar_dev=\$(ls /dev/mmcblk*)
	fi
	if [ -n "\$sdcar_dev" ];then
		mount "\$sdcar_dev" /mnt/sdcard/
		is_sdcard_mount="-o /mnt/sdcard"
	fi

	echo "ffaa0000.mmc" > /sys/bus/platform/drivers/dwmmc_rockchip/unbind

	test -n "$RK_AOV_DEFAULT_APP" || \
		test -z "\$tb_aov_vi"  || \
		sample_aov_vi -w \$rk_cam_w -h \$rk_cam_h \$is_sdcard_mount &

	# Just for test
	(sleep 5 && test ! -e "/userdata/auto_userdata_test" || sh /userdata/auto_test.sh &) &
}

(sleep 3 && run_delay_task) &

# Just for test
(sleep 5 && test ! -f /data/do-test.sh || sh /data/do-test.sh) &
EOF
chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcS

cat > $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcK <<EOF
#!/bin/sh
echo "Start to killall task!!!"

for item in `echo sample_aov_aiisp_iva_venc sample_aov_vi_iva_venc sample_aov_vi_venc sample_aov_vi`
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
umount /data
echo "rcK done"
EOF
chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcK
