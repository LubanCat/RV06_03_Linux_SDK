#include "server.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <pthread.h>
#include "module.h"
#include "../custom/custom.h"
#include "capture.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>
#include <time.h>
#include "hardware.h"
#include <dirent.h>
#include <curl/curl.h>
#include <math.h>
#include <poll.h>
#include <signal.h>

#define MAX_BUF_SIZE 512
#define COUNT_DOWN99 99
#define COUNT_DOWN20 20
#define COUNT_DOWN10 10
#define REFRESH_RATE 10
#define DIR_NUM 20
char *local_version = "1.33";
const char *userdata_path = "/userdata/device_info.json";
const char *root_path = "/root/device_info.json";

char in_video_open_path[64];
char out_video_open_path[64];
char in_video_close_path[64];
char out_video_close_path[64];

char in_video_clean_open_path[64]   = "/root/media/clean/in_video_open.jpg";
char out_video_clean_open_path[64]  = "/root/media/clean/out_video_open.jpg";
char in_video_clean_close_path[64]  = "/root/media/clean/in_video_close.jpg";
char out_video_clean_close_path[64] = "/root/media/clean/out_video_close.jpg";

#define CONFIG_SERVER "api.nwrecycle.com"
#define CONFIG_SERVER_PORT 9096
#define UPDATE_WAIT_TIME 320

extern lv_disp_drv_t disp_drv;
char *preview_data;
char in_video[12];
char out_video[12];
char cleantime_str[100];
extern sig_atomic_t g_main_run_;

video_device_t out_video_device;
video_device_t in_video_device;

pthread_t heartbeat_thread_id = NULL;
extern int server_connect;

extern pthread_mutex_t lv_task_mutex;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t camara_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t capture_in_thread_id = NULL;
pthread_t capture_out_thread_id = NULL;
pthread_t venc_rsv_thread_id = NULL;
pthread_t opendoor_thread_id = NULL;
pthread_t debug_count_thread_id = NULL;

extern char key_input[5];
extern int key_input_count;

extern int phone_login_flag;
extern int phone_login_update;

extern char phone_number[12];
extern int phone_number_count;

int heartbeat_num=0;
int updating_flag=0;
int settlement=0;

int ir_open_flag;

int is_close_door=0;

char update_version[16], update_url[64], update_md5[64];

int is_update=0;
int key_press=0;
int key_press_end=0;

#define DEBUG_COUNT_TIME 90


const char* get_oldest_folder(const char* path1, const char* path2, struct stat *stat1, struct stat *stat2) {
    if (stat1->st_ctime < stat2->st_ctime) {
        return path1;
    }
    return path2;
}

// 删除指定路径的文件夹
void delete_folder(const char* folder_path) {
    char command[1024];
    snprintf(command, sizeof(command), "rm -rf %s", folder_path);
    system(command);
}

// 检查文件夹有没有超过特定的文件夹大小
int check_dir_num(int num){
    DIR *dir;
    struct dirent *entry;
    struct stat entry_stat;
    const char* oldest_folder = NULL;
    int folder_count = 0;

    // 打开目标文件夹
    dir = opendir("/root/media");
    if (dir == NULL) {
        perror("Failed to open directory");
        return 1;
    }

    // 遍历文件夹
    while ((entry = readdir(dir)) != NULL) {
        // 跳过当前目录和上级目录
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 获取当前项的状态信息
        char entry_path[1024];
        snprintf(entry_path, sizeof(entry_path), "/root/media/%s", entry->d_name);
        if (stat(entry_path, &entry_stat) == -1) {
            perror("Failed to get file status");
            continue;
        }

        // 如果是文件夹，则进行处理
        if (S_ISDIR(entry_stat.st_mode)) {
            folder_count++;

            // 如果是第一个文件夹，或者找到了更旧的文件夹，则更新oldest_folder
            if (oldest_folder == NULL || get_oldest_folder(oldest_folder, entry_path, &entry_stat, (struct stat*)oldest_folder) == entry_path) {
                oldest_folder = strdup(entry_path); // 复制路径以保留它
            }
        }
    }
    // 关闭目录
    closedir(dir);

    // 如果文件夹数量大于num，则删除最旧的文件夹
    if (folder_count > num) {
        if (oldest_folder != NULL) {
            printf("Deleting oldest folder: %s\n", oldest_folder);
            delete_folder(oldest_folder);
            free((void*)oldest_folder); // 释放复制的路径
        } else {
            printf("No folders to delete.\n");
        }
    } else {
        printf("Folder count is %d, no need to delete.\n", folder_count);
    }

    return 0;    
}

void* debug_count_thread(void* arg) {
    printf("enter debug_count_thread\n");
    key_press=0;
    key_press_end=0;
    int time=0;
    while(!key_press_end){
        if(key_press){
            time=0;
            key_press=0;
        }else{
            if(time < DEBUG_COUNT_TIME)
                time++;
            else{
                if(out_video_device.test_flag ||in_video_device.test_flag){
                    out_video_device.test_flag=0;
                    in_video_device.test_flag=0;
                    inside_light(0);
                    pthread_mutex_lock(&lv_task_mutex);
                    ui_load_scr_animation(&guider_ui, &guider_ui.screen_test, guider_ui.screen_test_del, &guider_ui.screen_test_cam_del, setup_scr_screen_test, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, true);
                    set_screen_test_rod_hinder_value(rod_hinder_value);
                    pthread_mutex_unlock(&lv_task_mutex);
                }

                double cur_weight= get_current_weight();
                pthread_mutex_lock(&lv_task_mutex);
                set_screen_login(&guider_ui.screen_test_del, loginRes.recyclePrice, cur_weight, "2024/06/27 15:35:99", loginRes.qrCode, heartBeat.signal,server_connect);
                set_screen_login_qrcode(loginRes.qrCode);
                pthread_mutex_unlock(&lv_task_mutex);
	            screen_login_flag = 1;
                key_press_end=1;
            }
        }
        sleep(1);
    }
    printf("end\n");
}

void* close_door_thread(void* arg) {
    printf("enter close_door_thread\n");
    set_post_door_close();
    is_close_door=1;
}

#define RTMP_URL "rtmp://10.255.200.135/live/"

void venc_rsv_thread(void* arg) {
    video_frame_t *video_frame = (video_frame_t *)arg;
    VENC_STREAM_S rcv;
    RK_S32 ret = RK_SUCCESS;
    capture_rtmp_init(video_frame,RTMP_URL);
    capture_storage_init(video_frame);
    rcv.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));
    while(1){
        ret = RK_MPI_VENC_GetStream(0, &rcv, 1000);
        if (ret == RK_SUCCESS) {
            void *result = RK_MPI_MB_Handle2VirAddr(rcv.pstPack->pMbBlk);
            if(out_video_device.record_flag){
                if ((rcv.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
                (rcv.pstPack->DataType.enH264EType == H264E_NALU_ISLICE))
                    capture_storage_write_video_frame((char *)result,rcv.pstPack->u32Len,rcv.pstPack->u64PTS,1);
                else 
                    capture_storage_write_video_frame((char *)result,rcv.pstPack->u32Len,rcv.pstPack->u64PTS,0);
            }
            if(out_video_device.rtmp_flag){
                if ((rcv.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
                (rcv.pstPack->DataType.enH264EType == H264E_NALU_ISLICE))
                    capture_rtmp_write_video_frame((char *)result,rcv.pstPack->u32Len,rcv.pstPack->u64PTS,1);
                else 
                    capture_rtmp_write_video_frame((char *)result,rcv.pstPack->u32Len,rcv.pstPack->u64PTS,0);
            }
            RK_MPI_VENC_ReleaseStream(0, &rcv);
        }else{
            printf("ret = 0x%x\n",ret);
            while(1){
                usleep(10000);
            }
        }
    }
}

//外摄像头捕捉线程
void* capture_out_thread(void* arg) {
    out_video_device.video_frame.width=640;
    out_video_device.video_frame.height=480;
    char *data=malloc(out_video_device.video_frame.width * out_video_device.video_frame.height * 3);
    int length;
    while(1){
        //判断是不是在测试，拍照，录像，推流,预览
        if(out_video_device.test_flag || out_video_device.photo_flag || out_video_device.preview_flag ||
            out_video_device.record_flag ||out_video_device.rtmp_flag){

            if(out_video_device.live==0){

                printf("out_video mutex_lock\n");
                pthread_mutex_lock(&camara_mutex);
                
                get_cam_video(1,out_video);
                sprintf(out_video_device.video_frame.device,"%s",out_video);
                if(capture_start(&out_video_device.video_frame) != -1){
                    //设置摄像头正常工作的flag
                    out_video_device.err=0;
                    out_video_device.live=1;
                    printf("out capture_start\n");
                }
                else{
                    printf("out_video_device.err\n");
                    out_video_device.err=1;
                    printf("out_video mutex_unlock\n");
                    pthread_mutex_unlock(&camara_mutex);
                }
            }
            if(out_video_device.err){
                //测试
                if(out_video_device.test_flag){
                    lv_obj_clear_flag(guider_ui.screen_test_cam_cam_wrong, LV_OBJ_FLAG_HIDDEN);
                    out_video_device.test_flag=0;
                }
                //预览
                if(out_video_device.preview_flag){      
                    upload_fault(FAULT_CAMERA);
                    lv_obj_clear_flag(guider_ui.screen_submit_cam_wrong, LV_OBJ_FLAG_HIDDEN);
                    out_video_device.preview_flag=0;
                }
                //拍照
                if(out_video_device.photo_flag){
                    upload_fault(FAULT_CAMERA);
                    out_video_device.photo_flag=0;
                }
                //录像+推流
                if(out_video_device.record_flag || out_video_device.rtmp_flag){
                    out_video_device.record_flag=0;
                    out_video_device.rtmp_flag=0;
                }
                //跳过后面操作
                continue;
            }

            //获取rgb数据
            video_linux_get_frame_rgb(&out_video_device.video_frame,data,&length);
            //测试
            // if(out_video_device.test_flag){
            //     lv_color_t color_p[640*480];
            //     int size=640*480;
            //     int length;
            
            //     lv_area_t area;
            //     area.x1=140;
            //     area.x2=779;
            //     area.y1=60;
            //     area.y2=539;

            //     memset(color_p,0x00,size*sizeof(lv_color_t));

            //     for(int j=0;j<480;j++){
            //         for(int i=0;i<640;i++){
            //             color_p[j*640+i].full |= 0xff<<24;
            //             color_p[j*640+i].full |= data[(j*640+i)*3+0]<<16;
            //             color_p[j*640+i].full |= data[(j*640+i)*3+1]<<8;
            //             color_p[j*640+i].full |= data[(j*640+i)*3+2];
            //         }
            //     }
            //     drm_flush(&disp_drv,&area,color_p);
            // }

            if(out_video_device.test_flag){
                int size=480*320;
                lv_color_t color_p[size];
                int length;
            
                lv_area_t area;
                area.x1=272;
                area.x2=591;
                area.y1=60;
                area.y2=539;

                memset(color_p,0x00,size*sizeof(lv_color_t));

                for(int j=0;j<320;j++){
                    for(int i=0;i<480;i++){
                        color_p[j+(479-i)*320].full |= 0xff<<24;
                        color_p[j+(479-i)*320].full |= data[((j+79)*640+i+79)*3+0]<<16;
                        color_p[j+(479-i)*320].full |= data[((j+79)*640+i+79)*3+1]<<8;
                        color_p[j+(479-i)*320].full |= data[((j+79)*640+i+79)*3+2];
                    }
                }
                drm_flush(&disp_drv,&area,color_p);
            }
            //预览
            if(out_video_device.preview_flag){
                int size=480*320;
                lv_color_t color_p[size];

                lv_area_t area;
                area.x1=578;
                area.x2=897;
                area.y1=60;
                area.y2=539;

                memset(color_p,0x00,size*sizeof(lv_color_t));
                for(int j=0;j<320;j++){
                    for(int i=0;i<480;i++){
                        color_p[j+(479-i)*320].full |= 0xff<<24;
                        color_p[j+(479-i)*320].full |= data[((j+79)*640+i+79)*3+0]<<16;
                        color_p[j+(479-i)*320].full |= data[((j+79)*640+i+79)*3+1]<<8;
                        color_p[j+(479-i)*320].full |= data[((j+79)*640+i+79)*3+2];
                    }
                }
                drm_flush(&disp_drv,&area,color_p);    
            }
            //拍照
            if(out_video_device.photo_flag){
                printf("capture\n");
                //判断是不是开门拍照
                if(out_video_device.photo_opendoor)
                    //开门拍照
                    capture_rgb_to_jpg(&out_video_device.video_frame,data,out_video_open_path);
                else
                    //关门拍照
                    capture_rgb_to_jpg(&out_video_device.video_frame,data,out_video_close_path);

                out_video_device.photo_flag=0;
            }
            //录像+推流
            if(out_video_device.record_flag || out_video_device.rtmp_flag){
                if(out_video_device.avc_init_flag==0){
                    capture_venc_init(&out_video_device.video_frame);
                    printf("pthread_create\n");
                    int ret = pthread_create(&venc_rsv_thread_id, NULL, venc_rsv_thread, &out_video_device.video_frame);
                    if (ret != 0) {
                        perror("pthread_create");
                    }
                    out_video_device.avc_init_flag=1;
                }
                capture_rgb_to_avc(&out_video_device.video_frame,data);
            }else{
                if(out_video_device.avc_init_flag){
                    sleep(1);
                    pthread_cancel(venc_rsv_thread_id);
                    capture_storage_deinit();
                    usleep(25000);
                    capture_venc_deinit();
                    out_video_device.avc_init_flag=0;
                }
            }
        }else{
            if(out_video_device.live){
                capture_stop(&out_video_device.video_frame);
                out_video_device.live=0;
                printf("out_video_device release\n");
                printf("out_video mutex_unlock\n");
                pthread_mutex_unlock(&camara_mutex);
            }
        }
        usleep(1000);
    }
}

//内摄像头捕捉线程
void* capture_in_thread(void* arg) {
    in_video_device.video_frame.width=640;
    in_video_device.video_frame.height=480;
    char *data=malloc(in_video_device.video_frame.width * in_video_device.video_frame.height * 3);
    int length;
    while(1){
        usleep(1000);
        //判断是不是在测试，拍照，录像，推流,预览
        if(in_video_device.test_flag || in_video_device.photo_flag){
            if(in_video_device.live==0){

                printf("in_video mutex_lock\n");
                pthread_mutex_lock(&camara_mutex);
                
                get_cam_video(0,in_video);
                sprintf(in_video_device.video_frame.device,"%s",in_video);

                if(capture_start(&in_video_device.video_frame) != -1){
                    //设置摄像头正常工作的flag
                    in_video_device.err=0;
                    in_video_device.live=1;
                    printf("in_video_device capture_start\n");
                }else{
                    printf("in_video_device.err\n");
                    in_video_device.err=1;

                    printf("in_video mutex_unlock\n");
                    pthread_mutex_unlock(&camara_mutex);
                    
                }
            }
            if(in_video_device.err){
                if(in_video_device.test_flag){
                    printf("test_flag -> err\n");
                    usleep(250000);
                    lv_obj_clear_flag(guider_ui.screen_test_cam_cam_wrong, LV_OBJ_FLAG_HIDDEN);
                    in_video_device.test_flag=0;
                    printf("test_flag == 0\n");
                }
                //拍照
                if(in_video_device.photo_flag){
                    upload_fault(FAULT_CAMERA);
                    in_video_device.photo_flag=0;
                }
                //跳过后面操作
                continue;
            }

            //获取rgb数据
            video_linux_get_frame_rgb(&in_video_device.video_frame,data,&length);

            //测试
            if(in_video_device.test_flag){
                lv_color_t color_p[640*480];
                int size=640*480;
                int length;
            
                lv_area_t area;
                area.x1=140;
                area.x2=779;
                area.y1=60;
                area.y2=539;

                memset(color_p,0x00,size*sizeof(lv_color_t));

                for(int j=0;j<480;j++){
                    for(int i=0;i<640;i++){
                        color_p[j*640+i].full |= 0xff<<24;
                        color_p[j*640+i].full |= data[(j*640+i)*3+0]<<16;
                        color_p[j*640+i].full |= data[(j*640+i)*3+1]<<8;
                        color_p[j*640+i].full |= data[(j*640+i)*3+2];
                    }
                }
                drm_flush(&disp_drv,&area,color_p);
            }
            //拍照
            if(in_video_device.photo_flag){
                //判断是不是开门拍照
                if(in_video_device.photo_opendoor)
                    //开门拍照
                    capture_rgb_to_jpg(&in_video_device.video_frame,data,in_video_open_path);
                else
                    //关门拍照
                    capture_rgb_to_jpg(&in_video_device.video_frame,data,in_video_close_path);

                in_video_device.photo_flag=0;
                printf("in_video_device photo_flag=0\n");
            }
        }else{
            if(in_video_device.live){
                capture_stop(&in_video_device.video_frame);
                in_video_device.live=0;
                printf("in_video_device release\n");

                printf("in_video mutex_unlock\n");
                pthread_mutex_unlock(&camara_mutex);
            }
        }
    }
}

int cam_init(){
    int ret ;
    get_cam_video(0,in_video);
    get_cam_video(1,out_video);
    inside_light(1);
    video_frame_t video_frame;
    video_frame.width=640;
    video_frame.height=480;
    sprintf(video_frame.device,"%s",out_video);
    ret = capture_start(&video_frame);
    if(ret < 0){
        capture_stop(&video_frame);
        return -1;
    }

    video_linux_skip_frame(&video_frame,90);
    capture_stop(&video_frame);

    sprintf(video_frame.device,"%s",in_video);
    ret = capture_start(&video_frame);
    if(ret < 0){
        capture_stop(&video_frame);
        return -1;
    }
    video_linux_skip_frame(&video_frame,90);
    capture_stop(&video_frame);
    inside_light(0);
    out_video_device.photo_flag=0;
    out_video_device.record_flag=0;
    out_video_device.test_flag=0;
    out_video_device.rtmp_flag=0;
    out_video_device.avc_init_flag=0;
    out_video_device.preview_flag=0;

    in_video_device.photo_flag=0;
    in_video_device.record_flag=0;
    in_video_device.test_flag=0;
    in_video_device.rtmp_flag=0;
    in_video_device.avc_init_flag=0;
    in_video_device.preview_flag=0;

    // out_video_device.video_frame.width=640;
    // out_video_device.video_frame.height=480;
    // char *data=malloc(out_video_device.video_frame.width * out_video_device.video_frame.height * 3);
    // int length;
    // sprintf(out_video_device.video_frame.device,"%s",out_video);
    // capture_start(&out_video_device.video_frame);
    // video_linux_get_frame_rgb(&out_video_device.video_frame,data,&length);
    // capture_stop(&out_video_device.video_frame);

    // in_video_device.video_frame.width=640;
    // in_video_device.video_frame.height=480;
    // sprintf(in_video_device.video_frame.device,"%s",in_video);
    // capture_start(&in_video_device.video_frame);
    // video_linux_get_frame_rgb(&in_video_device.video_frame,data,&length);
    // capture_stop(&in_video_device.video_frame);

    // free(data);

    ret = pthread_create(&capture_in_thread_id, NULL, capture_in_thread, NULL);
    if (ret != 0) {
        perror("pthread_create");
    }

    ret = pthread_create(&capture_out_thread_id, NULL, capture_out_thread, NULL);
    if (ret != 0) {
        perror("pthread_create");
    }
}

// 设备信息
char sn[32];
char key[32];

// 文件路径
char *photo_file = "/root/photo.jpg";
char *video_file = "/root/video.mp4";

int sock;
char trans_Id[64];  // 事务ID

bool g_blConnected = false;

// 声明互斥锁

LoginConfig loginConfig;
LoginRes loginRes;
OpenDoor openDoor;
CloseDoor closeDoor;
HeartBeat heartBeat;

int send_tcp_packet(char *buf, int size)
{
    pthread_mutex_lock(&mutex);
    // 发送数据
    int bytes_sent = send(sock, buf, size, 0);
    pthread_mutex_unlock(&mutex);
    
    if (bytes_sent != size) {
        // 发送的数据量不匹配，可能是网络问题
        fprintf(stderr, "Warning: Only %d bytes sent out of %d\n", bytes_sent, size);
        perror("--------- send data failed---------------");
        
        close(sock);
        sock = -1;
        server_connect = 0;

        return -1;
    }
    
    return bytes_sent;
}

// 检查并移动配置文件
int check_and_move_file(const char *src, const char *dst) {
    struct stat buffer;

 // 检查目标文件是否存在
    if (stat(dst, &buffer) != 0) {
        if (errno == ENOENT) {
            // 目标文件不存在，检查源文件是否存在
            if (stat(src, &buffer) == 0) {
                // 源文件存在，使用shell命令移动到目标路径
                char command[256];
                snprintf(command, sizeof(command), "mv %s %s", src, dst);
                int ret = system(command);
                if (ret != 0) {
                    perror("移动文件时出错");
                    return -1;
                }
            } else {
                perror("找不到设备配置文件");
                return -1;
            }
        } else {
            perror("检查目标文件时出错");
            return -1;
        }
    }
    return 0;
}

int read_device_info() {

    check_and_move_file(root_path, userdata_path);

    // 打开文件
    FILE *file = fopen("/userdata/device_info.json", "r");
    if (file == NULL) {
        perror("无法打开设备配置文件");
        return -1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存来存储文件内容
    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL) {
        printf("无法分配内存\n");
        fclose(file);
        return -1;
    }

    // 读取文件内容
    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0'; // 确保文件内容以空字符结尾

    // 关闭文件
    fclose(file);

    // 解析 JSON 数据
    cJSON *root = cJSON_Parse(file_content);
    if (root == NULL) {
        printf("无法解析 JSON 数据\n");
        free(file_content);
        return -1;
    }

    // 获取 sn 和 key 字符串
    cJSON *sn_item = cJSON_GetObjectItem(root, "sn");
    cJSON *key_item = cJSON_GetObjectItem(root, "key");
    cJSON *self_weight_item = cJSON_GetObjectItem(root, "self_weight");
    cJSON *rat_item = cJSON_GetObjectItem(root, "rat");
    cJSON *rod_hinder_item = cJSON_GetObjectItem(root, "rod_hinder");

    if (cJSON_IsString(sn_item)) {
        strncpy(sn, sn_item->valuestring, 32 - 1); // 复制 sn 字符串到 sn 数组
        sn[32 - 1] = '\0'; // 确保字符串以空字符结尾
        printf("SN: %s\n", sn);
    }
    if (cJSON_IsString(key_item)) {
        strncpy(key, key_item->valuestring, 32 - 1); // 复制 key 字符串到 key 数组
        key[32 - 1] = '\0'; // 确保字符串以空字符结尾
        printf("Key: %s\n", key);
    }
    if (cJSON_IsString(self_weight_item)) {
        g_sRecycle_Box_Cnf.self_adc_var = atoi(self_weight_item->valuestring);
        printf("去皮重量 self_weight_adc: %d\n", g_sRecycle_Box_Cnf.self_adc_var);
    }
    if (cJSON_IsString(rat_item)) {
        g_sRecycle_Box_Cnf.rat = atof(rat_item->valuestring);
        printf("称重系数 rat: %.16f\n", g_sRecycle_Box_Cnf.rat);
    }
    if (cJSON_IsString(rod_hinder_item)) {
        rod_hinder_value = atof(rod_hinder_item->valuestring);
        printf("推杆阻力阈值 rat: %d\n", rod_hinder_value);
    }
    // 释放内存
    cJSON_Delete(root);
    free(file_content);

    return 0;
}

// 写入或修改设备配置文件的单个键值
// 键值转为char后传入
int write_device_info(const char *key_name, const char *value) {
    cJSON *root = NULL;

    // 尝试打开现有配置文件
    FILE *file = fopen("/userdata/device_info.json", "r");
    if (file == NULL) {
        // 如果文件不存在，创建一个空的 JSON 对象
        root = cJSON_CreateObject();
        if (root == NULL) {
            printf("无法创建 JSON 对象\n");
            return -1;
        }
    } else {
        // 文件存在，解析其中的 JSON 数据
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (file_size == 0) {
            // 如果文件内容为空，创建一个空的 JSON 对象
            root = cJSON_CreateObject();
            if (root == NULL) {
                printf("无法创建 JSON 对象\n");
                fclose(file);
                return -1;
            }
        } else {
            // 文件内容不为空，读取文件内容并解析
            char *file_content = (char *)malloc(file_size + 1);
            if (file_content == NULL) {
                printf("无法分配内存\n");
                fclose(file);
                return -1;
            }

            fread(file_content, 1, file_size, file);
            file_content[file_size] = '\0';

            fclose(file);

            root = cJSON_Parse(file_content);
            free(file_content);
            if (root == NULL) {
                printf("无法解析 JSON 数据, 创建新的 JSON 对象\n");
                // 如果无法解析 JSON 数据，创建一个新的空 JSON 对象
                root = cJSON_CreateObject();
                if (root == NULL) {
                    printf("无法创建 JSON 对象\n");
                    return -1;
                }
            }
        }
    }

    // 添加或修改键值对
    cJSON *item = cJSON_GetObjectItem(root, key_name);
    if (item == NULL) {
        cJSON_AddStringToObject(root, key_name, value);
    } else {
        cJSON_SetValuestring(item, value);
    }

    // 将 JSON 对象转换为字符串
    char *json_str = cJSON_PrintUnformatted(root);
    if (json_str == NULL) {
        printf("无法生成 JSON 字符串\n");
        cJSON_Delete(root);
        return -1;
    }

    // 将字符串写入配置文件
    int fd = open("/userdata/device_info.json", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("无法打开设备配置文件");
        cJSON_Delete(root);
        free(json_str);
        return -1;
    }

    if (write(fd, json_str, strlen(json_str)) == -1) {
        perror("写入失败");
        close(fd);
        cJSON_Delete(root);
        free(json_str);
        return -1;
    }

    // 确保数据立即同步到磁盘
    if (fsync(fd) == -1) {
        perror("fsync 失败");
        close(fd);
        cJSON_Delete(root);
        free(json_str);
        return -1;
    }

    close(fd);
    cJSON_Delete(root);
    free(json_str);

    return 0;
}

int ping_and_extract_ip(const char *host, char *ip) {
    FILE *fp;
    char path[1035];
    regex_t regex;
    regmatch_t pmatch[1];
    const char *pattern = "([0-9]{1,3}\\.){3}[0-9]{1,3}";

    // 打开用于读取的命令
    char command[256];
    snprintf(command, sizeof(command), "ping -c 1 %s", host);
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("无法执行命令\n");
        return -1;
    }

    // 编译正则表达式
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        printf("无法编译正则表达式\n");
        pclose(fp);
        return -1;
    }

    // 读取命令的输出并查找匹配的 IP 地址
    while (fgets(path, sizeof(path) - 1, fp) != NULL) {
        if (regexec(&regex, path, 1, pmatch, 0) == 0) {
            int start = pmatch[0].rm_so;
            int end = pmatch[0].rm_eo;
            strncpy(ip, path + start, end - start);
            ip[end - start] = '\0';
            regfree(&regex);
            pclose(fp);
            return 0;
        }
    }

    // 释放正则表达式和关闭文件指针
    regfree(&regex);
    pclose(fp);

    printf("未找到匹配的 IP 地址\n");
    return -1;
}


// 获取登录配置
int get_login_config() {
    struct sockaddr_in server_addr;
    char recv_buf[MAX_BUF_SIZE];
    int recv_len;

    char ip[16];
    const char *host = CONFIG_SERVER;
    int max_retries = 5; // 最大重试次数
    int retry_count = 0;

    while (retry_count < max_retries) {
        if (ping_and_extract_ip(host, ip) == 0) {
            printf("提取的 IP 地址: %s\n", ip);
            break;
        } else {
            printf("无法提取 IP 地址，10 秒后重试...\n");
            sleep(10); // 等待 10 秒
            retry_count++;
        }
    }

    if (retry_count == max_retries) {
        printf("无法获取服务器IP，程序退出。\n");
        return -1;
    }

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CONFIG_SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sock);
        return -1;
    }

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "loginConfig");
    cJSON_AddStringToObject(root, "iccid", moduleInfo.iccid);
    cJSON_AddStringToObject(root, "sn", sn);
    cJSON_AddStringToObject(root, "key", key);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送登录配置请求
    printf("Sending loginConfig request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0);

    if (recv_len < 0) {
        perror("Receive failed");
        close(sock);
        return -1;
    } else {
        recv_buf[recv_len] = '\0';
        printf("Server response: %s\n", recv_buf);

        // 解析 JSON 响应
        cJSON *response_json = cJSON_Parse(recv_buf);
        if (response_json == NULL) {
            printf("JSON parse error\n");
            close(sock);
            return -1;
        }

        cJSON *retCode = cJSON_GetObjectItem(response_json, "retCode");
        if (retCode == NULL || !cJSON_IsNumber(retCode)) {
            printf("Invalid retCode\n");
            cJSON_Delete(response_json);
            close(sock);
            return -1;
        }

        switch (retCode->valueint) {
            case 0:
                // 成功，获取额外数据
                {
                    cJSON *ip = cJSON_GetObjectItem(response_json, "ip");
                    cJSON *tcpPort = cJSON_GetObjectItem(response_json, "tcp");

                    if (ip && tcpPort) {
                        strncpy(loginConfig.ip, ip->valuestring, sizeof(loginConfig.ip)-1);
                        loginConfig.tcpPort = tcpPort->valueint;
                    } else {
                        printf("Missing ip or tcpPort in response\n");
                        cJSON_Delete(response_json);
                        close(sock);
                        return -1;
                    }
                }
                break;
            case 1:
                printf("被服务商禁用\n");
                break;
            case 2:
                printf("被管理员禁用\n");
                break;
            case 3:
                printf("SN或KEY错误\n");
                break;
            case 4:
                printf("请绑定设备\n");
                break;
            default:
                printf("Unknown retCode\n");
                cJSON_Delete(response_json);
                close(sock);
                return -1;
        }

        cJSON_Delete(response_json);
    }

    // 关闭套接字
    close(sock);

    return 0;
}

// 登录
int server_login() {

    char recv_buf[MAX_BUF_SIZE];
    int recv_len;

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "login");
    cJSON_AddStringToObject(root, "imsi", moduleInfo.imsi);
    cJSON_AddStringToObject(root, "imei", moduleInfo.imei);
    cJSON_AddStringToObject(root, "version", local_version);
    cJSON_AddStringToObject(root, "iccid", moduleInfo.iccid);
    cJSON_AddStringToObject(root, "sn", sn);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送登录请求
    printf("Sending login request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    // 接收服务器响应
    recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0);

    if (recv_len <= 0) {
        perror("Receive failed");
        return -1;
    }else {
        recv_buf[recv_len] = '\0';
        printf("Server response: %s\n", recv_buf);

        // 解析 JSON 响应
        cJSON *response_json = cJSON_Parse(recv_buf);
        if (response_json == NULL) {
            printf("JSON parse error\n");
            return -1;
        }

        cJSON *retCode = cJSON_GetObjectItem(response_json, "retCode");
        if (retCode == NULL || !cJSON_IsNumber(retCode)) {
            printf("Invalid retCode\n");
            cJSON_Delete(response_json);
            return -1;
        }

        switch (retCode->valueint) {
            case 0: {
                cJSON *qrCode = cJSON_GetObjectItem(response_json, "qrCode");
                cJSON *recyclePrice = cJSON_GetObjectItem(response_json, "recyclePrice");
                cJSON *heartBeatInterval = cJSON_GetObjectItem(response_json, "heartBeatInterval");
                cJSON *turnOnLight = cJSON_GetObjectItem(response_json, "turnOnLight");
                cJSON *turnOffLight = cJSON_GetObjectItem(response_json, "turnOffLight");
                cJSON *ir_flag = cJSON_GetObjectItem(response_json, "ir");

                if(ir_flag->valueint){
                    ir_open_flag = 1;
                }else{
                    ir_open_flag = 0;
                }

                if (qrCode && recyclePrice && heartBeatInterval && turnOnLight && turnOffLight) {
                    strncpy(loginRes.qrCode, qrCode->valuestring, sizeof(loginRes.qrCode) - 1);
                    loginRes.qrCode[sizeof(loginRes.qrCode) - 1] = '\0';
                    loginRes.recyclePrice = atof(recyclePrice->valuestring);
                    loginRes.heartBeatInterval = atoi(heartBeatInterval->valuestring);

                    // 解析 turnOnLight 和 turnOffLight 时间字符串
                    memset(&loginRes.turnOnLight, 0, sizeof(struct tm));
                    memset(&loginRes.turnOffLight, 0, sizeof(struct tm));

                    if (sscanf(turnOnLight->valuestring, "%2d%2d", &loginRes.turnOnLight.tm_hour, &loginRes.turnOnLight.tm_min) != 2) {
                        printf("Failed to parse turnOnLight time\n");
                        cJSON_Delete(response_json);
                        return -1;
                    }

                    if (sscanf(turnOffLight->valuestring, "%2d%2d", &loginRes.turnOffLight.tm_hour, &loginRes.turnOffLight.tm_min) != 2) {
                        printf("Failed to parse turnOffLight time\n");
                        cJSON_Delete(response_json);
                        return -1;
                    }

                    // 打印获取的时间
                    char turnOnLightBuf[6], turnOffLightBuf[6];
                    strftime(turnOnLightBuf, sizeof(turnOnLightBuf), "%H:%M", &loginRes.turnOnLight);
                    strftime(turnOffLightBuf, sizeof(turnOffLightBuf), "%H:%M", &loginRes.turnOffLight);

                    printf("Turn On Light Time: %s\n", turnOnLightBuf);
                    printf("Turn Off Light Time: %s\n", turnOffLightBuf);
                } else {
                    printf("Invalid response: missing expected fields\n");
                    cJSON_Delete(response_json);
                    return -1;
                }
                break;
            }
            case 1:
                printf("Login failed: 被服务商禁用\n");
                break;
            case 2:
                printf("Login failed: 被管理员禁用\n");
                break;
            case 3:
                printf("Login failed: SN或KEY错误\n");
                break;
            default:
                printf("Unknown retCode\n");
                cJSON_Delete(response_json);
                return -1;
        }
        cJSON_Delete(response_json);
    }
    return 0;
}

void handle_open_door(cJSON *response_json) {
    cJSON *transId = cJSON_GetObjectItem(response_json, "transId");
    cJSON *openType = cJSON_GetObjectItem(response_json, "openType");
    double current_weight;
    char *json_str;
    cJSON *root;
    int retCode = 0;
    
    if(opendoor_thread_id != NULL){
        retCode = 1;
        current_weight = get_current_weight(); // 获取当前重量的示例函数
        if (current_weight < 0)
            current_weight = 0.0;

        root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "cmd", "openDoor");
        cJSON_AddNumberToObject(root, "retCode", 1);
        cJSON_AddStringToObject(root, "transId", transId->valuestring);
        cJSON_AddNumberToObject(root, "curWeight", current_weight);
        json_str = cJSON_PrintUnformatted(root);
    }else{
        strncpy(trans_Id, transId->valuestring, sizeof(trans_Id) - 1);
        trans_Id[sizeof(trans_Id) - 1] = '\0';
        openDoor.openType = openType->valueint;
        current_weight = get_current_weight(); // 获取当前重量的示例函数
        if (current_weight < 0)
            current_weight = 0.0;
        openDoor.curWeight = current_weight;

        root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "cmd", "openDoor");
        cJSON_AddNumberToObject(root, "retCode", retCode);
        cJSON_AddStringToObject(root, "transId", trans_Id);
        cJSON_AddNumberToObject(root, "curWeight", openDoor.curWeight);
        json_str = cJSON_PrintUnformatted(root);
    }

    // 发送响应
    printf("Sending openDoor response: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    if(opendoor_thread_id == NULL){
        int ret = pthread_create(&opendoor_thread_id, NULL, opendoor_thread, NULL);
        if (ret != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }else
        printf("other program is running\n");
}

void handle_phone(cJSON *response_json) {
    int opendoor_retCode = 0;
    cJSON *retCode = cJSON_GetObjectItem(response_json, "retCode");
    printf("retCode = %d\n",retCode->valueint);
    if(retCode->valueint == 0  || retCode->valueint == 1){
        cJSON *transId = cJSON_GetObjectItem(response_json, "transId");
        strncpy(trans_Id, transId->valuestring, sizeof(trans_Id) - 1);
        trans_Id[sizeof(trans_Id) - 1] = '\0';
        printf("transId = %s ,retCode = %d\n",trans_Id,retCode->valueint);
        printf("open the door\n");
        
        phone_login_flag=0;
        phone_login_update=0;
        phone_number_count=0;

        usleep(300000);

        if(retCode->valueint == 1){
            pthread_mutex_lock(&lv_task_mutex);
            lv_obj_clear_flag(guider_ui.screen_login_phone_notice, LV_OBJ_FLAG_HIDDEN);
            pthread_mutex_unlock(&lv_task_mutex);

            pthread_mutex_lock(&lv_task_mutex);
            lv_obj_add_flag(guider_ui.screen_login_keyboard, LV_OBJ_FLAG_HIDDEN);
            pthread_mutex_unlock(&lv_task_mutex);

            sleep(3);
        }

        double current_weight = get_current_weight(); // 获取当前重量的示例函数
        if (current_weight < 0)
            current_weight = 0.0;

        openDoor.curWeight = current_weight;
        openDoor.openType=1;

        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "cmd", "openDoor");
        cJSON_AddNumberToObject(root, "retCode", opendoor_retCode);
        cJSON_AddStringToObject(root, "transId", trans_Id);
        cJSON_AddNumberToObject(root, "curWeight", openDoor.curWeight);
        char *json_str = cJSON_PrintUnformatted(root);

        // 发送响应
        printf("Sending openDoor response: %s\n", json_str);
        send_tcp_packet(json_str, strlen(json_str));
        free(json_str);
        cJSON_Delete(root);

        int ret = pthread_create(&opendoor_thread_id, NULL, opendoor_thread, NULL);
        if (ret != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }else{
        usleep(300000);
        printf("wrong input\n");
        pthread_mutex_lock(&lv_task_mutex);
        lv_obj_clear_flag(guider_ui.screen_login_phone_bad, LV_OBJ_FLAG_HIDDEN);
        pthread_mutex_unlock(&lv_task_mutex);
        sleep(3);
        pthread_mutex_lock(&lv_task_mutex);
        lv_obj_add_flag(guider_ui.screen_login_phone_bad, LV_OBJ_FLAG_HIDDEN);
        pthread_mutex_unlock(&lv_task_mutex);
    }
}

void handle_initconfig(cJSON *response_json) {
    int retCode;

    cJSON *recyclePrice = cJSON_GetObjectItem(response_json, "recyclePrice");
    cJSON *heartBeatInterval = cJSON_GetObjectItem(response_json, "heartBeatInterval");
    cJSON *turnOnLight = cJSON_GetObjectItem(response_json, "turnOnLight");
    cJSON *turnOffLight = cJSON_GetObjectItem(response_json, "turnOffLight");
    cJSON *ir_flag = cJSON_GetObjectItem(response_json, "ir");

    if(ir_flag->valueint){
        ir_open_flag = 1;
    }else{
        ir_open_flag = 0;
    }

    if (recyclePrice && heartBeatInterval && turnOnLight && turnOffLight) {
        loginRes.recyclePrice = atof(recyclePrice->valuestring);
        loginRes.heartBeatInterval = atoi(heartBeatInterval->valuestring);

        // 解析 turnOnLight 和 turnOffLight 时间字符串
        memset(&loginRes.turnOnLight, 0, sizeof(struct tm));
        memset(&loginRes.turnOffLight, 0, sizeof(struct tm));

        if (sscanf(turnOnLight->valuestring, "%2d%2d", &loginRes.turnOnLight.tm_hour, &loginRes.turnOnLight.tm_min) != 2) {
            printf("Failed to parse turnOnLight time\n");
            return -1;
        }

        if (sscanf(turnOffLight->valuestring, "%2d%2d", &loginRes.turnOffLight.tm_hour, &loginRes.turnOffLight.tm_min) != 2) {
            printf("Failed to parse turnOffLight time\n");
            return -1;
        }

        // 打印获取的时间
        char turnOnLightBuf[6], turnOffLightBuf[6];
        strftime(turnOnLightBuf, sizeof(turnOnLightBuf), "%H:%M", &loginRes.turnOnLight);
        strftime(turnOffLightBuf, sizeof(turnOffLightBuf), "%H:%M", &loginRes.turnOffLight);

        printf("Turn On Light Time: %s\n", turnOnLightBuf);
        printf("Turn Off Light Time: %s\n", turnOffLightBuf);

        pthread_mutex_lock(&lv_task_mutex);
        set_screen_login_price(loginRes.recyclePrice);
        pthread_mutex_unlock(&lv_task_mutex);

        int retCode = 0;
    } else {
        printf("Invalid response: missing expected fields\n");
        int retCode = 1;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "initConfig");
    cJSON_AddNumberToObject(root, "retCode", retCode);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送响应
    printf("Sending ota response: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);
}

void handle_debug() {
    int retCode = 0;

    screen_login_flag=0;
    pthread_mutex_lock(&lv_task_mutex);
    set_screen_test(&guider_ui.screen_login_del);
    pthread_mutex_unlock(&lv_task_mutex);

    int ret = pthread_create(&debug_count_thread_id, NULL, debug_count_thread, NULL);
    if (ret != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "debug");
    cJSON_AddNumberToObject(root, "retCode", retCode);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送响应
    printf("Sending ota response: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);
}

// 线程函数，用于执行定时重启
void *scheduled_reboot(void *arg) {
    time_t target_time = *(time_t *)arg;
    free(arg);

    while (difftime(target_time, time(NULL)) > 0) {
        sleep(1);
    }
    system("reboot");
    return NULL;
}

void handle_restart(cJSON *response_json) {
    int retCode = 0;
    cJSON *type = cJSON_GetObjectItem(response_json, "type");

    if (type->valueint == 1) {
        system("reboot");
    } else if (type->valueint == 2) {
        cJSON *time = cJSON_GetObjectItem(response_json, "time");
        struct tm scheduled_time;
        memset(&scheduled_time, 0, sizeof(struct tm));

        // 解析时间字符串 "yyyyMMddHHmmss"
        printf("time:%s\n", time->valuestring);
        if (strptime(time->valuestring, "%Y-%m-%d %H:%M:%S", &scheduled_time)) {
            time_t *target_time = malloc(sizeof(time_t));
            *target_time = mktime(&scheduled_time);

            pthread_t reboot_thread;
            if (pthread_create(&reboot_thread, NULL, scheduled_reboot, target_time) == 0) {
                pthread_detach(reboot_thread);
            } else {
                retCode = 1;
                free(target_time);
            }
        } else {
            fprintf(stderr, "Error: Failed to parse time string.\n");
            retCode = 1; // 时间格式错误
        }
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "restart");
    cJSON_AddNumberToObject(root, "retCode", retCode);
    char *json_str = cJSON_PrintUnformatted(root);

     // 发送响应
    printf("Sending restart response: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);
}

// 比较版本
int compare_versions(const char *v1, const char *v2) {
    int v1_major, v1_minor, v2_major, v2_minor;
    sscanf(v1, "%d.%d", &v1_major, &v1_minor);
    sscanf(v2, "%d.%d", &v2_major, &v2_minor);
    if (v1_major > v2_major) return 1;
    if (v1_major < v2_major) return -1;
    if (v1_minor > v2_minor) return 1;
    if (v1_minor < v2_minor) return -1;
    return 0;
}

int download_file(const char *url, const char *filename) {
    printf("Downloading file from %s to %s\n", url, filename);
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(filename, "wb");
        if (fp == NULL) {
            printf("Error opening file %s for writing.\n", filename);
            curl_easy_cleanup(curl);
            return 1;
        }
        // 设置连接超时时间为 10 秒
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        // 设置传输超时时间为 300 秒
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        fclose(fp);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK) {
            printf("File downloaded successfully.\n");
            return 0;
        } else {
            printf("File download failed: %s\n", curl_easy_strerror(res));
            return 1;
        }
    }
    printf("Error initializing CURL.\n");
    return 1;
}

int calculate_md5(const char *filename, char *md5_str) {
    printf("Calculating MD5 for %s\n", filename);
    char command[128];
    sprintf(command, "md5sum %s | awk '{ print $1 }'", filename);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        printf("Error opening pipe for MD5 calculation.\n");
        return 1;
    }
    fgets(md5_str, 64, fp);
    pclose(fp);
    // Remove the trailing newline character
    md5_str[strcspn(md5_str, "\n")] = 0;
    printf("Calculated MD5: %s\n", md5_str);
    return 0;
}


void delete_file(const char *filename) {
    if (remove(filename) != 0) {
        perror("Error deleting file");
    } else {
        printf("File %s successfully deleted\n", filename);
    }
}


void set_file_executable(const char *filename) {
    if (chmod(filename, S_IRWXU) != 0) {
        perror("Error setting file executable");
    } else {
        printf("File %s is now executable\n", filename);
    }
}

void *update_thread(void* arg){
    const char *filename = "/root/lvgl_app_temp";
    const char *lvgl_filename = "/root/lvgl_app";

    updating_flag=1; 
    printf("download\n");
    // 下载文件
    if (download_file(update_url, filename) == 0) {
        // 校验 MD5
        char downloaded_md5[64];
        printf("calculate_md5\n");
        if (calculate_md5(filename, downloaded_md5) == 0 && strcmp(downloaded_md5, update_md5) == 0) {
            printf("MD5 verification succeeded.\n");

            // 设置文件可执行
            set_file_executable(filename);

            // 尝试重命名文件
            if (rename(filename, lvgl_filename) == 0) {
                printf("File renamed successfully.\n");
            } else {
                perror("Error renaming file"); // 打印错误信息
            }

            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "cmd", "ota");
            cJSON_AddNumberToObject(root, "retCode", 0);
            char *json_str = cJSON_PrintUnformatted(root);

            // 发送响应
            printf("Sending ota response: %s\n", json_str);
            send_tcp_packet(json_str, strlen(json_str));
            free(json_str);
            cJSON_Delete(root);
            is_update=1;
            return ;
        } else {
            printf("MD5 verification failed.\n");
            // 如果 MD5 校验失败，删除文件
            delete_file(filename);
        }
    } else {
        printf("File download failed.\n");
        delete_file(filename);
    }
    is_update=2;
    return ;
}

void handle_ota(cJSON *response_json) {
    cJSON *version_item = cJSON_GetObjectItem(response_json, "version");
    cJSON *url_item = cJSON_GetObjectItem(response_json, "url");
    cJSON *md5_item = cJSON_GetObjectItem(response_json, "md5");

    if (version_item && url_item && md5_item) {
        printf("Parsed JSON successfully.\n");
        printf("Version: %s\n", version_item->valuestring);
        printf("URL: %s\n", url_item->valuestring);
        printf("MD5: %s\n", md5_item->valuestring);

        strncpy(update_version, version_item->valuestring, sizeof(update_version) - 1);
        update_version[sizeof(update_version) - 1] = '\0';
        strncpy(update_url, url_item->valuestring, sizeof(update_url) - 1);
        update_url[sizeof(update_url) - 1] = '\0';
        strncpy(update_md5, md5_item->valuestring, sizeof(update_md5) - 1);
        update_md5[sizeof(update_md5) - 1] = '\0';

        int retCode = 1;

        if (compare_versions(update_version, local_version) > 0) {
            printf("New version available: %s\n", update_version);
            pthread_t update_thread_id;
            if (pthread_create(&update_thread_id, NULL, update_thread, NULL) != 0) 
                perror("Failed to create heartbeat thread");
            int time=UPDATE_WAIT_TIME;
            is_update=0;
            while(!is_update && (time > 0)){
                sleep(1);
                time--;
            }
            if(is_update==1){
                // 退出程序
                exit(0);
            }else{
                //取消线程
                int rc = pthread_kill(update_thread_id, 0); // 0表示不发送信号，仅检查线程是否存在
                if (rc == 0) {
                    if (pthread_cancel(update_thread_id) != 0) {
                        perror("线程取消失败");
                    }

                    // 等待线程结束
                    if (pthread_join(update_thread_id, NULL) != 0) {
                        perror("等待线程失败");
                    }
                    printf("线程已取消\n");
                } else if (rc == ESRCH) {
                    printf("线程不存在\n");
                }
            }
        
        } else {
            printf("Current version is up to date.\n");
        }
        updating_flag=0;
        // 发送失败的响应
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "cmd", "ota");
        cJSON_AddNumberToObject(root, "retCode", retCode);
        char *json_str = cJSON_PrintUnformatted(root);

        // 发送响应
        printf("Sending ota response: %s\n", json_str);
        send_tcp_packet(json_str, strlen(json_str));
        free(json_str);
        cJSON_Delete(root);
    } else {
        printf("Failed to parse JSON.\n");
    }
}


void message_receiver_handler(const char *recv_buf) {
    // 解析 JSON 响应
    cJSON *response_json = cJSON_Parse(recv_buf);
    if (response_json == NULL) {
        printf("JSON parse error\n");
        return;
    }

    cJSON *cmd = cJSON_GetObjectItem(response_json, "cmd");
    if (cmd == NULL) {
        printf("Invalid command\n");
        cJSON_Delete(response_json);
        return;
    }

    char str[20];
    strncpy(str, cmd->valuestring, sizeof(str) - 1);
    str[sizeof(str) - 1] = '\0';

    if (strcmp(str, "openDoor") == 0) {
        handle_open_door(response_json);
    } else if (strcmp(str, "ota") == 0) {
        handle_ota(response_json);
    } else if (strcmp(str, "initConfig") == 0) {
        handle_initconfig(response_json);
    } else if (strcmp(str, "debug") == 0) {
        handle_debug();
    } else if (strcmp(str, "restart") == 0) {
        handle_restart(response_json);
    } else if (strcmp(str, "heartBeat") == 0) {
        printf("get heartBeat ret\n");
        heartbeat_num=0;
    } else if (strcmp(str, "phoneNumberLogin") == 0) {
        printf("get phoneNumberLogin ret\n");
        handle_phone(response_json);
    } else {
        cJSON *retCode = cJSON_GetObjectItem(response_json, "retCode");
        if (retCode == NULL || !cJSON_IsNumber(retCode)) {
            printf("Invalid retCode\n");
            return;
        }        
    }

    cJSON_Delete(response_json);
}

// 心跳线程函数
void *heartbeat_thread(void *arg) 
{
    while (g_main_run_) 
    {
        if (server_connect) {
            heartbeat_num=1;
            heartBeat.curWeight = get_current_weight();
            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "cmd", "heartBeat");
            cJSON_AddNumberToObject(root, "signal", heartBeat.signal);
            cJSON_AddNumberToObject(root, "curWeight", heartBeat.curWeight);
            
            char *json_str = cJSON_PrintUnformatted(root);

            // 发送心跳上传请求
            printf("Sending heartBeat request: %s\n", json_str);
            send_tcp_packet(json_str, strlen(json_str));
            free(json_str);
            
            cJSON_Delete(root);
        }

        sleep(10);

        if(heartbeat_num == 1){
            heartbeat_num=0;
            printf("heartbeat cause server_connect\n");
            server_connect=0;
        }
        // 休眠0
        sleep(loginRes.heartBeatInterval-10);
    }

    return NULL;
}


// 心跳线程函数
int allow_phone_login(char *phone_number) 
{

    if (server_connect) {
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "cmd", "phoneNumberLogin");
        cJSON_AddStringToObject(root, "phoneNumber", phone_number);
        
        char *json_str = cJSON_PrintUnformatted(root);

        // 发送心跳上传请求
        printf("Sending phone login request: %s\n", json_str);
        send_tcp_packet(json_str, strlen(json_str));
        free(json_str);
        
        cJSON_Delete(root);
    }
}

// 开门
void* opendoor_thread(void* arg) {
    close_door_flag=0;
    post_flag=0;
    quit_flag=0;
    open_clean_door_flag=0;
    hand_close_door_flag=0;

    // 关闭登录页面元素更新
    screen_login_flag=0;

    usleep(400 * 1000);

    double sub_weight =  openDoor.curWeight; //开门命令上传给服务器的重量

    printf("开门: openType=%d\n", openDoor.openType);

    if (openDoor.openType == 1) {
        // 满溢时进入满溢界面，不开投递门
        int overflow_status = get_overflow_status();
        overflow_status=1;
        if (overflow_status == 3) {
            printf("回收柜已满\n");
            upload_overflow(overflow_status);

            // printf("当前回收柜已满，请清运后再投递\n");
            // set_screen_full(&guider_ui.screen_login_del,loginRes.recyclePrice,COUNT_DOWN10);
            // for (int i = COUNT_DOWN10; i > 0; i--) {
            //     printf("倒计时: %d秒 后返回登录界面\n", i);
            //     pthread_mutex_lock(&lv_task_mutex);
            //     set_screen_full_time(i);
            //     pthread_mutex_unlock(&lv_task_mutex);
            //     sleep(1);
            // }
            // // 返回到登录页面
            // set_screen_login(&guider_ui.screen_full_del, loginRes.recyclePrice, sub_weight, cleantime_str, loginRes.qrCode, heartBeat.signal,server_connect);
        } else {
            int i = COUNT_DOWN99 * REFRESH_RATE;
            double cur_weight = 0, diff_weight, money;
            pthread_t capture_thread_id;
            char line[20];
            int transid_fd;
            ssize_t bytes_written;

            const char *filename = "/root/opendoor.txt";
            // reset_cam(0);
            // reset_cam(1);
            inside_light(1);
            transid_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            if (transid_fd == -1) {
                perror("Error opening file");
            }
            
            sprintf(line,"%s\n",trans_Id);

            bytes_written = write(transid_fd, line, strlen(line));
            if (bytes_written == -1) {
                perror("Error writing to file");
                close(transid_fd);
            }

            printf("set_screen_submit\n");
            // 投递界面
            pthread_mutex_lock(&lv_task_mutex);
            set_screen_submit(&guider_ui.screen_login_del,0,COUNT_DOWN99,0);
            pthread_mutex_unlock(&lv_task_mutex);

            //获取当前时间
            char currentTime[20]; // 足够存储 "YYYYMMDD_HHMMSS" 和结束符
            time_t now;
            struct tm *localNow;
            time(&now);
            localNow = localtime(&now);
            strftime(currentTime, sizeof(currentTime), "%Y%m%d_%H%M%S", localNow);
            
            sprintf(line,"%s\n",currentTime);

            bytes_written = write(transid_fd, line, strlen(line));
            if (bytes_written == -1) {
                perror("Error writing to file");
                close(transid_fd);
            }

            sprintf(line,"%0.2f\n",sub_weight);

            bytes_written = write(transid_fd, line, strlen(line));
            if (bytes_written == -1) {
                perror("Error writing to file");
                close(transid_fd);
            }

            fsync(transid_fd);
            close(transid_fd);

            printf("check_dir_num\n");
            check_dir_num(DIR_NUM);
            struct stat statbuf;
            //创建文件夹
            char *media_path="/root/media";
            if (stat(media_path, &statbuf) == -1) {
                // 文件夹不存在，尝试创建
                if (mkdir(media_path, 0755) == -1) {
                    perror("mkdir error");
                }
                printf("文件夹 '%s' 已创建\n", media_path);
            } else if (S_ISDIR(statbuf.st_mode))
                printf("文件夹 '%s' 已存在\n", media_path);
            else
                printf("路径 '%s' 已存在，但不是文件夹\n", media_path);

            char time_path[32];
            sprintf(time_path,"%s/%s",media_path,currentTime);

            int result = mkdir(time_path, 0755);
            if (result == 0) {
                printf("Folder created successfully.\n");
            } else {
                perror("mkdir failed"); // 打印错误信息
            }
            //柜内拍照
            sprintf(in_video_open_path,"%s/in_video_open.jpg",time_path);
            sprintf(out_video_open_path,"%s/out_video_open.jpg",time_path);
            sprintf(in_video_close_path,"%s/in_video_close.jpg",time_path);
            sprintf(out_video_close_path,"%s/out_video_close.jpg",time_path);

            in_video_device.photo_flag=1;
            in_video_device.photo_opendoor=1;

            usleep(400000);

            out_video_device.preview_flag=1;

            set_post_door_open();

            out_video_device.photo_flag=1;
            out_video_device.photo_opendoor=1;

            usleep(200000);

            out_video_device.record_flag=1;

            sync();

            printf("可回收物仓门已打开，请尽快投递\n");
            for (; i >= 0; i--) {
                // 获取重量，循环更新页面
                if ((i % REFRESH_RATE) == 0) {
                    cur_weight= get_current_weight();
                    if(cur_weight < 0)
                        cur_weight=0.0;
                    diff_weight = round((cur_weight - sub_weight) * 100) / 100; //手动保留两位小数
                    if(diff_weight < 0)
                        diff_weight=0.0;
                    money = round((diff_weight * loginRes.recyclePrice) * 100) / 100; //手动保留两位小数
                    // printf("倒计时: 总重量%fKG，当前重量%fKG，计费重量%fKG，计费%f元\n", sub_weight, cur_weight, diff_weight, money);
                    printf("倒计时: %d秒 后关闭可回收物仓门\n", i/REFRESH_RATE);
                    pthread_mutex_lock(&lv_task_mutex);
                    set_screen_submit_time(i/REFRESH_RATE);
                    set_screen_submit_money(money);
                    set_screen_submit_weigth(diff_weight);
                    pthread_mutex_unlock(&lv_task_mutex);
                }

                // 获取关闭仓门按键状态
                if (close_door_flag) {
                    printf("按键：关闭仓门");
                    close_door_flag=0;
                    break;
                }

                // 获取继续投递按键状态，按下倒计时重新从99开始
                if (post_flag) {
                    printf("按键：继续投递\n");
                    i = COUNT_DOWN99 * REFRESH_RATE;
                    post_flag=0;
                }

                if (i == COUNT_DOWN20 * REFRESH_RATE) {
                    // 投递重量、金额，10s倒计时
                    pthread_mutex_lock(&lv_task_mutex);
                    lv_obj_add_flag(guider_ui.screen_submit_close_door, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(guider_ui.screen_submit_post, LV_OBJ_FLAG_HIDDEN);
                    pthread_mutex_unlock(&lv_task_mutex);
                } else if (i == COUNT_DOWN99 * REFRESH_RATE) {
                    pthread_mutex_lock(&lv_task_mutex);
                    lv_obj_add_flag(guider_ui.screen_submit_post, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(guider_ui.screen_submit_close_door, LV_OBJ_FLAG_HIDDEN);
                    pthread_mutex_unlock(&lv_task_mutex);
                }
                usleep(1000000 / REFRESH_RATE);
            }

            pthread_t close_door_thread_id;
            printf("close_door_thread\n");
            pthread_create(&close_door_thread_id, NULL, close_door_thread, NULL);
            printf("close_door\n");
            while(!is_close_door){
                cur_weight= get_current_weight();
                if(cur_weight < 0)
                    cur_weight=0.0;
                diff_weight = round((cur_weight - sub_weight) * 100) / 100; //手动保留两位小数
                if(diff_weight < 0)
                    diff_weight=0.0;
                money = round((diff_weight * loginRes.recyclePrice) * 100) / 100; //手动保留两位小数
                pthread_mutex_lock(&lv_task_mutex);
                set_screen_submit_time(i/REFRESH_RATE);
                set_screen_submit_money(money);
                set_screen_submit_weigth(diff_weight);
                pthread_mutex_unlock(&lv_task_mutex);
                usleep(1000000 / REFRESH_RATE);
            }
            is_close_door=0;
            if(close_door(cur_weight, diff_weight) == 0){
                if (unlink(filename) == -1) {
                    // 如果删除失败，打印错误信息
                    perror("Error deleting file");
                }
            }

            out_video_device.photo_opendoor=0;
            out_video_device.photo_flag=1;
            usleep(100000);

            out_video_device.preview_flag=0;
            out_video_device.record_flag=0;
            usleep(300000);

            in_video_device.photo_opendoor=0;
            in_video_device.photo_flag=1;
            
            sleep(2);

            sync();

            upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, out_video_open_path);
            upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, in_video_open_path);
            upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, out_video_close_path);
            upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, in_video_close_path);
            //upload_video("http://api.nwrecycle.com/api/device/upload/video", "/root/media/record.mp4");
            
            pthread_mutex_lock(&lv_task_mutex);
            set_screen_login(&guider_ui.screen_submit_del, loginRes.recyclePrice, cur_weight, cleantime_str, loginRes.qrCode, heartBeat.signal,server_connect);
            pthread_mutex_unlock(&lv_task_mutex);
            inside_light(0);
            openDoor.openType = 0;
        }
    } else if (openDoor.openType == 2) {
        int i = COUNT_DOWN99 * REFRESH_RATE;
        int clearing = 0;
        char in_device[64];
        char out_device[64];
        double diff_weight = 0;
        double cur_weight = get_current_weight();
        if(cur_weight < 0)
            cur_weight=0.0;

        // // 清运操作页面1
        // pthread_mutex_lock(&lv_task_mutex);
        // set_screen_clean(&guider_ui.screen_login_del,COUNT_DOWN99,sub_weight);
        // pthread_mutex_unlock(&lv_task_mutex);

        // for (; i > 0; i--) {
        //     if ((i % REFRESH_RATE) == 0) {
        //         // 更新清运界面倒计时
        //         pthread_mutex_lock(&lv_task_mutex);
        //         set_screen_clean_time(i/REFRESH_RATE);
        //         pthread_mutex_unlock(&lv_task_mutex);
        //         printf("倒计时: %d秒后返回登录界面，可清运重量%.2fKG\n", i/REFRESH_RATE, sub_weight);
        //     }
        //     // 获取开启清运门按键状态，按下开启清运门
        //     if (open_clean_door_flag) {
        //         inside_light(1);
        //         printf("按键：开启清运门\n");
        //         set_clear_door_open();
        //         open_clean_door_flag=0;
        //         clearing = 1;
        //         break;
        //     }

        //     // 获取退出按键状态，按下返回登录界面
        //     if (quit_flag) {
        //         printf("按键：退出该模式\n");
        //         quit_flag=0;
        //         break;
        //     }
        //     usleep(1000000 / REFRESH_RATE);
        // }

        check_dir_num(DIR_NUM);
        struct stat statbuf;
        //创建文件夹
        char *clean_path="/root/media/clean";
        if (stat(clean_path, &statbuf) == -1) {
            // 文件夹不存在，尝试创建
            if (mkdir(clean_path, 0755) == -1) {
                perror("mkdir error");
            }
            printf("文件夹 '%s' 已创建\n", clean_path);
        } else if (S_ISDIR(statbuf.st_mode))
            printf("文件夹 '%s' 已存在\n", clean_path);
        else
            printf("路径 '%s' 已存在，但不是文件夹\n", clean_path);

        inside_light(1);
        get_cam_video(0,in_device);
        capture_jpg_compass_osd(in_device,30,640,480,in_video_clean_open_path,0);
        get_cam_video(1,out_device);
        capture_jpg_compass_osd(out_device,30,640,480,out_video_clean_open_path,0);
        set_clear_door_open();
        clearing = 1;

        // 判断是否进入清运状态，显示正在清运
        if (clearing == 1) {
            i = 0;

            // 清运操作页面2
            pthread_mutex_lock(&lv_task_mutex);
            set_screen_cleaning(&guider_ui.screen_login_del,sub_weight);
            pthread_mutex_unlock(&lv_task_mutex);

            while (1) {
                if ((i % REFRESH_RATE) == 0) {
                    cur_weight = get_current_weight();
                    if(cur_weight < 0)
                        cur_weight=0.0;
                    diff_weight = round((sub_weight - cur_weight) * 100) / 100; //手动保留两位小数
                    if (diff_weight < 0){
                        diff_weight = 0.00;
                    }
                    // 清运操作页面2 更新当前重量
                    pthread_mutex_lock(&lv_task_mutex);
                    set_screen_cleaning_weigth(cur_weight);
                    pthread_mutex_unlock(&lv_task_mutex);
                    printf("清运中: 当前重量%.2fKG\n", cur_weight);
                }

                // 检测清理门关闭状态
                if (get_clear_door_status() == 0) {
                    if(settlement){
                        close_door(cur_weight, diff_weight); // 发送关门信息
                        hand_close_door_flag=0;
                        clearing = 2;
                        settlement=0;
                        break;
                    }
                }
                usleep(1000000 / REFRESH_RATE);
                i++;
            }
        }

        // 判断进入清运完成状态
        if (clearing == 2) {
            capture_jpg_compass_osd(in_device,10,640,480,in_video_clean_close_path,0);
            capture_jpg_compass_osd(out_device,10,640,480,out_video_clean_close_path,0);
            inside_light(0);
            // 更新清运完成页面
            pthread_mutex_lock(&lv_task_mutex);
            set_screen_cleaned(&guider_ui.screen_login_del, COUNT_DOWN10, sub_weight, cur_weight, diff_weight);
            pthread_mutex_unlock(&lv_task_mutex);

            upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, out_video_clean_open_path);
            upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, in_video_clean_open_path);
            upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, out_video_clean_close_path);
            upload_photo("http://api.nwrecycle.com/api/device/upload/photo", 1, in_video_clean_close_path);
            
            printf("清运前重量%.2fKG,清运后重量%.2fKG,清运重量%.2fKG\n", sub_weight, cur_weight, diff_weight);
            for (i = COUNT_DOWN10; i > 0; i--) {
                printf("清运完成: %d秒后返回登录界面\n", i);
                pthread_mutex_lock(&lv_task_mutex);
                set_screen_cleaned_time(i);
                pthread_mutex_unlock(&lv_task_mutex);
                sleep(1);
            }
        }

        pthread_mutex_lock(&lv_task_mutex);
        set_screen_login(&guider_ui.screen_cleaned_del, loginRes.recyclePrice, cur_weight, cleantime_str, loginRes.qrCode, heartBeat.signal,server_connect);
        // 关门,发关门消息
        pthread_mutex_unlock(&lv_task_mutex);
        set_com_weight();
        openDoor.openType = 0;
    }
    opendoor_thread_id=NULL;
    return;
}

int close_door(double cur_weight, double diff_weight) 
{
    char recv_buf[MAX_BUF_SIZE];
    int ret=0;

    // 创建 JSON 请求
    closeDoor.curWeight = cur_weight;
    closeDoor.changeWeight = diff_weight;

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "closeDoor");
    cJSON_AddStringToObject(root, "transId", trans_Id);
    cJSON_AddNumberToObject(root, "curWeight", closeDoor.curWeight);
    cJSON_AddNumberToObject(root, "changeWeight", closeDoor.changeWeight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送关门请求
    printf("Sending closeDoor request: %s\n", json_str);
    if(send_tcp_packet(json_str, strlen(json_str)) < 0)
        ret=-1;
    
    free(json_str);
    cJSON_Delete(root);

    return ret;
}

// 上传超重
int upload_overweight(double curWeight) {
    char recv_buf[MAX_BUF_SIZE];

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "overweight");
    cJSON_AddNumberToObject(root, "curWeight", curWeight);
    char *json_str = cJSON_PrintUnformatted(root);

    // 发送超重上传请求
    printf("Sending overweight request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    return 0;
}

// 上传满载
int upload_overflow(int curCapacity) {
    char recv_buf[MAX_BUF_SIZE];

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "overflow");
    cJSON_AddNumberToObject(root, "curCapacity", curCapacity);
    char *json_str = cJSON_PrintUnformatted(root);


    // 发送满载上传请求
    printf("Sending overflow request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    return 0;
}

// 上传异常
int upload_fault(FaultType type) {
    char recv_buf[MAX_BUF_SIZE];
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[15];  // yyyyMMddhhmmss

    // 获取当前时间并格式化为字符串
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, sizeof(time_str), "%Y%m%d%H%M%S", timeinfo);

    // 创建 JSON 请求
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "fault");
    cJSON_AddStringToObject(root, "time", time_str);

    // 将type参数转化为字符串
    char type_str[2];
    snprintf(type_str, sizeof(type_str), "%d", type);
    cJSON_AddStringToObject(root, "type", type_str);

    char *json_str = cJSON_PrintUnformatted(root);

    // 发送满载上传请求
    printf("Sending fault request: %s\n", json_str);
    send_tcp_packet(json_str, strlen(json_str));
    free(json_str);
    cJSON_Delete(root);

    return 0;
}

// 回调函数来捕获返回的数据
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *recv_buf = (char *)userp;
    if (recv_buf != NULL) {
        strncat(recv_buf, contents, realsize);
    }
    return realsize;
}

// 上传文件的通用函数
int upload_file(const char *url, const char *file_path, const char *file_type, int photoType) {
    int res_code;
    char res_msg[256];

    if (!url || !sn || !moduleInfo.iccid || !trans_Id || !file_path || !file_type) {
        fprintf(stderr, "Invalid input parameters\n");
        return -1;
    }

    CURL *curl;
    CURLcode res;
    curl_mime *form = NULL;
    curl_mimepart *field = NULL;
    char recv_buf[256] = {0};

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        printf("Initializing curl...\n");

        form = curl_mime_init(curl);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "sn");
        curl_mime_data(field, sn, CURL_ZERO_TERMINATED);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "iccid");
        curl_mime_data(field, moduleInfo.iccid, CURL_ZERO_TERMINATED);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "transId");
        curl_mime_data(field, trans_Id, CURL_ZERO_TERMINATED);
        printf("Added form part: transId = %s\n", trans_Id);

        if (strcmp(file_type, "photo") == 0) {
            field = curl_mime_addpart(form);
            curl_mime_name(field, "photoType");
            curl_mime_data(field, (photoType == 1 ? "1" : "0"), CURL_ZERO_TERMINATED); // 1: 开门照片, 0: 其他照片
        }

        field = curl_mime_addpart(form);
        curl_mime_name(field, "file");
        curl_mime_filedata(field, file_path);
        printf("Added form part: file = %s\n", file_path);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        printf("Set URL: %s\n", url);

        // 捕获返回数据
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)recv_buf);
        printf("Set write function and buffer\n");

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_mime_free(form);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return -1;
        } else {
            printf("File uploaded successfully. Server response: %s\n", recv_buf);

            // 使用 cJSON 解析返回消息
            cJSON *json = cJSON_Parse(recv_buf);
            if (json == NULL) {
                fprintf(stderr, "JSON parse error\n");
                curl_mime_free(form);
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return -1;
            }
            cJSON *code = cJSON_GetObjectItem(json, "code");
            cJSON *msg = cJSON_GetObjectItem(json, "msg");
            if (code && cJSON_IsNumber(code) && msg && cJSON_IsString(msg)) {
                res_code = code->valueint;
                strncpy(res_msg, msg->valuestring, sizeof(res_msg) - 1);
                res_msg[sizeof(res_msg) - 1] = '\0';
                printf("Parsed response: code = %d, msg = %s\n", res_code, res_msg);
            } else {
                fprintf(stderr, "Invalid JSON response\n");
                cJSON_Delete(json);
                curl_mime_free(form);
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return -1;
            }
            cJSON_Delete(json);
        }

        curl_mime_free(form);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}

// 上传照片
int upload_photo(const char *url, int photoType, const char *file_path) {
    return upload_file(url, file_path, "photo", photoType);
}

// 上传视频
int upload_video(const char *url, const char *file_path) {
    return upload_file(url, file_path, "video", 0);
}

int read_info() {
    int ret;
    // 默认参数
    strcpy(sn, "00016000000001");
    strcpy(key, "device_key");
    g_sRecycle_Box_Cnf.self_adc_var = 83819;
    g_sRecycle_Box_Cnf.rat = 0.0064;

    ret = read_device_info();
    if (ret != 0){
        // {"sn":"00016000000002","key":"device_key"}
        printf("配置文件参数设置失败\n");
        // LVGL提示 未设置SN和Key
    }

    FILE *file = fopen("/userdata/cleantime.txt", "r");

    if (file == NULL) {
        // 文件不存在
        perror("无法打开文件");
        snprintf(cleantime_str, sizeof(cleantime_str), "未清运");
    } else {
        if (fgets(cleantime_str, sizeof(cleantime_str), file) == NULL) {
            // 文件为空或读取失败
            perror("文件为空或读取失败");
            snprintf(cleantime_str, sizeof(cleantime_str), "未清运");
        } else {
            // 读取成功，移除换行符
            cleantime_str[strcspn(cleantime_str, "\n")] = 0;
        }
        fclose(file);
    }

    // 打印读取的清运时间
    printf("读取的清运时间: %s\n", cleantime_str);
}

void* socket_task_thread(void* arg) 
{
    int ret;

    // 获取登录配置
    while (g_main_run_) 
    {
        // 获取登录配置
        ret = get_login_config(&loginConfig);
        if (ret == 0) 
        {
            printf("Login configuration:\n");
            printf("IP: %s\n", loginConfig.ip);
            printf("TCP Port: %d\n", loginConfig.tcpPort);
            
            break; // 成功获取配置后退出循环
        } 
        else 
        {
            printf("Failed to get login configuration. Retrying in 60 seconds...\n");
            sleep(60); // 获取失败时，等待60秒后重试
        }
    }

    while (g_main_run_) 
    {
        if (0 == server_connect) 
        {
            printf("network disconnect,try to connect...\n");
            struct sockaddr_in server_addr;
            // 创建套接字
            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                perror("Socket creation failed");
                return NULL;
            }
                
            // 设置服务器地址
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(loginConfig.tcpPort);
            server_addr.sin_addr.s_addr = inet_addr(loginConfig.ip);

            // 连接到服务器 
            if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
            {
                perror("Connection to server failed");
                
                close(sock);
                sleep(60);
                
                continue;
            }

            struct timeval tv;
            // 设置接收超时时间为5秒
            tv.tv_sec = 5;  // 秒
            tv.tv_usec = 0; // 微秒

            if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) < 0) {
                perror("setsockopt failed");
                exit(EXIT_FAILURE);
            }
            
            // 登录服务器
            while (g_main_run_) 
            {
                ret = server_login();
                if (ret == 0) 
                {
                    printf("Login successful:\n");
                    printf("QR Code: %s\n", loginRes.qrCode);
                    printf("Recycle Price: %.2f\n", loginRes.recyclePrice);
                    printf("Heartbeat Interval: %d seconds\n", loginRes.heartBeatInterval);
                    server_connect = 1;           
                    break;
                }
                else 
                {
                    printf("Failed to get login configuration. Retrying in 60 seconds...\n");
                    sleep(60); // 获取失败时，等待60秒后重试
                }
            }
        }

        if (NULL == heartbeat_thread_id) 
        {
            // 创建心跳线程
            if (pthread_create(&heartbeat_thread_id, NULL, heartbeat_thread, NULL) != 0) 
            {
                perror("Failed to create heartbeat thread");

                return NULL;
            }
        }
        usleep(50*1000);
    }
    
    return NULL;
}

