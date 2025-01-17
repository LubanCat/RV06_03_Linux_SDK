#!/bin/sh

set -x

echo "start to do-test.sh"
# print rtt log
make_meta --rtt-log

dmesg &> /tmp/dmesg.log
f=$(ls /dev/mmcblk*p*)
if [ -z "$f" ];then
	f=$(ls /dev/mmcblk*)
fi
if [ -n "$f" ];then
	mount "$f" /mnt/sdcard/
	cnt=$(cat /mnt/sdcard/test_tb_num)
	cnt=$(( cnt + 1 ))
	echo $cnt > /mnt/sdcard/test_tb_num
	make_meta -d /dev/block/by-name/meta
	make_meta --update --meta_path /dev/block/by-name/meta --rk_fastae_max_frame "$(( cnt % 20 + 3 ))"
	echo
	echo "debug-test cnt [$cnt]"
	log_dir="/mnt/sdcard/log_$cnt"
	mkdir -p $log_dir
	make_meta --rtt-log &> $log_dir/rtt-fastae.log
	cp /tmp/venc-test.bin $log_dir/venc-test-$cnt.bin
	cp /tmp/venc0.bin $log_dir/venc0-$cnt.bin
	cp /tmp/venc1.bin $log_dir/venc1-$cnt.bin
	(cat /dev/mpi/vlog; cat /dev/mpi/valloc; cat /dev/mpi/vmcu; cat /dev/mpi/vrgn; cat /dev/mpi/vsys; echo ==========================================; cat /dev/mpi/vlog; cat /dev/mpi/valloc; cat /dev/mpi/vmcu; cat /dev/mpi/vrgn; cat /dev/mpi/vsys;) |tee > $log_dir/dev-mpi-$cnt.log
	cat /proc/vcodec/enc/venc_info &> $log_dir/proc-vcodec-enc-venc_info-$cnt-1
	cat /proc/vcodec/enc/venc_info &> $log_dir/proc-vcodec-enc-venc_info-$cnt-2
	cp -fa /tmp/dmesg.log $log_dir/dmesg-$cnt.log
	cp -fa /tmp/pts.txt $log_dir/
	if [ -f "/tmp/rkaiq0.log" ];then
		cp /tmp/rkaiq0.log $log_dir
	fi
	if [ -f "/tmp/rkaiq1.log" ];then
		cp /tmp/rkaiq1.log $log_dir
	fi
	sync
	umount /mnt/sdcard
fi

# clean rtt log
make_meta --rtt-log-clean

while true
do
	msg="`cat /proc/rkisp-vir0| grep Inter`"
	num=${msg#*Cnt:}
	num=${num%% *}

	if [ -f /proc/rkisp-vir1 ];then
		msg1="`cat /proc/rkisp-vir1| grep Inter`"
		num1=${msg1#*Cnt:}
		num1=${num1%% *}
		if [ "$num1" -lt 50 ];then
			dmesg
			echo "rkisp-vir1 error"
			break
		fi
	fi

	if [ "$num" -lt 50 ];then
		dmesg
		echo "rkisp-vir0 error"
		break
	else
		reboot
	fi
done
