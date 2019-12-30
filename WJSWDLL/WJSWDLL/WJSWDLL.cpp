// WJSWDLL.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "WJSWDLL.h"
#include "CameraModule/Camera6467_plate.h"
#include "CameraModule/Camera6467_VFR.h"
#include "utilityTool/ToolFunction.h"
#include "CameraModule/DeviceListManager.h"
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
static int g_MsgPLATENO = ::RegisterWindowMessage("PLATENO");

WJSWDLL_API int DELSPEC WVS_Initialize()
{
    WRITE_LOG("begin ");
    
    g_CameraInfoList.clear();
    Tool_LoadCamerInfoFromINI(INI_FILE_NAME, g_CameraInfoList);
    WRITE_LOG("read %s finish. ", INI_FILE_NAME);

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
            pTemp->ConnectToCamera();
            pTemp->SetDeviceType(it->iDeviceType);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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
                pImagData = pResult->CIMG_BeginCapture.pbImgData;
                dwIageDataLength = pResult->CIMG_BeginCapture.dwImgSize;
            default:
                pImagData = pResult->CIMG_BestSnapshot.pbImgData;
                dwIageDataLength = pResult->CIMG_BestSnapshot.dwImgSize;
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
                memset(ImgBuf, 0 , ImgBufLen);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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

WJSWDLL_API int DELSPEC WVS_Startrecord(int LaneNO, char *sFileName)
{
    if (LaneNO < 0
        || NULL == sFileName
        || strlen(sFileName) == 0)
    {
        WRITE_LOG("parameter is invalid.");
        return -1;
    }
    WRITE_LOG("begin , LaneNO = %d, sFileName = %s.", LaneNO, sFileName);

    int iRet = -1;
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNO);
    if (NULL != pCamTemp)
    {
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
        case CAMERA_TYPE_VFR:
        default:
            pCamTemp->StartToSaveAviFile(0, sFileName, pCamTemp->GetCurrentH264FrameTime());
            break;
        }
        iRet = 0;
    }
    else
    {
        WRITE_LOG("can not find the camer by Id %d .", LaneNO);
    }
    WRITE_LOG("finish, return %d", iRet);
    return iRet;
}

WJSWDLL_API int DELSPEC WVS_Stoprecord(int LaneNo)
{
    WRITE_LOG("begin , LaneNo = %d.", LaneNo);
    int iRet = -1;
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
    if (NULL != pCamTemp)
    {
        switch (pCamTemp->GetDeviceType())
        {
        case CAMERA_TYPE_PLATE:
        case CAMERA_TYPE_VFR:
        default:
            pCamTemp->StopSaveAviFile(0);
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
    BaseCamera* pCamTemp = DeviceListManager::GetInstance()->GetDeviceById(LaneNo);
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