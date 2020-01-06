// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 WJSWDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// WJSWDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef WJSWDLL_EXPORTS
#define WJSWDLL_API EXTERN_C  __declspec(dllexport)
#else
#define WJSWDLL_API EXTERN_C  __declspec(dllimport)
#endif

#define  DELSPEC __stdcall

//************************************
// Method:        WVS_Settime
// Describe:        校时函数，根据上位机时间给相机校时（在函数内获取系统当前时间）；
// FullName:      WVS_Settime
// Access:          public 
// Returns:        int
// Returns Describe:        0  成功；-1 失败，失败时记录错误日志
// Parameter:    int LaneNo ;  相机车道号
//************************************
WJSWDLL_API int DELSPEC WVS_Settime(int LaneNo);

//************************************
// Method:        WVS_Initialize
// Describe:        根据配置文件进行多个相机的初始化；包括以下内容： 
//                       初始化 SDK；读取配置文件；登陆相机；注册回调；相机校时
// FullName:      WVS_Initialize
// Access:          public 
// Returns:        int
// Returns Describe:        0  成功；-1 失败，失败时记录错误日志
//************************************
WJSWDLL_API int DELSPEC  WVS_Initialize();

//************************************
// Method:        WVS_CloseHv
// Describe:        相机退出函数 
// FullName:      WVS_CloseHv
// Access:          public 
// Returns:        int
// Returns Describe:        0  成功；-1 失败，失败时记录错误日志
//************************************
WJSWDLL_API int DELSPEC  WVS_CloseHv();

//************************************
// Method:        WVS_GetBigImage
// Describe:        获取近景图 
// FullName:      WVS_GetBigImage
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    unsigned char LaneNo    ;车道号，默认 1  ，用来区分相机 
// Parameter:    int IdentNo                    ;序号，默认 0，备用 
// Parameter:    char * ImgBuf                 ;存储图片缓存的长度 
// Parameter:    int ImgBufLen                 ;存储图片 
// Parameter:    int * ImgSize                   ;记录图片的实际大小 
//************************************
WJSWDLL_API int DELSPEC   WVS_GetBigImage(unsigned char  LaneNo, int  IdentNo, char*  ImgBuf, int  ImgBufLen, int*  ImgSize);

//************************************
// Method:        WVS_GetFarBigImage
// Describe:        获取远景图 
// FullName:      WVS_GetFarBigImage
// Access:          public 
// Returns:        int
// Returns Describe:                                    ;0  成功；-1 失败，失败时记录错误日志
// Parameter:    unsigned char LaneNo       ;车道号，默认 1  ，用来区分相机 
// Parameter:    int IdentNo                        ;序号，默认 0，备用 
// Parameter:    char * ImgBuf                      ;存储图片缓存的长度
// Parameter:    int ImgBufLen                      ;存储图片 
// Parameter:    int * ImgSize                        ;记录图片的实际大小 
//************************************
WJSWDLL_API int DELSPEC   WVS_GetFarBigImage(unsigned char  LaneNo, int  IdentNo, char*  ImgBuf, int  ImgBufLen, int* ImgSize);

//************************************
// Method:        WVS_GetSmallImage
// Describe:        获取车牌图
// FullName:      WVS_GetSmallImage
// Access:          public 
// Returns:        int
// Returns Describe:                                    ;0  成功；-1 失败，失败时记录错误日志
// Parameter:    unsigned char LaneNo       ;车道号，默认 1  ，用来区分相机 
// Parameter:    int IdentNo                        ;序号，默认 0，备用 
// Parameter:    char * ImgBuf                    ;存储图片缓存的长度 
// Parameter:    int ImgBufLen                    ;存储图片 
// Parameter:    int * ImgSize                      ;记录图片的实际大小 
//************************************
WJSWDLL_API int DELSPEC   WVS_GetSmallImage(unsigned char  LaneNo, int  IdentNo, char*  ImgBuf, int  ImgBufLen, int* ImgSize);

//************************************
// Method:        WVS_GetPlateNo
// Describe:        获取车牌号 
// FullName:      WVS_GetPlateNo
// Access:          public 
// Returns:        int
// Returns Describe:                                    ;0  成功；-1 失败，失败时记录错误日志
// Parameter:    unsigned char LaneNo       ;车道号，默认 1  ，用来区分相机
// Parameter:    int IdentNo                        ;序号，默认 0，备用 
// Parameter:    char * PlateNo                   ;存储车牌号，首字为车牌颜色
// Parameter:    int PlateNoLen                   ;存储图片缓存的长度 
//************************************
WJSWDLL_API int DELSPEC  WVS_GetPlateNo(unsigned char LaneNo, int IdentNo, char* PlateNo, int PlateNoLen);

//************************************
// Method:        WVS_ForceSendLaneHv
// Describe:        触发相机抓拍 
// FullName:      WVS_ForceSendLaneHv
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    int LaneNo                 ;车道号，默认 1  ，用来区分相机 
// Parameter:    int ImageOutType       ;输出图片类型，默认 4，备用 
//************************************
WJSWDLL_API int DELSPEC  WVS_ForceSendLaneHv(int LaneNo, int ImageOutType);

//************************************
// Method:        WVS_StartRealPlay
// Describe:       播放实时视频 
// FullName:      WVS_StartRealPlay
// Access:          public 
// Returns:        int
// Returns Describe:                                ;0  成功；-1 失败，失败时记录错误日志
// Parameter:    int LaneNo                     ;车道号，默认 1  ，用来区分相机
// Parameter:    HWND FormHwnd         ;C#播放窗口、控件句柄 
//************************************
WJSWDLL_API int DELSPEC  WVS_StartRealPlay(int LaneNo, HWND FormHwnd);

//************************************
// Method:        WVS_StopRealPlay
// Describe:        停止播放视频，释放资源 
// FullName:      WVS_StopRealPlay
// Access:          public 
// Returns:        int
// Returns Describe:                 ;0  成功；-1 失败，失败时记录错误日志
// Parameter:    int LaneNo      ;车道号，默认 1  ，用来区分相机
//************************************
WJSWDLL_API int DELSPEC  WVS_StopRealPlay(int LaneNo);

//************************************
// Method:        WVS_Startrecord
// Describe:       视频录像功能，该功能是在视频打开预览功能后，才可开启；调用此函数时开始录制短视频，调用停止录像时将文件输出。 
// FullName:      WVS_Startrecord
// Access:          public 
// Returns:        int
// Returns Describe:                             ;0  成功；-1 失败，失败时记录错误日志
// Parameter:    int LaneNO                 ;车道号，默认 1  ，用来区分相机
// Parameter:    char * sFileName         ;视频文件保存路径 
//************************************
WJSWDLL_API int DELSPEC WVS_Startrecord(int LaneNO, char *sFileName);

//************************************
// Method:        WVS_StopRealPlay
// Describe:        停止视频录像，调用停止录像时将文件输出。 
// FullName:      WVS_StopRealPlay
// Access:          public 
// Returns:        int
// Returns Describe:                        ;0  成功；-1 失败，失败时记录错误日志； 
// Parameter:    int LaneNo             ;车道号，默认 1  ，用来区分相机 
//************************************
WJSWDLL_API int DELSPEC WVS_Stoprecord(int LaneNo);

//************************************
// Method:        WVS_GetHvIsConnected
// Describe:        测试相机连接是否正常。 
// FullName:      WVS_GetHvIsConnected
// Access:          public 
// Returns:        int
// Returns Describe:                ;0  成功；-1 失败，失败时记录错误日志； 
// Parameter:    int LaneNo     ;车道号，默认 1  ，用来区分相机 
//************************************
WJSWDLL_API int DELSPEC WVS_GetHvIsConnected(int LaneNo);

 //************************************
 // Method:        WVS_Getrecord
// Describe:   ：该功能是在上位机接到录像完毕的消息后调用。调用此函数时相机将捕获到的视
//                      频按传入的完整路径进行保存。若缓存中包含多个视频文件则只保存最近的一个。
 // FullName:      WVS_Getrecord
 // Access:          public 
 // Returns:        int                            ;0  成功；-1 失败，失败时记录错误日志
 // Returns Describe:
 // Parameter:    int LaneNo               ;车道号，默认 1  ，用来区分相机
 // Parameter:    char * sFileName      ; 视频文件保存路径 
 //************************************
WJSWDLL_API int DELSPEC  WVS_Getrecord(int LaneNo, char *sFileName);

/*
    配置文件
    1、配置文件说明
    文件名：vehicledetectorconfig.ini
    文件路径：与动态库同级目录。
    配置文件可定义多个相机的参数，可自由开启指定数量的相机

    2、配置文件内容
    //主车道车牌识别器数量设置
    [VEHICLEDETECTOR]
    NUM = 1

    //车牌识别器 1
    [DETECTOR1]
    LaneID = 1
    IP = 192.168.1.1
    User = admin
    Password = 12345

    //车牌识别器 2
    [DETECTOR2]
    LaneID = 2
    IP = 192.168.1.2
    User = admin
    Password = 12345

    //车牌识别器 3
    [DETECTOR3]
    LaneID = 3
    IP = 192.168.1.3
    User = admin
    Password = 12345
*/

