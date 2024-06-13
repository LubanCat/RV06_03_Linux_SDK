// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <getopt.h>
#include <termios.h>

#include "audio.h"
#include "common.h"
#include "isp.h"
#include "log.h"
#include "network.h"
#include "param.h"
#include "rockiva.h"
#include "server.h"
#include "storage.h"
#include "system.h"
#include "video.h"
#include <linux/input.h>
#include "device.h"
#include "rk_pwm.h"
#include "ptzcmd.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "rkipc.c"
pthread_mutex_t lock;
pthread_cond_t cond;

char *server_id = "41010500002000000001";
char *server_ip = "192.168.103.37";
int server_port = 5060;
int device_port = 5064;
char *device_id_head = "3402000000200000";
char device_id[21];
char username[21];
char *password = "12345678";
char *manufacture = "lbc-4G-ipc";

// char *server_id = "34020000002000000001";
// char *server_ip = "192.168.103.8";
// int server_port = 6668;
// int device_port = 5064;
// char *device_id_head = "3402000000200000";
// char device_id[21];
// char username[21];
// char *password = "12345678";
// char *manufacture = "lbc-4G-ipc";


// char *server_id = "41010500002000000001";
// char *server_ip = "47.108.227.19";
// int server_port = 8116;
// int device_port = 5064;
// char *device_id_head = "4101050000200000";
// char device_id[21];
// char username[21];
// char *password = "newings2024";
// char *manufacture = "lbc-4G-ipc";

enum { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };

int enable_minilog = 0;
int rkipc_log_level = LOG_INFO;

static int g_main_run_ = 1;
char *rkipc_ini_path_ = NULL;
char *rkipc_iq_file_path_ = NULL;

static void sig_proc(int signo) {
	LOG_INFO("received signo %d \n", signo);
	g_main_run_ = 0;
}

static const char short_options[] = "c:a:l:";
static const struct option long_options[] = {{"config", required_argument, NULL, 'c'},
                                             {"aiq_file", no_argument, NULL, 'a'},
                                             {"log_level", no_argument, NULL, 'l'},
                                             {"help", no_argument, NULL, 'h'},
                                             {0, 0, 0, 0}};

static void usage_tip(FILE *fp, int argc, char **argv) {
	fprintf(fp,
	        "Usage: %s [options]\n"
	        "Version %s\n"
	        "Options:\n"
	        "-c | --config      rkipc ini file, default is "
	        "/userdata/rkipc.ini, need to be writable\n"
	        "-a | --aiq_file    aiq file dir path, default is /etc/iqfiles\n"
	        "-l | --log_level   log_level [0/1/2/3], default is 2\n"
	        "-h | --help        for help \n\n"
	        "\n",
	        argv[0], "V1.0");
}

void rkipc_get_opt(int argc, char *argv[]) {
	for (;;) {
		int idx;
		int c;
		c = getopt_long(argc, argv, short_options, long_options, &idx);
		if (-1 == c)
			break;
		switch (c) {
		case 0: /* getopt_long() flag */
			break;
		case 'c':
			rkipc_ini_path_ = optarg;
			break;
		case 'a':
			rkipc_iq_file_path_ = optarg;
			break;
		case 'l':
			rkipc_log_level = atoi(optarg);
			break;
		case 'h':
			usage_tip(stdout, argc, argv);
			exit(EXIT_SUCCESS);
		default:
			usage_tip(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}
}

#define SHORT_PRESS_THRESHOLD 3  // 短按阈值，单位为秒
#define AO_FREAD_SIZE 1024 * 4
static void *wait_key_event(void *arg) {
	int key_fd;
	key_fd = open("/dev/input/event0", O_RDONLY);
	if (key_fd < 0) {
		LOG_ERROR("can't open /dev/input/event0\n");
		return NULL;
	}
	fd_set rfds;
	int nfds = key_fd + 1;
	struct timeval timeout;
	struct input_event key_event;
	struct timespec press_time;
	int key_volumeup_pressed = 0;

	while (g_main_run_) {
		// The rfds collection must be emptied every time,
		// otherwise the descriptor changes cannot be detected
		timeout.tv_sec = 1;
		FD_ZERO(&rfds);
		FD_SET(key_fd, &rfds);
		select(nfds, &rfds, NULL, NULL, &timeout);
		// wait for the key event to occur
		if (FD_ISSET(key_fd, &rfds)) {
			read(key_fd, &key_event, sizeof(key_event));
			LOG_DEBUG("[timeval:sec:%d,usec:%d,type:%d,code:%d,value:%d]\n", key_event.time.tv_sec,
			         key_event.time.tv_usec, key_event.type, key_event.code, key_event.value);
			if ((key_event.code == KEY_VOLUMEDOWN) && key_event.value) {
				LOG_INFO("get KEY_VOLUMEDOWN\n");
				rkipc_ao_init();
				FILE *fp = fopen("/oem/usr/share/speaker_test.wav", "rb");
				int size = AO_FREAD_SIZE;
				char *tmp_data;
				tmp_data = malloc(AO_FREAD_SIZE);
				while (size > 0) {
					memset((void *)tmp_data, 0, AO_FREAD_SIZE);
					size = fread(tmp_data, 1, AO_FREAD_SIZE, fp);
					rkipc_ao_write(tmp_data, AO_FREAD_SIZE);
				}
				rkipc_ao_write(tmp_data, 0);
				free(tmp_data);
				fclose(fp);
				rkipc_ao_deinit();
			}

			if (key_event.code == KEY_VOLUMEUP) {
				if (key_event.value == 1) {  // 按键按下
					clock_gettime(CLOCK_MONOTONIC, &press_time);
					key_volumeup_pressed = 1;
				} else if (key_event.value == 0 && key_volumeup_pressed) {  // 按键松开
					struct timespec release_time;
					clock_gettime(CLOCK_MONOTONIC, &release_time);
					double press_duration = (release_time.tv_sec - press_time.tv_sec) +
											(release_time.tv_nsec - press_time.tv_nsec) / 1e9;

					if (press_duration < SHORT_PRESS_THRESHOLD) {
						LOG_INFO("get KEY_VOLUMEUP: short press, system reboot\n");
						rk_system_reboot();
					} else {
						LOG_INFO("get KEY_VOLUMEUP: long press, system factory reset\n");
						rk_system_factory_reset();
					}
					key_volumeup_pressed = 0;
				}
			}
		}
	}

	if (key_fd) {
		close(key_fd);
		key_fd = 0;
	}
	LOG_DEBUG("wait key event out\n");
	return NULL;
}


// gpio key事件处理线程
static void *wait_gpio_event(void *arg) {
    int key_fd = open("/dev/input/event1", O_RDONLY);
    if (key_fd < 0) {
        LOG_ERROR("Can't open /dev/input/event1: %s\n", strerror(errno));
        return NULL;
    }

    fd_set rfds;
    struct timeval timeout;
    struct input_event key_event;

    while (g_main_run_) {
        // 初始化文件描述符集合和超时时间
        FD_ZERO(&rfds);
        FD_SET(key_fd, &rfds);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int retval = select(key_fd + 1, &rfds, NULL, NULL, &timeout);
        if (retval == -1) {
            LOG_ERROR("select() failed: %s\n", strerror(errno));
            break;
        } else if (retval) {
            if (FD_ISSET(key_fd, &rfds)) {
                ssize_t bytes_read = read(key_fd, &key_event, sizeof(key_event));
                if (bytes_read == -1) {
                    LOG_ERROR("read() failed: %s\n", strerror(errno));
                    break;
                }
                if (bytes_read == sizeof(key_event)) {
                    LOG_DEBUG("[timeval:sec:%ld,usec:%ld,type:%d,code:%d,value:%d]\n",
                             key_event.time.tv_sec, key_event.time.tv_usec,
                             key_event.type, key_event.code, key_event.value);

                    if (key_event.code == KEY_A && key_event.value) {
                        LOG_INFO("Received KEY_A: PIR sensor output high\n");
                    }
                } else {
                    LOG_ERROR("Incomplete read: expected %zu bytes, got %zd bytes\n", sizeof(key_event), bytes_read);
                }
            }
        }
    }

    close(key_fd);
    LOG_DEBUG("wait key event thread exiting\n");
    return NULL;
}

#define DAY_TO_NIGHT_SENSITIVITY 900
static void *day_night_event(void *arg) {
    int ldr_fd, light_state = 1, ready = 0;
    char buffer[16];
    ssize_t bytes_read;
    char value;

    while (g_main_run_) {
        // 打开设备文件
        ldr_fd = open("/sys/bus/iio/devices/iio:device0/in_voltage1_raw", O_RDONLY);
        if (ldr_fd < 0) {
            LOG_ERROR("Failed to open device file: /sys/bus/iio/devices/iio:device0/in_voltage1_raw\n");
            return NULL;
        }

        // 清空缓冲区
        memset(buffer, 0, sizeof(buffer));

        // 读取设备文件内容
        bytes_read = read(ldr_fd, buffer, sizeof(buffer) - 1);
        close(ldr_fd); // 在这里关闭文件描述符
        ldr_fd = 0;

        if (bytes_read < 0) {
            LOG_ERROR("Failed to read device file: /sys/bus/iio/devices/iio:device0/in_voltage1_raw\n");
            return NULL;
        }

        // 确保缓冲区以 NULL 结尾
        buffer[bytes_read] = '\0';

        // 将读取的值转换为整数
        int light_value = atoi(buffer);
        LOG_DEBUG("Converted light sensor value: %d\n", light_value);

        if (light_value <= 512) {
            LOG_ERROR("light sensor value error\n");
        } else {
            if (light_state == 1 && light_value >= DAY_TO_NIGHT_SENSITIVITY) {
                LOG_INFO("light sensor value transitioning to night\n");
                light_state = 0;
            } else if (light_state == 0 && light_value <= (DAY_TO_NIGHT_SENSITIVITY - 150)) {
                LOG_INFO("light sensor value transitioning to day\n");
                light_state = 1;
            }

            // 如果状态改变或者第一次运行，执行相应操作
            if (ready == 0 || light_state != ready) {
                if (light_state == 0) {
                    // 亮度调节
                    int period = 1000000;
                    int duty = 1000000;
                    rk_isp_set_light_strength(1, period, duty, PWM_POLARITY_NORMAL);
                    rk_isp_set_light_strength(0, period, duty, PWM_POLARITY_NORMAL);
                    // 关滤光片
                    // rk_isp_enable_ircut(false);
                } else {
                    // 白天关闭灯光
                    rk_isp_close_light(1);
                    rk_isp_close_light(0);
                    // 开启滤光片
                    // rk_isp_enable_ircut(true);
                }
                ready = light_state;
            }
        }
        sleep(1);
    }
    LOG_DEBUG("Exiting day_night_event thread\n");
    return NULL;
}

// 4G网卡信号质量处理线程
int rssi, ber;
static void *signal_quality(void *arg) {
    int tty_fd = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY | O_NDELAY);
    if (tty_fd < 0) {
        LOG_ERROR("Cannot open serial port device /dev/ttyUSB2");
        return NULL;
    }

    struct termios options;

    // 获取当前串口配置
    tcgetattr(tty_fd, &options);

    // 设置波特率为115200
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    // 启用接收器，并设置本地模式
    options.c_cflag |= (CLOCAL | CREAD);

    // 设置数据位为8位
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // 无校验
    options.c_cflag &= ~PARENB;

    // 设置停止位为1位
    options.c_cflag &= ~CSTOPB;

    // 设置为原始输入模式（非规范模式）
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    // 设置为原始输出模式
    options.c_oflag &= ~OPOST;

    // 将配置写入串口
    tcsetattr(tty_fd, TCSANOW, &options);

    char buf[256];
    int n;

    // 发送ATE0命令关闭回显
    n = write(tty_fd, "ATE0\r\n", 6);
    if (n < 0) {
        LOG_ERROR("Failed to send ATE0 command");
        close(tty_fd);
        return NULL;
    }

    // 等待响应
    usleep(100000);

    // 读取ATE0命令的响应
    n = read(tty_fd, buf, sizeof(buf));
    if (n < 0) {
        LOG_ERROR("Failed to read ATE0 response");
        close(tty_fd);
        return NULL;
    }

    while (g_main_run_) {
        // 发送AT+CSQ命令查询信号质量
        n = write(tty_fd, "AT+CSQ\r\n", 8);
        if (n < 0) {
            LOG_ERROR("Failed to send AT+CSQ command");
            close(tty_fd);
            return NULL;
        }

        // 等待响应
        usleep(100000);

        // 读取AT+CSQ命令的响应
        n = read(tty_fd, buf, sizeof(buf));
        if (n < 0) {
            LOG_ERROR("Failed to read AT+CSQ response");
            close(tty_fd);
            return NULL;
        }

        // 提取信号质量数据
        char *start = strstr(buf, "+CSQ: ");
        if (start) {
            sscanf(start, "+CSQ: %d,%d", &rssi, &ber);
            LOG_INFO("Extracted signal quality: RSSI=%d, BER=%d\n", rssi, ber);
        } else {
            LOG_INFO("No +CSQ response found\n");
        }

		// osd_signal_update(rssi, ber);
		// // 将信号强度信息格式化为宽字符字符串
		// swprintf(result, r_size, L"RSSI=%d, BER=%d", rssi, ber);

		// wchar_t w_signal[16] = {0};
		// int wchar_cnt = 0;


		// swprintf(w_signal, wchar_cnt, L" RSSI=%d BER=%d", rssi, ber);
        // rk_osd_set_enabled(7, value_int);
        // rk_osd_set_position_x(7, value_int);
        // rk_osd_set_position_y(7, value_int);
        // rk_osd_set_display_text(7, value.c_str());

        // 等待一段时间后再次查询
        sleep(5);
    }

    close(tty_fd);
    LOG_INFO("Signal quality thread exiting");
    return NULL;
}

int string_to_int(const char *str) {
	int num[3];
	int sum;
	for(int i=13;i<16;i++){
		if (str[i] <= '9')
			num[i-13]=str[i] - '0';
		else
			num[i-13]=10 + str[i] - 'a';
	}
	sum = num[0]*16*16 + num[1]*16 + num[2];

    return sum;
}

static void *motor_init_thread(void *arg) {
	// 云台位置初始化 
	motor_init();

    LOG_INFO("motor init thread exiting");
    return NULL;
}

int main(int argc, char **argv) {
	pthread_t key_chk;
	pthread_t gpio_key_chk;
	pthread_t gb28181_chk;
	pthread_t day_night;
	pthread_t signal_chk;
	pthread_t motor_init;
	char chipid[16];
	LOG_DEBUG("main begin\n");

	//云台初始化线程
	pthread_create(&motor_init, NULL, motor_init_thread, NULL);

	rkipc_version_dump();
	signal(SIGINT, sig_proc);
	signal(SIGTERM, sig_proc);

	// 获取程序运行时的配置
	rkipc_get_opt(argc, argv);
	LOG_INFO("rkipc_ini_path_ is %s, rkipc_iq_file_path_ is %s, rkipc_log_level "
	         "is %d\n",
	         rkipc_ini_path_, rkipc_iq_file_path_, rkipc_log_level);

	// ini配置文件加载
	rk_param_init(rkipc_ini_path_);
	// 网络模块初始化
	rk_network_init(NULL);
	// 系统管理模块初始化
	rk_system_init();
		// 获取参数
	if (rk_param_get_int("video.source:enable_npu", 0))
		// npu处理
		rkipc_rockiva_init();
	if (rk_param_get_int("video.source:enable_aiq", 1)) {
		// isp初始化
		rk_isp_init(0, rkipc_iq_file_path_);
		// 设置帧率               // 获取参数
		rk_isp_set_frame_rate(0, rk_param_get_int("isp.0.adjustment:fps", 30));

		if (rk_param_get_int("isp:init_form_ini", 1))
			// 使用ini参数覆盖iqfile参数
			rk_isp_set_from_ini(0);
	}
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	// RKMPI接口初始化
	RK_MPI_SYS_Init();
	// 视频，模块初始化
	rk_video_init();
	//音频模块初始化
	rkipc_audio_init();
	// rkipc 信号处理服务
	rkipc_server_init();
	// 存储模块初始化
	rk_storage_init();
	//创建按键处理线程
	pthread_create(&key_chk, NULL, wait_key_event, NULL);
	// 自动日夜转换
	pthread_create(&day_night, NULL, day_night_event, NULL);
	// // PIR人体传感器
	// pthread_create(&gpio_key_chk, NULL, wait_gpio_event, NULL);
	// 4G网卡信号质量查询
	pthread_create(&signal_chk, NULL, signal_quality, NULL);

	// chipid转换device id
	rk_chip_id_get(chipid);
	printf("chipid : %s", chipid);
	int num = string_to_int(chipid);
    printf("转换结果: %d\n", num);
	sprintf(device_id, "%s%04d", device_id_head,num);
	sprintf(username, "%s%04d", device_id_head,num);

	device_info();
	pthread_create(&gb28181_chk, NULL, device_start, NULL);

	//循环等待信号量结束
	while (g_main_run_) {
		usleep(1000 * 1000);
	}

	// deinit
	pthread_join(day_night, NULL);
	pthread_join(key_chk, NULL);
	// pthread_join(gpio_key_chk, NULL);
	rk_storage_deinit();
	rkipc_server_deinit();
	rk_system_deinit();
	rk_video_deinit();
	if (rk_param_get_int("video.source:enable_aiq", 1))
		rk_isp_deinit(0);
	if (rk_param_get_int("audio.0:enable", 0))
		rkipc_audio_deinit();
	RK_MPI_SYS_Exit();
	if (rk_param_get_int("video.source:enable_npu", 0))
		rkipc_rockiva_deinit();
	rk_network_deinit();
	rk_param_deinit();

	return 0;
}
