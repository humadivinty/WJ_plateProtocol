// WJSWDLL.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "WJSWDLL.h"
#include "CameraModule/Camera6467_plate.h"
#include "CameraModule/Camera6467_VFR.h"
#include "utilityTool/ToolFunction.h"
#include "CameraModule/DeviceListManager.h"
#include "ResultSentStatusManager.h"
#include "ToolImgeProcessFunc.h"
#include <algorithm>

Camera6467_plate* pCamera = NULL;

#ifdef WINDOWS
#define WRITE_LOG(fmt, ...) Tool_WriteFormatLog("%s:: "fmt, __FUNCTION__, ##__VA_ARGS__);
#else
//#define WRITE_LOG(...) Tool_WriteFormatLog("%s:: ", __FUNCTION__, ##__VA_ARGS__);
#define WRITE_LOG(fmt,...) Tool_WriteFormatLog("%s:: " fmt, __FUNCTION__,##__VA_ARGS__);
#endif

#define CAMERA_TYPE_PLATE 0
#define CAMERA_TYPE_VFR 1

std::list<CameraInfo> g_CameraInfoList;
int g_iCameraMode = CAMERA_TYPE_PLATE;
std::shared_ptr<uint8_t> g_pImgData = nullptr;

static int g_MsgPLATENO = ::RegisterWindowMessage("PLATENO");

BaseCamera* GetCamerByModeAndLaneNo(int iMode, int LaneNo)
{
    BaseCamera* pCamTemp = NULL;
    switch (iMode)
    {
    case CAMERA_TYPE_PLATE:
        pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
        break;
    case CAMERA_TYPE_VFR:
        pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(1);
        break;
    default:
        break;
    }
    return pCamTemp;
}

uint8_t* GetImgBufferAddress()
{
    uint8_t* pData = NULL;
    if (!g_pImgData)
    {
        g_pImgData = std::shared_ptr<uint8_t>(new uint8_t[MAX_IMG_SIZE], std::default_delete<uint8_t[]>());
    }
    if (g_pImgData)
    {
        pData = g_pImgData.get();
        memset(pData, 0, MAX_IMG_SIZE);
    }
    return pData;
}

void PrintStatuse(ResultSentStatus sta)
{
    WRITE_LOG("status :     int DeviceID = %d,   dwCarID = %lu, iFrontImgSendStatus = %d , iSidImgSendStatus = %d , \
                      iTailImgSendStatus = %d ,  iVideoSendStatus = %d , iSmallImgSendStatus = %d iBinaryImgSendStatus = %d , uTimeReceive = %lu ",
                      sta.DeviceID,
                      sta.dwCarID,
                      sta.iFrontImgSendStatus,
                      sta.iSidImgSendStatus,
                      sta.iTailImgSendStatus,
                      sta.iVideoSendStatus,
                      sta.iSmallImgSendStatus,
                      sta.iBinaryImgSendStatus,
                      sta.uTimeReceive);
}


WJSWDLL_API int DELSPEC WVS_Initialize()
{
    WRITE_LOG("begin ");
    
    g_CameraInfoList.clear();
    Tool_LoadCamerInfoFromINI(INI_FILE_NAME, g_CameraInfoList);
    WRITE_LOG("read %s finish. ", INI_FILE_NAME);

    Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "CameraMode", "type", g_iCameraMode);
    WRITE_LOG("read from %s, camera mode = %d ( %s )", 
        INI_FILE_NAME, 
        g_iCameraMode,
        g_iCameraMode == 0 ? "CAMERA_TYPE_PLATE" : "CAMERA_TYPE_VFR");

    BaseCamera* pTemp = NULL;
    for (std::list<CameraInfo>::iterator it = g_CameraInfoList.begin(); it != g_CameraInfoList.end(); it++)
    {
        pTemp = DeviceListManager::GetInstance()->GetDeviceByIpAddress(it->chIP);
        if (NULL == pTemp)
        {
            switch (it->iDeviceType)
            {
            case CAMERA_TYPE_PLATE:
                pTemp = new Camera6467_plate();
                WRITE_LOG("camera type is %d, create Camera6467_plate ", it->iDeviceType);
                break;
            case CAMERA_TYPE_VFR:
                pTemp = new Camera6467_VFR();
                WRITE_LOG("camera type is %d, create Camera6467_VFR ", it->iDeviceType);
                break;
            default:
                pTemp = new Camera6467_plate();
                WRITE_LOG("camera type is %d, create Camera6467_plate default", it->iDeviceType);
                break;
            }
            pTemp->SetCameraIP(it->chIP);
            pTemp->SetLoginID(it->iDeviceID);
            pTemp->SetDeviceType(it->iDeviceType);
            pTemp->ConnectToCamera();
            int iSaveFrameID = it->iSaveFrameChannelID;
            pTemp->SetH264Callback(iSaveFrameID, 0, 0, 0xffff0700);
            pTemp->SetWindowsWndForResultComming(HWND_BROADCAST, g_MsgPLATENO);

            DeviceListManager::GetInstance()->AddOneDevice(it->iDeviceID, pTemp);
            WRITE_LOG("camera %s is create success", it->chIP);
        }
        else
        {
            WRITE_LOG("camera %s is already exsit , do nothing", it->chIP);
        }
    }
    WRITE_LOG("finish.");
    return 0;
}

WJSWDLL_API int DELSPEC WVS_CloseHv()
{
    WRITE_LOG("begin.");
    DeviceListManager::GetInstance()->ClearAllDevice();
    WRITE_LOG("finish");
    return 0;
}

WJSWDLL_API int DELSPEC WVS_Settime(int LaneNo)
{
    WRITE_LOG("begin , LaneNo = %d.", LaneNo);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        if (pCamTemp->SynTime())
        {
            WRITE_LOG("SynTime success");
            iRet = 0;
        }
        else
        {
            WRITE_LOG("SynTime failed.");
        }        
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_GetBigImage(unsigned char LaneNo, int IdentNo, char* ImgBuf, int ImgBufLen, int* ImgSize)
{
    WRITE_LOG("begin , LaneNo = %d, IdentNo = %d, ImgBuf = %p, ImgBufLen = %d, ImgSize = %p.", LaneNo, IdentNo, ImgBuf, ImgBufLen, ImgSize);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        std::shared_ptr<CameraResult> pResult;
        ResultSentStatus SentStatus;
        int iItem = LaneNo;
        long iValue = 0;
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        case CAMERA_TYPE_VFR:
            if (((Camera6467_VFR*)pCamTemp)->GetFrontSendStatus(SentStatus))
            {
                PrintStatuse(SentStatus);
                pResult = ((Camera6467_VFR*)pCamTemp)->GetResultByCarID(SentStatus.dwCarID);
            }
            break;
        default:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        }

        if (pResult)
        {            
            unsigned char* pImagData = NULL;
            unsigned long dwIageDataLength = 0;

            const char* pChPlateNumber = pResult->chPlateNO;
            unsigned long dwCarID = pResult->dwCarID;
            
            switch (pCamTemp->GetDeviceType())
            {
            case CAMERA_TYPE_PLATE:
                if (pResult->CIMG_BestSnapshot.dwImgSize > 0
                    && NULL != pResult->CIMG_BestSnapshot.pbImgData)
                {
                    pImagData = pResult->CIMG_BestSnapshot.pbImgData;
                    dwIageDataLength = pResult->CIMG_BestSnapshot.dwImgSize;
                }
                else
                {
                    pImagData = pResult->CIMG_LastSnapshot.pbImgData;
                    dwIageDataLength = pResult->CIMG_LastSnapshot.dwImgSize;
                }
                break;
            case CAMERA_TYPE_VFR:
                if (SentStatus.iFrontImgSendStatus == sta_waitGet)
                {
                    pImagData = pResult->CIMG_BeginCapture.pbImgData;
                    dwIageDataLength = pResult->CIMG_BeginCapture.dwImgSize;

                    iItem = item_frontImg;
                    iValue = sta_sentFinish;                    
                }
                else if (SentStatus.iSidImgSendStatus == sta_waitGet)
                {
                    pImagData = pResult->CIMG_BestCapture.pbImgData;
                    dwIageDataLength = pResult->CIMG_BestCapture.dwImgSize;

                    iItem = item_sideImg;
                    iValue = sta_sentFinish;
                }
                else if (SentStatus.iTailImgSendStatus == sta_waitGet)
                {
                    pImagData = pResult->CIMG_LastCapture.pbImgData;
                    dwIageDataLength = pResult->CIMG_LastCapture.dwImgSize;

                    iItem = item_tailImg;
                    iValue = sta_sentFinish;
                }
                else
                {
                    WRITE_LOG("get result success, but item is not ready.", );
                }
                break;
            default:
                pImagData = pResult->CIMG_BestSnapshot.pbImgData;
                dwIageDataLength = pResult->CIMG_BestSnapshot.dwImgSize;
                break;
            }           

            if ( (pImagData == NULL  || dwIageDataLength == 0)
                && CAMERA_TYPE_VFR ==pCamTemp->GetDeviceType()
                )
            {
                WRITE_LOG("item is invalid , use lane number to get result type.");
                switch (LaneNo)
                {
                case item_frontImg:
                    pImagData = pResult->CIMG_BeginCapture.pbImgData;
                    dwIageDataLength = pResult->CIMG_BeginCapture.dwImgSize;

                    iItem = item_frontImg;
                    iValue = sta_sentFinish;
                    break;
                case item_sideImg:
                    pImagData = pResult->CIMG_BestCapture.pbImgData;
                    dwIageDataLength = pResult->CIMG_BestCapture.dwImgSize;

                    iItem = item_sideImg;
                    iValue = sta_sentFinish;
                    break;

                case item_tailImg:
                    pImagData = pResult->CIMG_LastCapture.pbImgData;
                    dwIageDataLength = pResult->CIMG_LastCapture.dwImgSize;

                    iItem = item_tailImg;
                    iValue = sta_sentFinish;
                    break;
                default:
                    pImagData = pResult->CIMG_BeginCapture.pbImgData;
                    dwIageDataLength = pResult->CIMG_BeginCapture.dwImgSize;

                    iItem = item_frontImg;
                    iValue = sta_sentFinish;
                    break;
                }
            }

            WRITE_LOG("get result success , carID = %lu, plate number = %s, big image data = %p,image length = %lu ",
                dwCarID,
                pChPlateNumber,
                pImagData,
                dwIageDataLength);

            if (ImgBufLen < dwIageDataLength)
            {
                WRITE_LOG("ImgBufLen %d < dwIageDataLength %lu , compress image.", ImgBufLen, dwIageDataLength);
                unsigned char* pBuf = GetImgBufferAddress();
                size_t iBufLength = MAX_IMG_SIZE;
                if (0 == Tool_CompressImg(pImagData, dwIageDataLength, pBuf, iBufLength, ImgBufLen - 1))
                {
                    WRITE_LOG("compress success, begin to copy image data., size = %d", iBufLength);
                    memset(ImgBuf, 0, ImgBufLen);
                    memcpy(ImgBuf, pBuf, iBufLength);
                    *ImgSize = iBufLength;
                    WRITE_LOG("finish copy image data.");
                    iRet = 0;
                }
                else
                {
                    WRITE_LOG("compress image failed, do not copy data.");
                }
            }
            else
            {
                if (ImgBufLen >= dwIageDataLength
                    && 0 != dwIageDataLength)
                {
                    WRITE_LOG("begin to copy image data.");
                    memset(ImgBuf, 0, ImgBufLen);
                    memcpy(ImgBuf, pImagData, dwIageDataLength);
                    *ImgSize = dwIageDataLength;
                    WRITE_LOG("finish copy image data.");
                    iRet = 0;
                }
                else
                {
                    *ImgSize = 0;
                    WRITE_LOG("ImgBufLen %d is smaller than image data length %lu, do not copy data.", ImgBufLen, dwIageDataLength);
                }
            }

            //if (ImgBufLen >= dwIageDataLength
            //    && 0 != dwIageDataLength)
            //{
            //    WRITE_LOG("begin to copy image data.");
            //    memset(ImgBuf, 0 , ImgBufLen);
            //    memcpy(ImgBuf, pImagData, dwIageDataLength);
            //    *ImgSize = dwIageDataLength;
            //    WRITE_LOG("finish copy image data.");
            //    iRet = 0;
            //}
            //else
            //{
            //    *ImgSize = 0;
            //    WRITE_LOG("ImgBufLen %d is smaller than image data length %lu, do not copy data.", ImgBufLen, dwIageDataLength);
            //}

            if (CAMERA_TYPE_VFR == pCamTemp->GetDeviceType())
            {
                WRITE_LOG("current device type is VFR , upadate sent status , carid = %d , item = %d , value = %d.", pResult->dwCarID, iItem, iValue);
                ((Camera6467_VFR*)pCamTemp)->UpdateSendStatus(pCamTemp->GetLoginID(), pResult->dwCarID, iItem, iValue);
                ((Camera6467_VFR*)pCamTemp)->SetIfSendResult(false);
            }
        }
        else
        {
            WRITE_LOG("result is not ready");
        }
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_GetFarBigImage(unsigned char LaneNo, int IdentNo, char* ImgBuf, int ImgBufLen, int* ImgSize)
{
    WRITE_LOG("begin , LaneNo = %d, IdentNo = %d, ImgBuf = %p, ImgBufLen = %d, ImgSize = %p.", LaneNo, IdentNo, ImgBuf, ImgBufLen, ImgSize);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        std::shared_ptr<CameraResult> pResult;
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        case CAMERA_TYPE_VFR:
            pResult = ((Camera6467_VFR*)pCamTemp)->GetFrontResult();
            break;
        default:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        }

        if (pResult)
        {
            iRet = 0;
            unsigned char* pImagData = NULL;
            unsigned long dwIageDataLength = 0;

            const char* pChPlateNumber = pResult->chPlateNO;
            unsigned long dwCarID = pResult->dwCarID;

            switch (pCamTemp->GetDeviceType())
            {
            case CAMERA_TYPE_PLATE:
                pImagData = pResult->CIMG_LastSnapshot.pbImgData;
                dwIageDataLength = pResult->CIMG_LastSnapshot.dwImgSize;
                break;
            case CAMERA_TYPE_VFR:
                pImagData = pResult->CIMG_BeginCapture.pbImgData;
                dwIageDataLength = pResult->CIMG_BeginCapture.dwImgSize;
                break;
            default:
                pImagData = pResult->CIMG_LastSnapshot.pbImgData;
                dwIageDataLength = pResult->CIMG_LastSnapshot.dwImgSize;
                break;
            }

            WRITE_LOG("get result success is , carID = %lu, plate number = %s, big image data = %p,image length = %lu ",
                dwCarID,
                pChPlateNumber,
                pImagData,
                dwIageDataLength);

            if (ImgBufLen < dwIageDataLength)
            {
                WRITE_LOG("ImgBufLen %d < dwIageDataLength %lu , compress image.", ImgBufLen, dwIageDataLength);
                unsigned char* pBuf = GetImgBufferAddress();
                size_t iBufLength = MAX_IMG_SIZE;
                if (0 == Tool_CompressImg(pImagData, dwIageDataLength, pBuf, iBufLength, ImgBufLen - 1))
                {
                    WRITE_LOG("compress success, begin to copy image data., size = %d", iBufLength);
                    memset(ImgBuf, 0, ImgBufLen);
                    memcpy(ImgBuf, pBuf, iBufLength);
                    *ImgSize = iBufLength;
                    WRITE_LOG("finish copy image data.");
                    iRet = 0;
                }
                else
                {
                    WRITE_LOG("compress image failed, do not copy data.");
                }
            }
            else
            {
                if (ImgBufLen >= dwIageDataLength
                    && 0 != dwIageDataLength)
                {
                    WRITE_LOG("begin to copy image data.");
                    memset(ImgBuf, 0, ImgBufLen);
                    memcpy(ImgBuf, pImagData, dwIageDataLength);
                    *ImgSize = dwIageDataLength;
                    WRITE_LOG("finish copy image data.");
                    iRet = 0;
                }
                else
                {
                    *ImgSize = 0;
                    WRITE_LOG("ImgBufLen %d is smaller than image data length %lu, do not copy data.", ImgBufLen, dwIageDataLength);
                }
            }

            //if (ImgBufLen >= dwIageDataLength
            //    && 0 != dwIageDataLength)
            //{
            //    WRITE_LOG("begin to copy image data.");
            //    memset(ImgBuf, 0, ImgBufLen);
            //    memcpy(ImgBuf, pImagData, dwIageDataLength);
            //    *ImgSize = dwIageDataLength;
            //    WRITE_LOG("finish copy image data.");
            //}
            //else
            //{
            //    *ImgSize = 0;
            //    WRITE_LOG("ImgBufLen %d is smaller than image data length %lu, do not copy data.", ImgBufLen, dwIageDataLength);
            //}
        }
        else
        {
            WRITE_LOG("result is not ready");
        }
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_GetSmallImage(unsigned char LaneNo, int IdentNo, char* ImgBuf, int ImgBufLen, int* ImgSize)
{
    WRITE_LOG("begin , LaneNo = %d, IdentNo = %d, ImgBuf = %p, ImgBufLen = %d, ImgSize = %p.", LaneNo,  IdentNo,  ImgBuf,  ImgBufLen,  ImgSize);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        ResultSentStatus SentStatus;
        int iItem = item_receiveTime;
        long iValue = 0;

        std::shared_ptr<CameraResult> pResult;
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        case CAMERA_TYPE_VFR:
            if (((Camera6467_VFR*)pCamTemp)->GetFrontSendStatus(SentStatus))
            {
                pResult = ((Camera6467_VFR*)pCamTemp)->GetResultByCarID(SentStatus.dwCarID);
            }
            break;
        default:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        }

        if (pResult)
        {
            iRet = 0;
            unsigned char* pImagData = NULL;
            unsigned long dwIageDataLength = 0;

            const char* pChPlateNumber = pResult->chPlateNO;
            unsigned long dwCarID = pResult->dwCarID;

            switch (pCamTemp->GetDeviceType())
            {
            case CAMERA_TYPE_PLATE:
            case CAMERA_TYPE_VFR:
            default:
                pImagData = pResult->CIMG_PlateImage.pbImgData;
                dwIageDataLength = pResult->CIMG_PlateImage.dwImgSize;
                break;
            }

            WRITE_LOG("get result success is , carID = %lu, plate number = %s, big image data = %p,image length = %lu ",
                dwCarID,
                pChPlateNumber,
                pImagData,
                dwIageDataLength);

            if (ImgBufLen >= dwIageDataLength
                && 0 != dwIageDataLength)
            {
                WRITE_LOG("begin to copy image data.");
                memset(ImgBuf, 0, ImgBufLen);
                memcpy(ImgBuf, pImagData, dwIageDataLength);
                *ImgSize = dwIageDataLength;
                WRITE_LOG("finish copy image data.");
            }
            else
            {
                *ImgSize = 0;
                WRITE_LOG("ImgBufLen %d is smaller than image data length %lu, do not copy data.", ImgBufLen, dwIageDataLength);
            }

            if (CAMERA_TYPE_VFR == pCamTemp->GetDeviceType()
                /*&& TRUE == bRet*/)
            {
                int iItem = item_smallImg;
                long iValue = sta_sentFinish;
                WRITE_LOG("current device type is VFR , upadate sent status , carid = %d , item = %d , value = %d.", pResult->dwCarID, iItem, iValue);
                ((Camera6467_VFR*)pCamTemp)->UpdateSendStatus(pCamTemp->GetLoginID(), pResult->dwCarID, iItem, iValue);
            }
        }
        else
        {
            WRITE_LOG("result is not ready");
        }
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_GetPlateNo(unsigned char LaneNo, int IdentNo, char* PlateNo, int PlateNoLen)
{
    WRITE_LOG("begin , LaneNo = %d, IdentNo = %d, PlateNo = %p, PlateNoLen = %d.", LaneNo, IdentNo, PlateNo, PlateNoLen);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        ResultSentStatus SentStatus;
        int iItem = item_receiveTime;
        long iValue = 0;

        std::shared_ptr<CameraResult> pResult;
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        case CAMERA_TYPE_VFR:
            if (((Camera6467_VFR*)pCamTemp)->GetFrontSendStatus(SentStatus))
            {
                pResult = ((Camera6467_VFR*)pCamTemp)->GetResultByCarID(SentStatus.dwCarID);
            }
            break;
        default:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        }

        if (pResult)
        {
            iRet = 0;
            const char* pChPlateNumber = pResult->chPlateNO;
            unsigned long dwCarID = pResult->dwCarID;

            WRITE_LOG("get result success is , carID = %lu, plate number = %s",
                dwCarID,
                pChPlateNumber);

            memset(PlateNo, '\0', PlateNoLen);
            if (strlen(pChPlateNumber) > PlateNoLen)
            {
                WRITE_LOG("plate buffer length is smaller than plate , only copy %d BYTE data", PlateNoLen - 1);
                memcpy(PlateNo, pChPlateNumber, PlateNoLen - 1);
            }
            else
            {
                sprintf(PlateNo, "%s", pChPlateNumber);
            }       
        }
        else
        {
            WRITE_LOG("result is not ready");
        }
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_ForceSendLaneHv(int LaneNo, int ImageOutType)
{
    WRITE_LOG("begin , LaneNo = %d, ImageOutType = %d.", LaneNo, ImageOutType);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        if (pCamTemp->TakeCapture())
        {
            WRITE_LOG("TakeCapture success");
            iRet = 0;
        }
        else
        {
            WRITE_LOG("TakeCapture failed");
        }       
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_StartRealPlay(int LaneNo, HWND FormHwnd)
{
    WRITE_LOG("begin , LaneNo = %d, FormHwnd = %p.", LaneNo, FormHwnd);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        auto camInfo = std::find_if(std::begin(g_CameraInfoList), 
            std::end(g_CameraInfoList), 
            [LaneNo](CameraInfo it)
        {
            return (it.iDeviceID == LaneNo);
        });
        int iVideoChannelID = (camInfo == std::end(g_CameraInfoList) ) ? 0 : camInfo->iVideoChannelID;
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
        case CAMERA_TYPE_VFR:   
        default:
            pCamTemp->StartPlayVideoByChannel(iVideoChannelID, FormHwnd);
            break;
        }
        iRet = 0;
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_StopRealPlay(int LaneNo)
{
    WRITE_LOG("begin , LaneNo = %d.", LaneNo);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        auto camInfo = std::find_if(std::begin(g_CameraInfoList),
            std::end(g_CameraInfoList),
            [LaneNo](CameraInfo it)
        {
            return (it.iDeviceID == LaneNo);
        });
        int iVideoChannelID = (camInfo == std::end(g_CameraInfoList)) ? 0 : camInfo->iVideoChannelID;

        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
        case CAMERA_TYPE_VFR:
        default:
            pCamTemp->StopPlayVideoByChannel(iVideoChannelID);
            break;
        }
        iRet = 0;
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_Startrecord(int LaneNo, char *sFileName)
{
    if (LaneNo < 0
        || NULL == sFileName
        || strlen(sFileName) == 0)
    {
        WRITE_LOG("parameter is invalid.");
        return -1;
    }
    WRITE_LOG("begin , LaneNO = %d, sFileName = %s.", LaneNo, sFileName);

    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
            pCamTemp->StartToSaveAviFile(0, sFileName, pCamTemp->GetCurrentH264FrameTime());
            break;
        case CAMERA_TYPE_VFR:
            pCamTemp->StartToSaveVideoUnlimited(0, sFileName, pCamTemp->GetCurrentH264FrameTime());
            break;
        //case CAMERA_TYPE_VFR:
        //    WRITE_LOG(" %d is CAMERA_TYPE_VFR,  unsupport save video with no finish time.", LaneNo);
        //    break;
        default:            
            break;
        }
        iRet = 0;
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_Stoprecord(int LaneNo)
{
    WRITE_LOG("begin , LaneNo = %d.", LaneNo);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
            pCamTemp->StopSaveAviFile(0);
            break;
        case CAMERA_TYPE_VFR:
            pCamTemp->StopSaveVideoUnlimited(0);
            break;
        //case CAMERA_TYPE_VFR:
        //    WRITE_LOG(" %d is CAMERA_TYPE_VFR,  unsupport stop video with no begin time.", LaneNo);
        //    break;
        default:
            break;
        }
        iRet = 0;
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_GetHvIsConnected(int LaneNo)
{
    WRITE_LOG("begin , LaneNo = %d.", LaneNo);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        if (0 == pCamTemp->GetCamStatus())
        {
            iRet = 0;
        }
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_Getrecord(int LaneNo, char *sFileName)
{
    WRITE_LOG("begin , LaneNo = %d, sFileName = %s.", LaneNo, sFileName);
    int iRet = -1;
    BaseCamera* pCamTemp = GetCamerByModeAndLaneNo(g_iCameraMode, LaneNo);
    if (NULL != pCamTemp)
    {
        std::shared_ptr<CameraResult> pResult;
        ResultSentStatus SentStatus;
        int iItem = item_receiveTime;
        long iValue = 0;
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        case CAMERA_TYPE_VFR:
            if (((Camera6467_VFR*)pCamTemp)->GetFrontSendStatus(SentStatus))
            {
                pResult = ((Camera6467_VFR*)pCamTemp)->GetResultByCarID(SentStatus.dwCarID);
            }
            break;
        default:
            pResult = std::shared_ptr<CameraResult>(((Camera6467_plate*)pCamTemp)->GetOneResult());
            break;
        }

        if (pResult)
        {
            WRITE_LOG("get result success is , carID = %lu, plate number = %s",   pResult->dwCarID,  pResult->chPlateNO);

            bool bVideoReady = false;
            for (int iTryTime = 0; iTryTime < 15; iTryTime++)
            {
                if (pCamTemp->FindIfFileNameInReciveList(pResult->chSaveFileName))
                {
                    bVideoReady = true;
                    break;                    
                }
                Sleep(100);
            }

            BOOL bRet = FALSE;
            if (bVideoReady)
            {
                WRITE_LOG("video %s is ready, rename to the new path %s", pResult->chSaveFileName, sFileName);
                bRet = CopyFile(pResult->chSaveFileName, sFileName, FALSE);
                WRITE_LOG("CopyFile video %s  finish , operation code = %d, getlast error = %s",
                    sFileName,
                    bRet,
                    bRet ? "NULL" : Tool_GetLastErrorAsString().c_str());

                iRet = bRet ? 0 : iRet;
            }
            else
            {
                WRITE_LOG("video %s is not ready", pResult->chSaveFileName);
            }

            if (CAMERA_TYPE_VFR == pCamTemp->GetDeviceType()
                /*&& TRUE == bRet*/)
            {
                int iItem = item_video;
                long iValue = sta_sentFinish;
                WRITE_LOG("current device type is VFR , upadate sent status , carid = %d , item = %d , value = %d.", pResult->dwCarID, iItem, iValue);
                ((Camera6467_VFR*)pCamTemp)->UpdateSendStatus(pCamTemp->GetLoginID(), pResult->dwCarID, iItem, iValue);
                ((Camera6467_VFR*)pCamTemp)->SetIfSendResult(false);

                if (TRUE == bRet)
                {
                    remove(pResult->chSaveFileName);
                }
            }
        }
        else
        {
            WRITE_LOG("result is not ready");
        }
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNo);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}
