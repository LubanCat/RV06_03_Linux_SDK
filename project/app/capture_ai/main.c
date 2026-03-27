/*
 * capture_ai - 摄像头捕获 + AI人脸检测 + 显示
 * 整合VI/VPSS/RKNN实现实时人脸检测显示
 * 参考ai/yolov5face实现
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <math.h>
#include <stdint.h>

#include "rk_mpi_vi.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_vpss.h"
#include "rk_comm_vi.h"
#include "rk_comm_vpss.h"
#include "rk_common.h"

/* AI相关头文件 */
#include "rknn_api.h"

/* 摄像头参数 */
#define CAMERA_WIDTH    1632
#define CAMERA_HEIGHT   1224

/* 显示参数 */
#define DISPLAY_WIDTH   640
#define DISPLAY_HEIGHT  480

/* AI模型输入参数 - yolov5n-face 640x640 */
#define MODEL_WIDTH     640
#define MODEL_HEIGHT    640

/* 裁剪区域 - 从摄像头中心裁剪正方形 */
#define CROP_SIZE       960
#define CROP_X          ((CAMERA_WIDTH - CROP_SIZE) / 2)
#define CROP_Y          ((CAMERA_HEIGHT - CROP_SIZE) / 2)

/* 显示区域在960x960中的位置 */
#define DISP_OFFSET_X   ((CROP_SIZE - DISPLAY_WIDTH) / 2)
#define DISP_OFFSET_Y   ((CROP_SIZE - DISPLAY_HEIGHT) / 2)

/* VI参数 */
#define VI_DEV_ID       0
#define VI_PIPE_ID      0
#define VI_CHN_ID       0

/* VPSS参数 - 双通道 */
#define VPSS_GRP_ID     0
#define VPSS_CHN_DISP   0   /* 显示通道 BGRA8888 */
#define VPSS_CHN_AI     1   /* AI模型输入通道 RGB888 */

/* NPU相关 */
#define PROP_BOX_SIZE   16  /* 4(bbox) + 1(obj) + 10(landmarks) + 1(cls) = 16 */
#define NMS_THRESH      0.5
#define BOX_THRESH      0.5

/* anchor for yolov5n-face 320x320 */
static const int anchor[3][6] = {
    {4,5,  8,10,  13,16},
    {23,29,  43,55,  73,105},
    {146,217,  231,300,  335,433}
};

static volatile int g_running = 1;
static int g_skip_frames = 1;  /* 跳帧数：0=每帧做AI, 1=隔帧, 2=跳2帧... */

/* 人脸检测结果 */
typedef struct {
    float x1, y1, x2, y2;
    float prop;
    float points[5][2];  /* 5个关键点 */
} face_result_t;

typedef struct {
    int count;
    face_result_t faces[64];
} face_result_list_t;

static void signal_handler(int sig) {
    g_running = 0;
    printf("\nReceived signal %d, exiting...\n", sig);
}

/* Framebuffer */
typedef struct {
    int fd;
    unsigned char *fb_mem;
    unsigned int fb_size;
    unsigned int stride;
    unsigned int bpp;
} fb_info_t;

static int fb_init(fb_info_t *fb, const char *dev) {
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    memset(fb, 0, sizeof(fb_info_t));
    fb->fd = open(dev, O_RDWR);
    if (fb->fd < 0) {
        perror("open framebuffer failed");
        return -1;
    }

    ioctl(fb->fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb->fd, FBIOGET_FSCREENINFO, &finfo);

    fb->stride = finfo.line_length;
    fb->fb_size = finfo.smem_len;
    fb->bpp = vinfo.bits_per_pixel;

    fb->fb_mem = mmap(NULL, fb->fb_size, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fb->fd, 0);
    if (fb->fb_mem == MAP_FAILED) {
        close(fb->fd);
        return -1;
    }

    printf("Framebuffer: %dx%d, %d bpp\n",
           (int)vinfo.xres, (int)vinfo.yres, fb->bpp);
    return 0;
}

static void fb_deinit(fb_info_t *fb) {
    if (fb->fb_mem && fb->fb_mem != MAP_FAILED)
        munmap(fb->fb_mem, fb->fb_size);
    if (fb->fd >= 0) close(fb->fd);
}

/* VI初始化 */
static int vi_init(void) {
    RK_S32 ret;
    VI_DEV_ATTR_S dev_attr;
    VI_DEV_BIND_PIPE_S bind_pipe;
    VI_CHN_ATTR_S chn_attr;
    VI_DEV_STATUS_S dev_status;

    ret = RK_MPI_VI_QueryDevStatus(VI_DEV_ID, &dev_status);
    if (ret == RK_SUCCESS && !dev_status.bProbeOk) {
        printf("Sensor probe failed\n");
        return -1;
    }

    ret = RK_MPI_VI_GetDevAttr(VI_DEV_ID, &dev_attr);
    if (ret == RK_ERR_VI_NOT_CONFIG) {
        memset(&dev_attr, 0, sizeof(dev_attr));
        dev_attr.stMaxSize.u32Width = CAMERA_WIDTH;
        dev_attr.stMaxSize.u32Height = CAMERA_HEIGHT;
        RK_MPI_VI_SetDevAttr(VI_DEV_ID, &dev_attr);
    }

    ret = RK_MPI_VI_GetDevIsEnable(VI_DEV_ID);
    if (ret != RK_SUCCESS) {
        RK_MPI_VI_EnableDev(VI_DEV_ID);
        memset(&bind_pipe, 0, sizeof(bind_pipe));
        bind_pipe.u32Num = 1;
        bind_pipe.PipeId[0] = VI_PIPE_ID;
        RK_MPI_VI_SetDevBindPipe(VI_DEV_ID, &bind_pipe);
    }

    memset(&chn_attr, 0, sizeof(chn_attr));
    chn_attr.stSize.u32Width = CAMERA_WIDTH;
    chn_attr.stSize.u32Height = CAMERA_HEIGHT;
    chn_attr.enPixelFormat = RK_FMT_YUV420SP;
    chn_attr.u32Depth = 2;
    chn_attr.stFrameRate.s32SrcFrameRate = -1;
    chn_attr.stFrameRate.s32DstFrameRate = -1;
    chn_attr.stIspOpt.u32BufCount = 3;
    chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
    chn_attr.stIspOpt.bNoUseLibV4L2 = RK_TRUE;
    chn_attr.stIspOpt.stMaxSize.u32Width = CAMERA_WIDTH;
    chn_attr.stIspOpt.stMaxSize.u32Height = CAMERA_HEIGHT;

    RK_MPI_VI_SetChnAttr(VI_PIPE_ID, VI_CHN_ID, &chn_attr);
    RK_MPI_VI_EnableChn(VI_PIPE_ID, VI_CHN_ID);

    printf("VI initialized: %dx%d\n", CAMERA_WIDTH, CAMERA_HEIGHT);
    return 0;
}

static void vi_deinit(void) {
    RK_MPI_VI_DisableChn(VI_PIPE_ID, VI_CHN_ID);
    RK_MPI_VI_DisableDev(VI_DEV_ID);
}

/* VPSS初始化 - 双通道输出 */
static int vpss_init(void) {
    RK_S32 ret;
    VPSS_GRP_ATTR_S grp_attr;
    VPSS_CHN_ATTR_S chn_attr;
    VPSS_CROP_INFO_S crop_info;

    memset(&grp_attr, 0, sizeof(grp_attr));
    grp_attr.u32MaxW = CAMERA_WIDTH;
    grp_attr.u32MaxH = CAMERA_HEIGHT;
    grp_attr.enPixelFormat = RK_FMT_YUV420SP;
    grp_attr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    grp_attr.stFrameRate.s32SrcFrameRate = -1;
    grp_attr.stFrameRate.s32DstFrameRate = -1;

    ret = RK_MPI_VPSS_CreateGrp(VPSS_GRP_ID, &grp_attr);
    if (ret != RK_SUCCESS) {
        printf("VPSS_CreateGrp failed: 0x%x\n", ret);
        return -1;
    }

    /* 设置Group裁剪 - 从中心裁剪960x960正方形 */
    memset(&crop_info, 0, sizeof(crop_info));
    crop_info.bEnable = RK_TRUE;
    crop_info.enCropCoordinate = VPSS_CROP_ABS_COOR;
    crop_info.stCropRect.s32X = CROP_X;
    crop_info.stCropRect.s32Y = CROP_Y;
    crop_info.stCropRect.u32Width = CROP_SIZE;
    crop_info.stCropRect.u32Height = CROP_SIZE;
    RK_MPI_VPSS_SetGrpCrop(VPSS_GRP_ID, &crop_info);
    printf("VPSS crop: (%d,%d) %dx%d\n", CROP_X, CROP_Y, CROP_SIZE, CROP_SIZE);

    /* CHN0 - 显示输出 640x640 BGRA8888 */
    memset(&chn_attr, 0, sizeof(chn_attr));
    chn_attr.enChnMode = VPSS_CHN_MODE_USER;
    chn_attr.u32Width = MODEL_WIDTH;
    chn_attr.u32Height = MODEL_HEIGHT;
    chn_attr.enVideoFormat = VIDEO_FORMAT_LINEAR;
    chn_attr.enPixelFormat = RK_FMT_BGRA8888;
    chn_attr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    chn_attr.u32Depth = 2;
    chn_attr.u32FrameBufCnt = 3;
    RK_MPI_VPSS_SetChnAttr(VPSS_GRP_ID, VPSS_CHN_DISP, &chn_attr);
    RK_MPI_VPSS_EnableChn(VPSS_GRP_ID, VPSS_CHN_DISP);

    /* CHN1 - AI输入 640x640 RGB888 */
    memset(&chn_attr, 0, sizeof(chn_attr));
    chn_attr.enChnMode = VPSS_CHN_MODE_USER;
    chn_attr.u32Width = MODEL_WIDTH;
    chn_attr.u32Height = MODEL_HEIGHT;
    chn_attr.enVideoFormat = VIDEO_FORMAT_LINEAR;
    chn_attr.enPixelFormat = RK_FMT_RGB888;
    chn_attr.enDynamicRange = DYNAMIC_RANGE_SDR8;
    chn_attr.u32Depth = 2;
    chn_attr.u32FrameBufCnt = 3;
    RK_MPI_VPSS_SetChnAttr(VPSS_GRP_ID, VPSS_CHN_AI, &chn_attr);
    RK_MPI_VPSS_EnableChn(VPSS_GRP_ID, VPSS_CHN_AI);

    RK_MPI_VPSS_EnableBackupFrame(VPSS_GRP_ID);
    RK_MPI_VPSS_StartGrp(VPSS_GRP_ID);

    printf("VPSS: CHN0 %dx%d BGRA for display, CHN1 %dx%d RGB for AI\n",
           MODEL_WIDTH, MODEL_HEIGHT, MODEL_WIDTH, MODEL_HEIGHT);
    return 0;
}

static void vpss_deinit(void) {
    RK_MPI_VPSS_StopGrp(VPSS_GRP_ID);
    RK_MPI_VPSS_DisableBackupFrame(VPSS_GRP_ID);
    RK_MPI_VPSS_DisableChn(VPSS_GRP_ID, VPSS_CHN_AI);
    RK_MPI_VPSS_DisableChn(VPSS_GRP_ID, VPSS_CHN_DISP);
    RK_MPI_VPSS_DestroyGrp(VPSS_GRP_ID);
}

/* RKNN上下文 */
typedef struct {
    rknn_context ctx;
    int input_width;
    int input_height;
    int input_channel;
    int n_output;
    rknn_tensor_attr *input_attrs;
    rknn_tensor_attr *output_attrs;
    rknn_tensor_mem *input_mems[1];
    rknn_tensor_mem *output_mems[3];
    int is_quant;
} rknn_context_t;

static rknn_context_t g_rknn_ctx;

/* 辅助函数 */
static float sigmoid(float x) { return 1.0f / (1.0f + expf(-x)); }

static float deqnt_affine_to_f32(int8_t qnt, int32_t zp, float scale) {
    return ((float)qnt - (float)zp) * scale;
}

static float CalculateOverlap(float xmin0, float ymin0, float xmax0, float ymax0,
                              float xmin1, float ymin1, float xmax1, float ymax1) {
    float w = fmaxf(0.f, fminf(xmax0, xmax1) - fmaxf(xmin0, xmin1) + 1.0f);
    float h = fmaxf(0.f, fminf(ymax0, ymax1) - fmaxf(ymin0, ymin1) + 1.0f);
    float i = w * h;
    float u = (xmax0 - xmin0 + 1.0f) * (ymax0 - ymin0 + 1.0f) +
              (xmax1 - xmin1 + 1.0f) * (ymax1 - ymin1 + 1.0f) - i;
    return u <= 0.f ? 0.f : (i / u);
}

/* 初始化RKNN模型 */
static int init_rknn_model(const char *model_path) {
    rknn_context_t *ctx = &g_rknn_ctx;
    rknn_input_output_num io_num;
    int ret;

    memset(ctx, 0, sizeof(rknn_context_t));

    /* 加载模型 */
    ret = rknn_init(&ctx->ctx, (void *)model_path, 0, 0, NULL);
    if (ret < 0) {
        printf("rknn_init failed: %d\n", ret);
        return -1;
    }

    rknn_query(ctx->ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
    printf("Model: input=%d, output=%d\n", io_num.n_input, io_num.n_output);

    ctx->n_output = io_num.n_output;

    /* 获取输入属性 - 使用NATIVE查询 */
    ctx->input_attrs = malloc(sizeof(rknn_tensor_attr) * io_num.n_input);
    memset(ctx->input_attrs, 0, sizeof(rknn_tensor_attr) * io_num.n_input);
    for (int i = 0; i < io_num.n_input; i++) {
        ctx->input_attrs[i].index = i;
        ret = rknn_query(ctx->ctx, RKNN_QUERY_NATIVE_INPUT_ATTR,
                        &ctx->input_attrs[i], sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC) {
            printf("rknn_query input failed: %d\n", ret);
            return -1;
        }
        printf("Input[%d]: n_dims=%d, dims=[%d,%d,%d,%d], size=%d\n",
               i, ctx->input_attrs[i].n_dims,
               ctx->input_attrs[i].dims[0], ctx->input_attrs[i].dims[1],
               ctx->input_attrs[i].dims[2], ctx->input_attrs[i].dims[3],
               ctx->input_attrs[i].size_with_stride);
    }

    /* 设置输入格式 - NHWC, UINT8 */
    ctx->input_attrs[0].type = RKNN_TENSOR_UINT8;
    ctx->input_attrs[0].fmt = RKNN_TENSOR_NHWC;

    /* NHWC格式: dims[0]=1, dims[1]=H, dims[2]=W, dims[3]=C */
    ctx->input_height = ctx->input_attrs[0].dims[1];
    ctx->input_width = ctx->input_attrs[0].dims[2];
    ctx->input_channel = ctx->input_attrs[0].dims[3];
    printf("Model input: %dx%dx%d (NHWC)\n",
           ctx->input_width, ctx->input_height, ctx->input_channel);

    /* 获取输出属性 - 使用NATIVE_NHWC查询 */
    ctx->output_attrs = malloc(sizeof(rknn_tensor_attr) * io_num.n_output);
    memset(ctx->output_attrs, 0, sizeof(rknn_tensor_attr) * io_num.n_output);
    for (int i = 0; i < io_num.n_output; i++) {
        ctx->output_attrs[i].index = i;
        ret = rknn_query(ctx->ctx, RKNN_QUERY_NATIVE_NHWC_OUTPUT_ATTR,
                        &ctx->output_attrs[i], sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC) {
            printf("rknn_query output failed: %d\n", ret);
            return -1;
        }
        printf("Output[%d]: n_dims=%d, dims=[%d,%d,%d,%d], zp=%d, scale=%f\n",
               i, ctx->output_attrs[i].n_dims,
               ctx->output_attrs[i].dims[0], ctx->output_attrs[i].dims[1],
               ctx->output_attrs[i].dims[2], ctx->output_attrs[i].dims[3],
               ctx->output_attrs[i].zp, ctx->output_attrs[i].scale);
    }

    /* 创建输入tensor内存 */
    ctx->input_mems[0] = rknn_create_mem(ctx->ctx, ctx->input_attrs[0].size_with_stride);
    ret = rknn_set_io_mem(ctx->ctx, ctx->input_mems[0], &ctx->input_attrs[0]);
    if (ret < 0) {
        printf("rknn_set_io_mem input failed: %d\n", ret);
        return -1;
    }

    /* 创建输出tensor内存 */
    for (int i = 0; i < io_num.n_output; i++) {
        ctx->output_mems[i] = rknn_create_mem(ctx->ctx, ctx->output_attrs[i].size_with_stride);
        ret = rknn_set_io_mem(ctx->ctx, ctx->output_mems[i], &ctx->output_attrs[i]);
        if (ret < 0) {
            printf("rknn_set_io_mem output[%d] failed: %d\n", i, ret);
            return -1;
        }
    }

    /* 检查是否量化模型 */
    if (ctx->output_attrs[0].qnt_type == RKNN_TENSOR_QNT_AFFINE_ASYMMETRIC) {
        ctx->is_quant = 1;
    } else {
        ctx->is_quant = 0;
        printf("Warning: RV1106 only supports quantized models\n");
    }

    printf("RKNN model loaded: %s\n", model_path);
    return 0;
}

static void rknn_deinit(void) {
    rknn_context_t *ctx = &g_rknn_ctx;

    if (ctx->input_mems[0])
        rknn_destroy_mem(ctx->ctx, ctx->input_mems[0]);

    for (int i = 0; i < 3; i++) {
        if (ctx->output_mems[i])
            rknn_destroy_mem(ctx->ctx, ctx->output_mems[i]);
    }

    if (ctx->input_attrs) free(ctx->input_attrs);
    if (ctx->output_attrs) free(ctx->output_attrs);

    rknn_destroy(ctx->ctx);
}

/* 处理单个输出层 */
static int process_output_layer(int8_t *input, const int *anchor, int grid_h, int grid_w,
                                int height, int width, int stride, float threshold,
                                int32_t zp, float scale,
                                float *boxes, float *objProbs, float *landmarks, int *validCount,
                                int max_faces) {
    int count = 0;
    int grid_len = grid_h * grid_w;
    int tensor_len = PROP_BOX_SIZE * 3;

    for (int a = 0; a < 3 && count < max_faces; a++) {
        for (int i = 0; i < grid_h && count < max_faces; i++) {
            for (int j = 0; j < grid_w && count < max_faces; j++) {
                int base_idx = tensor_len * (i * grid_w + j) + PROP_BOX_SIZE * a;
                float obj_conf = sigmoid(deqnt_affine_to_f32(input[base_idx + 4], zp, scale));

                if (obj_conf < threshold) continue;

                float cls_conf = sigmoid(deqnt_affine_to_f32(input[base_idx + 15], zp, scale));
                if (cls_conf >= threshold) {
                    /* 解码边界框 */
                    float box_x = sigmoid(deqnt_affine_to_f32(input[base_idx + 0], zp, scale)) * 2.0f - 0.5f;
                    float box_y = sigmoid(deqnt_affine_to_f32(input[base_idx + 1], zp, scale)) * 2.0f - 0.5f;
                    float box_w = sigmoid(deqnt_affine_to_f32(input[base_idx + 2], zp, scale)) * 2.0f;
                    float box_h = sigmoid(deqnt_affine_to_f32(input[base_idx + 3], zp, scale)) * 2.0f;

                    box_x = (box_x + j) * (float)stride;
                    box_y = (box_y + i) * (float)stride;
                    box_w = box_w * box_w * (float)anchor[a * 2];
                    box_h = box_h * box_h * (float)anchor[a * 2 + 1];
                    box_x -= (box_w / 2.0f);
                    box_y -= (box_h / 2.0f);

                    boxes[count * 4 + 0] = box_x;
                    boxes[count * 4 + 1] = box_y;
                    boxes[count * 4 + 2] = box_w;
                    boxes[count * 4 + 3] = box_h;

                    /* 解码关键点 */
                    for (int k = 0; k < 5; k++) {
                        int lm_idx = base_idx + 5 + k * 2;
                        landmarks[count * 10 + k * 2 + 0] =
                            deqnt_affine_to_f32(input[lm_idx], zp, scale) * (float)anchor[a * 2] + j * (float)stride;
                        landmarks[count * 10 + k * 2 + 1] =
                            deqnt_affine_to_f32(input[lm_idx + 1], zp, scale) * (float)anchor[a * 2 + 1] + i * (float)stride;
                    }

                    objProbs[count] = obj_conf * cls_conf;
                    count++;
                }
            }
        }
    }

    *validCount = count;
    return count;
}

/* NMS */
static void nms_sorted(float *boxes, float *objProbs, int *order, int n, float threshold) {
    for (int i = 0; i < n; i++) {
        if (order[i] == -1) continue;

        for (int j = i + 1; j < n; j++) {
            if (order[j] == -1) continue;

            int a = order[i], b = order[j];
            float x1 = fmaxf(boxes[a*4+0], boxes[b*4+0]);
            float y1 = fmaxf(boxes[a*4+1], boxes[b*4+1]);
            float x2 = fminf(boxes[a*4+0]+boxes[a*4+2], boxes[b*4+0]+boxes[b*4+2]);
            float y2 = fminf(boxes[a*4+1]+boxes[a*4+3], boxes[b*4+1]+boxes[b*4+3]);

            float inter = fmaxf(0, x2-x1) * fmaxf(0, y2-y1);
            float area_a = boxes[a*4+2] * boxes[a*4+3];
            float area_b = boxes[b*4+2] * boxes[b*4+3];
            float iou = inter / (area_a + area_b - inter);

            if (iou > threshold) {
                order[j] = -1;
            }
        }
    }
}

/* RKNN推理和后处理 */
static int rknn_inference(void *input_data, int input_size, face_result_list_t *results) {
    rknn_context_t *ctx = &g_rknn_ctx;
    int ret;

    results->count = 0;

    /* 复制输入数据到tensor内存 */
    if (input_size > ctx->input_attrs[0].size_with_stride) {
        printf("Input size too large: %d > %d\n", input_size, ctx->input_attrs[0].size_with_stride);
        return -1;
    }
    memcpy(ctx->input_mems[0]->virt_addr, input_data, input_size);

    /* 运行推理 */
    ret = rknn_run(ctx->ctx, NULL);
    if (ret != RKNN_SUCC) {
        printf("rknn_run failed: %d\n", ret);
        return -1;
    }

    /* 后处理 */
    float *all_boxes = malloc(sizeof(float) * 1000 * 4);
    float *all_probs = malloc(sizeof(float) * 1000);
    float *all_landmarks = malloc(sizeof(float) * 1000 * 10);
    int total_count = 0;

    /* 处理3个输出层 */
    for (int i = 0; i < 3; i++) {
        int grid_h = ctx->output_attrs[i].dims[1];
        int grid_w = ctx->output_attrs[i].dims[2];
        int stride = MODEL_HEIGHT / grid_h;

        int layer_count = 0;
        process_output_layer(
            (int8_t *)ctx->output_mems[i]->virt_addr,
            anchor[i], grid_h, grid_w,
            MODEL_HEIGHT, MODEL_WIDTH, stride,
            BOX_THRESH,
            ctx->output_attrs[i].zp, ctx->output_attrs[i].scale,
            all_boxes + total_count * 4,
            all_probs + total_count,
            all_landmarks + total_count * 10,
            &layer_count,
            1000 - total_count
        );
        total_count += layer_count;
    }

    if (total_count == 0) {
        free(all_boxes);
        free(all_probs);
        free(all_landmarks);
        return 0;
    }

    /* 按置信度排序 */
    int *order = malloc(sizeof(int) * total_count);
    for (int i = 0; i < total_count; i++) order[i] = i;
    for (int i = 0; i < total_count - 1; i++) {
        for (int j = i + 1; j < total_count; j++) {
            if (all_probs[order[i]] < all_probs[order[j]]) {
                int tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }
        }
    }

    /* NMS */
    nms_sorted(all_boxes, all_probs, order, total_count, NMS_THRESH);

    /* 收集结果 */
    for (int i = 0; i < total_count && results->count < 64; i++) {
        int idx = order[i];
        if (idx == -1) continue;

        face_result_t *face = &results->faces[results->count];
        face->x1 = all_boxes[idx * 4 + 0];
        face->y1 = all_boxes[idx * 4 + 1];
        face->x2 = all_boxes[idx * 4 + 0] + all_boxes[idx * 4 + 2];
        face->y2 = all_boxes[idx * 4 + 1] + all_boxes[idx * 4 + 3];
        face->prop = all_probs[idx];

        for (int k = 0; k < 5; k++) {
            face->points[k][0] = all_landmarks[idx * 10 + k * 2 + 0];
            face->points[k][1] = all_landmarks[idx * 10 + k * 2 + 1];
        }

        results->count++;
    }

    free(order);
    free(all_boxes);
    free(all_probs);
    free(all_landmarks);

    return 0;
}

/* 绘制人脸框 - 坐标已在640x640上，显示区域是中心640x480 */
static void draw_face_result(unsigned char *fb_mem, int fb_stride,
                             face_result_list_t *results) {
    /* 显示区域在640x640中的Y范围: 80到560 */
    int disp_y_start = (MODEL_HEIGHT - DISPLAY_HEIGHT) / 2;  // 80
    int disp_y_end = disp_y_start + DISPLAY_HEIGHT;          // 560

    for (int i = 0; i < results->count; i++) {
        face_result_t *face = &results->faces[i];

        /* AI坐标 -> 显示坐标 (Y方向减去偏移) */
        int x1 = (int)face->x1;
        int y1 = (int)face->y1 - disp_y_start;
        int x2 = (int)face->x2;
        int y2 = (int)face->y2 - disp_y_start;

        /* 边界检查 */
        x1 = x1 < 0 ? 0 : (x1 >= DISPLAY_WIDTH ? DISPLAY_WIDTH - 1 : x1);
        y1 = y1 < 0 ? 0 : (y1 >= DISPLAY_HEIGHT ? DISPLAY_HEIGHT - 1 : y1);
        x2 = x2 < 0 ? 0 : (x2 >= DISPLAY_WIDTH ? DISPLAY_WIDTH - 1 : x2);
        y2 = y2 < 0 ? 0 : (y2 >= DISPLAY_HEIGHT ? DISPLAY_HEIGHT - 1 : y2);

        if (x1 >= x2 || y1 >= y2) continue;

        /* 画矩形框 BGRA格式 - 绿色 */
        for (int x = x1; x <= x2; x++) {
            int idx1 = (y1 * fb_stride + x * 4);
            int idx2 = (y2 * fb_stride + x * 4);
            fb_mem[idx1 + 0] = 0;    fb_mem[idx1 + 1] = 255;  fb_mem[idx1 + 2] = 0;  fb_mem[idx1 + 3] = 255;
            fb_mem[idx2 + 0] = 0;    fb_mem[idx2 + 1] = 255;  fb_mem[idx2 + 2] = 0;  fb_mem[idx2 + 3] = 255;
        }
        for (int y = y1; y <= y2; y++) {
            int idx1 = (y * fb_stride + x1 * 4);
            int idx2 = (y * fb_stride + x2 * 4);
            fb_mem[idx1 + 0] = 0;    fb_mem[idx1 + 1] = 255;  fb_mem[idx1 + 2] = 0;  fb_mem[idx1 + 3] = 255;
            fb_mem[idx2 + 0] = 0;    fb_mem[idx2 + 1] = 255;  fb_mem[idx2 + 2] = 0;  fb_mem[idx2 + 3] = 255;
        }

        /* 画关键点 - 红色 */
        for (int j = 0; j < 5; j++) {
            int px = (int)face->points[j][0];
            int py = (int)face->points[j][1] - disp_y_start;

            if (px >= 0 && px < DISPLAY_WIDTH && py >= 0 && py < DISPLAY_HEIGHT) {
                /* 画3x3的点 */
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int nx = px + dx, ny = py + dy;
                        if (nx >= 0 && nx < DISPLAY_WIDTH && ny >= 0 && ny < DISPLAY_HEIGHT) {
                            int ni = (ny * fb_stride + nx * 4);
                            fb_mem[ni + 0] = 0;    /* B */
                            fb_mem[ni + 1] = 0;    /* G */
                            fb_mem[ni + 2] = 255;  /* R */
                            fb_mem[ni + 3] = 255;  /* A */
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <model.rknn> [-t skip_frames]\n", argv[0]);
        printf("  -t N  : AI推理跳帧数 (默认1)\n");
        printf("          0 = 每帧做AI\n");
        printf("          1 = 隔帧做AI (推荐)\n");
        printf("          2 = 每3帧做1次AI\n");
        return -1;
    }

    /* 解析参数 */
    const char *model_path = argv[1];
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            g_skip_frames = atoi(argv[i + 1]);
            if (g_skip_frames < 0) g_skip_frames = 0;
            i++;
        }
    }

    printf("Model: %s, skip_frames: %d\n", model_path, g_skip_frames);

    fb_info_t fb;
    VIDEO_FRAME_INFO_S vi_frame, vpss_disp, vpss_ai;
    int frame_count = 0;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    /* 初始化系统 */
    RK_MPI_SYS_Init();

    /* 初始化Framebuffer */
    if (fb_init(&fb, "/dev/fb0") != 0) {
        RK_MPI_SYS_Exit();
        return -1;
    }

    /* 初始化VI */
    if (vi_init() != 0) {
        fb_deinit(&fb);
        RK_MPI_SYS_Exit();
        return -1;
    }

    /* 初始化VPSS */
    if (vpss_init() != 0) {
        vi_deinit();
        fb_deinit(&fb);
        RK_MPI_SYS_Exit();
        return -1;
    }

    /* 初始化RKNN */
    if (init_rknn_model(model_path) != 0) {
        vpss_deinit();
        vi_deinit();
        fb_deinit(&fb);
        RK_MPI_SYS_Exit();
        return -1;
    }

    printf("Pipeline: Camera %dx%d -> Crop %dx%d -> Model %dx%d\n",
           CAMERA_WIDTH, CAMERA_HEIGHT, CROP_SIZE, CROP_SIZE, MODEL_WIDTH, MODEL_HEIGHT);
    printf("Display: center %dx%d from %dx%d AI output\n",
           DISPLAY_WIDTH, DISPLAY_HEIGHT, MODEL_WIDTH, MODEL_HEIGHT);
    printf("Press Ctrl+C to exit\n");

    /* 显示区域在640x640中的Y偏移 */
    int disp_y_offset = (MODEL_HEIGHT - DISPLAY_HEIGHT) / 2;  // 80
    face_result_list_t results;
    memset(&results, 0, sizeof(results));

    /* 主循环 */
    while (g_running) {
        /* 1. 获取VI帧 */
        if (RK_MPI_VI_GetChnFrame(VI_PIPE_ID, VI_CHN_ID, &vi_frame, 1000) != RK_SUCCESS)
            continue;

        /* 2. 发送到VPSS */
        if (RK_MPI_VPSS_SendFrame(VPSS_GRP_ID, 0, &vi_frame, 1000) != RK_SUCCESS) {
            RK_MPI_VI_ReleaseChnFrame(VI_PIPE_ID, VI_CHN_ID, &vi_frame);
            continue;
        }

        /* 3. 跳帧处理：根据 g_skip_frames 决定是否做AI */
        /* skip=0: 每帧做AI, skip=1: 每2帧做1次, skip=2: 每3帧做1次... */
        if (g_skip_frames == 0 || (frame_count % (g_skip_frames + 1) == 0)) {
            /* 获取AI输入帧 (CHN1: 640x640 RGB888) */
            if (RK_MPI_VPSS_GetChnFrame(VPSS_GRP_ID, VPSS_CHN_AI, &vpss_ai, 1000) == RK_SUCCESS) {
                void *ai_data = RK_MPI_MB_Handle2VirAddr(vpss_ai.stVFrame.pMbBlk);
                rknn_inference(ai_data, MODEL_WIDTH * MODEL_HEIGHT * 3, &results);
                RK_MPI_VPSS_ReleaseChnFrame(VPSS_GRP_ID, VPSS_CHN_AI, &vpss_ai);
            }
        }

        /* 4. 获取显示帧 (CHN0: 640x640 BGRA8888) - 每帧都更新 */
        if (RK_MPI_VPSS_GetChnFrame(VPSS_GRP_ID, VPSS_CHN_DISP, &vpss_disp, 1000) == RK_SUCCESS) {
            void *disp_data = RK_MPI_MB_Handle2VirAddr(vpss_disp.stVFrame.pMbBlk);

            /* 从640x640中心截取640x480，直接复制BGRA到FB */
            for (int y = 0; y < DISPLAY_HEIGHT; y++) {
                memcpy(fb.fb_mem + y * fb.stride,
                       (unsigned char *)disp_data + (y + disp_y_offset) * MODEL_WIDTH * 4,
                       DISPLAY_WIDTH * 4);
            }

            /* 绘制检测结果（使用最近一次AI结果） */
            if (results.count > 0) {
                draw_face_result(fb.fb_mem, fb.stride, &results);
            }

            RK_MPI_VPSS_ReleaseChnFrame(VPSS_GRP_ID, VPSS_CHN_DISP, &vpss_disp);
        }

        RK_MPI_VI_ReleaseChnFrame(VI_PIPE_ID, VI_CHN_ID, &vi_frame);

        frame_count++;
        if (frame_count % 30 == 0) {
            printf("Frame: %d, faces: %d\n", frame_count, results.count);
        }
    }

    printf("\nExiting, %d frames processed\n", frame_count);

    rknn_deinit();
    vpss_deinit();
    vi_deinit();
    fb_deinit(&fb);
    RK_MPI_SYS_Exit();

    return 0;
}