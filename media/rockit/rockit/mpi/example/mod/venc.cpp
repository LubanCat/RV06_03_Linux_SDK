#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "rk_debug.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_ivs.h"
#include "rk_mpi_tde.h"
#include "rk_comm_ivs.h"
#include "rk_comm_tde.h"

#include "test_comm_argparse.h"
#include "test_comm_imgproc.h"
#include "test_comm_venc.h"
#include "test_comm_utils.h"


#define FORMATE RK_FMT_RGB888

void get_tde_size(int width ,int height,int *size){
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;

    stPicBufAttr.u32Width = width;
    stPicBufAttr.u32Height = height;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)FORMATE;
    stPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
    s32Ret = RK_MPI_CAL_TDE_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        printf("get picture buffer size failed. err 0x%x", s32Ret);
    }
    *size = stMbPicCalResult.u32MBSize;
}

int venc_resize(char* input_path,int width,int height,char *output_path ,int dst_width,int dst_height){
    RK_S32 s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S pstRecvParam;
    VENC_CHN_ATTR_S stAttr;
    void *pVirAddr = RK_NULL;
    int ret;
    VIDEO_FRAME_INFO_S stFrame;
    VENC_STREAM_S rcv;
    int buffersize;

    char *src_buf;
    //初始化mpi
    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    
    get_tde_size(width,height,&buffersize);

    printf("calc picture size: %d\n", buffersize);

    MB_BLK blk = RK_NULL;
    ret = RK_MPI_SYS_MmzAlloc(&blk, RK_NULL, RK_NULL,buffersize);
    if (RK_NULL == blk) {
        printf("RK_MPI_MB_GetMB fail\n");
        usleep(2000llu);
    }

    pVirAddr = RK_MPI_MB_Handle2VirAddr(blk);
    printf("pVirAddr = %p\n",pVirAddr);

    FILE *fp = fopen(input_path, "rb");
    size_t bytesRead = fread(pVirAddr, sizeof(char), width * height * 3, fp);
    printf("bytesRead=%d\n",bytesRead);
    fflush(fp);
    fclose(fp);

    RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);

    TDE_SURFACE_S pstSrcSurface;
    TDE_RECT_S pstSrcRect;

    pstSrcSurface.pMbBlk           = blk;
    pstSrcSurface.u32Width         = width;
    pstSrcSurface.u32Height        = height;
    pstSrcSurface.enColorFmt       = FORMATE;
    pstSrcSurface.enComprocessMode = COMPRESS_MODE_NONE;
    pstSrcRect.s32Xpos             = 0;
    pstSrcRect.s32Ypos             = 0;
    pstSrcRect.u32Width            = width;
    pstSrcRect.u32Height           = height;

    TDE_SURFACE_S pstDstSurface;
    TDE_RECT_S pstDstRect;

    get_tde_size(dst_width,dst_height,&buffersize);
    printf("calc picture size: %d\n", buffersize);
    MB_BLK pstDstBlk = RK_NULL;
    s32Ret = RK_MPI_SYS_MmzAlloc(&pstDstBlk, RK_NULL, RK_NULL, buffersize);

    pstDstSurface.pMbBlk           = pstDstBlk;
    pstDstSurface.u32Width         = dst_width;
    pstDstSurface.enColorFmt       = FORMATE;
    pstDstSurface.u32Height        = dst_height;
    pstDstSurface.enComprocessMode = COMPRESS_MODE_NONE;
    pstDstRect.s32Xpos             = 0;
    pstDstRect.s32Ypos             = 0;
    pstDstRect.u32Width            = dst_width;
    pstDstRect.u32Height           = dst_height;

    RK_TDE_Open();
    TDE_HANDLE hHandle = RK_TDE_BeginJob();
    if (RK_ERR_TDE_INVALID_HANDLE == hHandle) {
        RK_LOGE("start job fail");
        return RK_FAILURE;
    }

    s32Ret = RK_TDE_QuickResize(hHandle, &pstSrcSurface, &pstSrcRect,&pstDstSurface, &pstDstRect);
    if (s32Ret != RK_SUCCESS) {
        RK_TDE_CancelJob(hHandle);
        return RK_FAILURE;
    }

    fp = fopen(output_path, "wb+");
	void *data = RK_MPI_MB_Handle2VirAddr(pstDstSurface.pMbBlk);
    char *buf;
    buf=(char *)malloc(dst_width*dst_height*3);
    for(int i=0;i<dst_width*dst_height*3 ;i++){
        printf("%2X",buf[i]);
    }
    fwrite(data, 1,dst_width*dst_height*3 , fp);
    fflush(fp);
    fclose(fp);

    s32Ret = RK_TDE_EndJob(hHandle, RK_FALSE, RK_TRUE, 10);
        if (s32Ret != RK_SUCCESS) {
        RK_TDE_CancelJob(hHandle);
        return RK_FAILURE;
    }
    RK_TDE_WaitForDone(hHandle);
    RK_TDE_CancelJob(hHandle);
    RK_TDE_Close();
    RK_MPI_SYS_Free(blk);
    RK_MPI_SYS_Free(pstDstBlk);
    return -1;
}



int venc_save_jpg(char* input_path,int width,int height,char *output_path){
    RK_S32 s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S pstRecvParam;
    VENC_CHN_ATTR_S stAttr;
    void *pVirAddr = RK_NULL;
    int ret;
    VIDEO_FRAME_INFO_S stFrame;
    VENC_STREAM_S rcv;
    int buffersize;

    char *src_buf;
    //初始化mpi
    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;

    stPicBufAttr.u32Width = width;
    stPicBufAttr.u32Height = height;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)FORMATE;
    stPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
    s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        printf("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }
    
    buffersize= stMbPicCalResult.u32MBSize;
    printf("calc picture size: %d\n", buffersize);
    memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
    memset(&pstRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));

    //h264
    stAttr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
    stAttr.stVencAttr.u32PicWidth = width;
    stAttr.stVencAttr.u32PicHeight = height;
    stAttr.stVencAttr.u32VirWidth = width;
    stAttr.stVencAttr.u32VirHeight = height;    
    stAttr.stVencAttr.u32MaxPicWidth = width;
    stAttr.stVencAttr.u32MaxPicHeight = height;
    stAttr.stVencAttr.u32StreamBufCnt = 2;
    stAttr.stVencAttr.u32Profile = 0;
    stAttr.stVencAttr.u32BufSize = width * height;
    stAttr.stVencAttr.enPixelFormat = FORMATE;
    stAttr.stVencAttr.bByFrame = RK_TRUE;
    stAttr.stVencAttr.enMirror = MIRROR_NONE;
    stAttr.stVencAttr.stAttrJpege.bSupportDCF = RK_FALSE;
    stAttr.stVencAttr.stAttrJpege.stMPFCfg.u8LargeThumbNailNum = 0;
    stAttr.stVencAttr.stAttrJpege.enReceiveMode = VENC_PIC_RECEIVE_SINGLE;
    
    //初始化通道
    ret = RK_MPI_VENC_CreateChn(0,&stAttr);
    if(ret < 0)
        printf("CreateChn ret = 0x%X\n",ret);
    pstRecvParam.s32RecvPicNum=1;
    ret = RK_MPI_VENC_StartRecvFrame(0, &pstRecvParam);
    if(ret < 0)
        printf("StartRecvFrame ret = 0x%X\n",ret);

    memset(&stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
    MB_BLK blk = RK_NULL;
    ret = RK_MPI_SYS_MmzAlloc(&blk, RK_NULL, RK_NULL,buffersize);
    if (RK_NULL == blk) {
        printf("RK_MPI_MB_GetMB fail\n");
        usleep(2000llu);
    }

    pVirAddr = RK_MPI_MB_Handle2VirAddr(blk);
    printf("pVirAddr = %p\n",pVirAddr);

    FILE *fp = fopen(input_path, "rb");
    size_t bytesRead = fread(pVirAddr, sizeof(char), width * height * 3, fp);
    printf("bytesRead=%d\n",bytesRead);
    fflush(fp);
    fclose(fp);

    RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);

    stFrame.stVFrame.pMbBlk = blk;
    stFrame.stVFrame.u32Width = width;
    stFrame.stVFrame.u32Height = height;
    stFrame.stVFrame.u32VirWidth = width;
    stFrame.stVFrame.u32VirHeight = height;
    stFrame.stVFrame.enPixelFormat = FORMATE;
    stFrame.stVFrame.u32FrameFlag |= FRAME_FLAG_SNAP_END;

    ret = RK_MPI_VENC_SendFrame(0, &stFrame, -1);
    src_buf=(char *)malloc(width*height*3);

    fp = fopen(output_path, "wb+");
    rcv.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));
    ret = RK_MPI_VENC_GetStream(0, &rcv, 3000);
    if (ret == RK_SUCCESS) {
		void *data = RK_MPI_MB_Handle2VirAddr(rcv.pstPack->pMbBlk);
        memcpy(src_buf,data,rcv.pstPack->u32Len);
		fwrite(data, 1, rcv.pstPack->u32Len, fp);
        printf("pic size = %d\n",rcv.pstPack->u32Len);
        fflush(fp);
        fclose(fp);
    }

    RK_MPI_SYS_Free(blk);
    RK_MPI_VENC_ReleaseStream(0, &rcv);
    RK_MPI_VENC_StopRecvFrame(0);
    RK_MPI_VENC_DestroyChn(0);
    return -1;
}


int main(){
    int width=1920;
    int height=1080;
    int scale=4;

    int dst_width=width/scale;
    int dst_height=height/scale;

    venc_resize("picture.RGB888",width,height,"480x270.RGB888",dst_width,dst_height);
    venc_save_jpg("480x270.RGB888",width,height,"480x270.jpg");
}