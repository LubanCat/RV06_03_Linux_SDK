#include "nalu.h"
#include "h264_parser.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

// 用动态数组代替vector
Nalu_t *nalu_vector = NULL;
int nalu_vector_size = 0;

char sps_data[128] = {0};
int sps_data_length = 0;

char pps_data[128] = {0};
int pps_data_length = 0;

void out_nalu(char *buffer, int size, NaluType naluType) {
    // C语言没有std::cout，因此这里省略了打印十六进制的代码
    // 如果需要，可以自己实现一个binToHex函数

    if (NALU_TYPE_SPS == naluType) {
        memcpy(sps_data, buffer, size);
        sps_data_length = size;
        return;
    }

    if (NALU_TYPE_PPS == naluType) {
        memcpy(pps_data, buffer, size);
        pps_data_length = size;
        return;
    }

    Nalu_t new_Nalu;
    bool is_i_frame = (NALU_TYPE_IDR == naluType);

    char *packet = (char *)malloc(is_i_frame ? (size + sps_data_length + pps_data_length) : size);
    if (is_i_frame) {
        memcpy(packet, sps_data, sps_data_length);
        memcpy(packet + sps_data_length, pps_data, pps_data_length);
        memcpy(packet + sps_data_length + pps_data_length, buffer, size);
        size += (sps_data_length + pps_data_length);
    } else {
        memcpy(packet, buffer, size);
    }
    new_Nalu.packet = packet;
    new_Nalu.length = size;
    new_Nalu.type = naluType;

    // 重新分配内存以存储新的Nalu_t元素
    nalu_vector_size++;
    nalu_vector = (Nalu_t *)realloc(nalu_vector, nalu_vector_size * sizeof(Nalu_t));
    nalu_vector[nalu_vector_size - 1] = new_Nalu;
}

int load(const char *path) {
    // simplified_h264_parser需要接受一个函数指针作为参数
    return simplest_h264_parser(path, out_nalu);
}

// 记得释放分配的内存
void free_nalu_vector() {
    for (int i = 0; i < nalu_vector_size; i++) {
        free(nalu_vector[i].packet);
    }
    free(nalu_vector);
    nalu_vector = NULL;
    nalu_vector_size = 0;
}

// 调用load函数后，记得调用free_nalu_vector来释放内存
