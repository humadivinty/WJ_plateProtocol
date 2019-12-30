// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� WJSWDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// WJSWDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef WJSWDLL_EXPORTS
#define WJSWDLL_API EXTERN_C  __declspec(dllexport)
#else
#define WJSWDLL_API EXTERN_C  __declspec(dllimport)
#endif

#define  DELSPEC __stdcall

//************************************
// Method:        WVS_Settime
// Describe:        Уʱ������������λ��ʱ������Уʱ���ں����ڻ�ȡϵͳ��ǰʱ�䣩��
// FullName:      WVS_Settime
// Access:          public 
// Returns:        int
// Returns Describe:        0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
// Parameter:    int LaneNo ;  ���������
//************************************
WJSWDLL_API int DELSPEC WVS_Settime(int LaneNo);

//************************************
// Method:        WVS_Initialize
// Describe:        ���������ļ����ж������ĳ�ʼ���������������ݣ� 
//                       ��ʼ�� SDK����ȡ�����ļ�����½�����ע��ص������Уʱ
// FullName:      WVS_Initialize
// Access:          public 
// Returns:        int
// Returns Describe:        0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
//************************************
WJSWDLL_API int DELSPEC  WVS_Initialize();

//************************************
// Method:        WVS_CloseHv
// Describe:        ����˳����� 
// FullName:      WVS_CloseHv
// Access:          public 
// Returns:        int
// Returns Describe:        0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
//************************************
WJSWDLL_API int DELSPEC  WVS_CloseHv();

//************************************
// Method:        WVS_GetBigImage
// Describe:        ��ȡ����ͼ 
// FullName:      WVS_GetBigImage
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    unsigned char LaneNo    ;�����ţ�Ĭ�� 1  ������������� 
// Parameter:    int IdentNo                    ;��ţ�Ĭ�� 0������ 
// Parameter:    char * ImgBuf                 ;�洢ͼƬ����ĳ��� 
// Parameter:    int ImgBufLen                 ;�洢ͼƬ 
// Parameter:    int * ImgSize                   ;��¼ͼƬ��ʵ�ʴ�С 
//************************************
WJSWDLL_API int DELSPEC   WVS_GetBigImage(unsigned char  LaneNo, int  IdentNo, char*  ImgBuf, int  ImgBufLen, int*  ImgSize);

//************************************
// Method:        WVS_GetFarBigImage
// Describe:        ��ȡԶ��ͼ 
// FullName:      WVS_GetFarBigImage
// Access:          public 
// Returns:        int
// Returns Describe:                                    ;0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
// Parameter:    unsigned char LaneNo       ;�����ţ�Ĭ�� 1  ������������� 
// Parameter:    int IdentNo                        ;��ţ�Ĭ�� 0������ 
// Parameter:    char * ImgBuf                      ;�洢ͼƬ����ĳ���
// Parameter:    int ImgBufLen                      ;�洢ͼƬ 
// Parameter:    int * ImgSize                        ;��¼ͼƬ��ʵ�ʴ�С 
//************************************
WJSWDLL_API int DELSPEC   WVS_GetFarBigImage(unsigned char  LaneNo, int  IdentNo, char*  ImgBuf, int  ImgBufLen, int* ImgSize);

//************************************
// Method:        WVS_GetSmallImage
// Describe:        ��ȡ����ͼ
// FullName:      WVS_GetSmallImage
// Access:          public 
// Returns:        int
// Returns Describe:                                    ;0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
// Parameter:    unsigned char LaneNo       ;�����ţ�Ĭ�� 1  ������������� 
// Parameter:    int IdentNo                        ;��ţ�Ĭ�� 0������ 
// Parameter:    char * ImgBuf                    ;�洢ͼƬ����ĳ��� 
// Parameter:    int ImgBufLen                    ;�洢ͼƬ 
// Parameter:    int * ImgSize                      ;��¼ͼƬ��ʵ�ʴ�С 
//************************************
WJSWDLL_API int DELSPEC   WVS_GetSmallImage(unsigned char  LaneNo, int  IdentNo, char*  ImgBuf, int  ImgBufLen, int* ImgSize);

//************************************
// Method:        WVS_GetPlateNo
// Describe:        ��ȡ���ƺ� 
// FullName:      WVS_GetPlateNo
// Access:          public 
// Returns:        int
// Returns Describe:                                    ;0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
// Parameter:    unsigned char LaneNo       ;�����ţ�Ĭ�� 1  �������������
// Parameter:    int IdentNo                        ;��ţ�Ĭ�� 0������ 
// Parameter:    char * PlateNo                   ;�洢���ƺţ�����Ϊ������ɫ
// Parameter:    int PlateNoLen                   ;�洢ͼƬ����ĳ��� 
//************************************
WJSWDLL_API int DELSPEC  WVS_GetPlateNo(unsigned char LaneNo, int IdentNo, char* PlateNo, int PlateNoLen);

//************************************
// Method:        WVS_ForceSendLaneHv
// Describe:        �������ץ�� 
// FullName:      WVS_ForceSendLaneHv
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    int LaneNo                 ;�����ţ�Ĭ�� 1  ������������� 
// Parameter:    int ImageOutType       ;���ͼƬ���ͣ�Ĭ�� 4������ 
//************************************
WJSWDLL_API int DELSPEC  WVS_ForceSendLaneHv(int LaneNo, int ImageOutType);

//************************************
// Method:        WVS_StartRealPlay
// Describe:       ����ʵʱ��Ƶ 
// FullName:      WVS_StartRealPlay
// Access:          public 
// Returns:        int
// Returns Describe:                                ;0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
// Parameter:    int LaneNo                     ;�����ţ�Ĭ�� 1  �������������
// Parameter:    HWND FormHwnd         ;C#���Ŵ��ڡ��ؼ���� 
//************************************
WJSWDLL_API int DELSPEC  WVS_StartRealPlay(int LaneNo, HWND FormHwnd);

//************************************
// Method:        WVS_StopRealPlay
// Describe:        ֹͣ������Ƶ���ͷ���Դ 
// FullName:      WVS_StopRealPlay
// Access:          public 
// Returns:        int
// Returns Describe:                 ;0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
// Parameter:    int LaneNo      ;�����ţ�Ĭ�� 1  �������������
//************************************
WJSWDLL_API int DELSPEC  WVS_StopRealPlay(int LaneNo);

//************************************
// Method:        WVS_Startrecord
// Describe:       ��Ƶ¼���ܣ��ù���������Ƶ��Ԥ�����ܺ󣬲ſɿ��������ô˺���ʱ��ʼ¼�ƶ���Ƶ������ֹͣ¼��ʱ���ļ������ 
// FullName:      WVS_Startrecord
// Access:          public 
// Returns:        int
// Returns Describe:                             ;0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־
// Parameter:    int LaneNO                 ;�����ţ�Ĭ�� 1  �������������
// Parameter:    char * sFileName         ;��Ƶ�ļ�����·�� 
//************************************
WJSWDLL_API int DELSPEC WVS_Startrecord(int LaneNO, char *sFileName);

//************************************
// Method:        WVS_StopRealPlay
// Describe:        ֹͣ��Ƶ¼�񣬵���ֹͣ¼��ʱ���ļ������ 
// FullName:      WVS_StopRealPlay
// Access:          public 
// Returns:        int
// Returns Describe:                        ;0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־�� 
// Parameter:    int LaneNo             ;�����ţ�Ĭ�� 1  ������������� 
//************************************
WJSWDLL_API int DELSPEC WVS_Stoprecord(int LaneNo);

//************************************
// Method:        WVS_GetHvIsConnected
// Describe:        ������������Ƿ������� 
// FullName:      WVS_GetHvIsConnected
// Access:          public 
// Returns:        int
// Returns Describe:                ;0  �ɹ���-1 ʧ�ܣ�ʧ��ʱ��¼������־�� 
// Parameter:    int LaneNo     ;�����ţ�Ĭ�� 1  ������������� 
//************************************
WJSWDLL_API int DELSPEC WVS_GetHvIsConnected(int LaneNo);

/*
    �����ļ�
    1�������ļ�˵��
    �ļ�����vehicledetectorconfig.ini
    �ļ�·�����붯̬��ͬ��Ŀ¼��
    �����ļ��ɶ���������Ĳ����������ɿ���ָ�����������

    2�������ļ�����
    //����������ʶ������������
    [VEHICLEDETECTOR]
    NUM = 1

    //����ʶ���� 1
    [DETECTOR1]
    LaneID = 1
    IP = 192.168.1.1
    User = admin
    Password = 12345

    //����ʶ���� 2
    [DETECTOR2]
    LaneID = 2
    IP = 192.168.1.2
    User = admin
    Password = 12345

    //����ʶ���� 3
    [DETECTOR3]
    LaneID = 3
    IP = 192.168.1.3
    User = admin
    Password = 12345
*/

