#ifndef lib_H
#define lib_H

#include <windows.h>
#include <stdio.h>

#define MAX_INT 65536

__declspec(dllexport) typedef struct tagFileSettings {
    char* szData;                               // ������ ��������, � ������� �������� ���������� �����
    int* iIndent;                               // ������ ������������ �����
    size_t iMaxLength;                          // ����� ������������ ������
    size_t iStrNum;                             // ���������� ������
    BOOL fest;                                  // ����� ������: 0 - ��� ������, 1 - � �������
    char szFileName[_MAX_PATH];                 // ������ ��� �������� �����
    char szTitleName[_MAX_FNAME + _MAX_EXT];    // ��� �������� �����
    OPENFILENAME* ofn;                          // ��������� ��� ������ �� ����������� ���������� ����� Open
} FileSettings;

__declspec(dllexport) typedef struct tagViewer {
    int cxChar;                                 // ������ �������
    int cyChar;                                 // ������ �������
    int iVScrlCurPos;                           // ��������� ������������� ��������
    int iVScrlMaxPos;                           // ������������ ��������� ������������� ��������
    int iHScrlCurPos;                           // ��������� ��������������� ��������
    int iHScrlMaxPos;                           // ������������ ��������� ��������������� ��������
    size_t cySize;                              // ������ �������
    size_t cxSize;                              // ������ �������
    int iIncSize;                               // ����� �����, �� ������� ���������� ����� �� �������� �������� �� ���� �������
    int iScrlInc;                               // ������� ����, �� ������� ��������� ��������
    int iRealScrlInc;                           // C������ ����, �� ������� ����� ��������� ����� �� ������
    char* pstrFirstStr;                         // ������ ������� ������ �� ������ (��� ������ � �������)
    int iFirstStrIndex;                         // ������(��� ����� ������) ������� ������ �� ������  (��� ������ � �������)
    int iFirstStrLength;                        // ����� ������� ���������(�� ����� ������ � �������� iFirstStrIndex, ��� ������ �� ������) �� ������  (��� ������ � �������)
} Viewer;

/**
*@brief ����� x �� � � ����������� � ������� ������� � ��������� �� ����� ���� ����
*
*@param x �������
*@param y ��������
*@return ��������� ������� x �� y
*/
__declspec(dllexport) int div1(int x, int y);

/**
*@brief ����� x �� � � ����������� � ������� �������
*
*@param x �������
*@param y ��������
*@return ��������� ������� x �� y
*/
__declspec(dllexport) int div2(int x, int y);

/**
*@brief ��������� ��������� ����� ����� �������� ����� �� ���������� ���������(�������� ��������� ��������)
*
*@param fs ��������� �����(Model)
*@param vw ��������� ��� ����������� ������(Viewer)
*@return ���������� ����� ��� ������
*/
__declspec(dllexport) size_t ShiftUp(FileSettings* fs, Viewer* vw);

/**
*@breif ��������� ����� �������� ��� ������� �� ������ ��������� ����� ��������� ��������� ��������  (��� ������ � �������)
*
*@param fs ��������� �����(Model)
*@param vw ��������� ��� ����������� ������(Viewer)
*/
__declspec(dllexport) size_t MaxStrNum(FileSettings* fs, Viewer* vw);

/**
*@breif ��������� ����� �������� ��� ������� �� ������ ��������� ����� ��������� ��������� ��������  (��� ������ � �������)
*
*@param fs ��������� �����(Model)
*@param vw ��������� ��� ����������� ������(Viewer)
*/
__declspec(dllexport) void FindNewFirstStr(Viewer* vw, FileSettings* fs);

/**
*@brief ��������� ������� � ������, � ������� ������������ ����� (����� ��������� ��������� ��������), ������ ��������������� �� ������(��� ����� ������), � ����� ���������� ���������
*
*@param vw ��������� ��� ����������� ������(Viewer)
*@param fs ��������� �����(Model)
*@param iInc ����� ������ �� ������, � �������� ����� ��������������
*@param iStrLengthP ������ ��������������� ��������� ������� ������(��� ����� ������)
*@param iCurIndex ����� ���������� ���������
*@return ������� � ������, � ������� ������������ �����
*/
__declspec(dllexport) PSTR FindStrToPaint(Viewer* vw, FileSettings* fs, int iInc, int* iStrLengthP, int* iCurIndex);

/**
*@brief �������������� ���������� ���������� �����(Model)
*
*@param fs ��������� �� ��������� ���������, ������� ����� ����������������
*@param szMem �����, � ������� �������� ���������� �����
*@param iFileSize ������ ����� ������
*/
__declspec(dllexport) void CreateFS(FileSettings* fs, char* szMem, int iFileSize);

/**
*@brief �������������� ��������� ���������� ��� ����������� ������(Viewer)
*
*@param vw ��������� �� ��������� ���������, ������� ����� ����������������
*@param tm ������� ������
*/
__declspec(dllexport) void CreateViewer(Viewer* vw, TEXTMETRIC* tm);

/**
*@brief �������� ���� ��������� Viewer
*
*@param vw ��������� �� ��������� ���������, ���� ������� ����� ��������
*@param fs ��������� �����(Model)
*/
__declspec(dllexport) void UpdateViewer(Viewer* vw, FileSettings* fs);

/**
*@brief ��������� ��������� �������� ��� �������� �� ����� � �������
*
*@param fs ��������� �����(Model)
*@param vw ��������� ��� ����������� ������(Viewer)
*@return ������� �������� ��������
*/
__declspec(dllexport) int GetCurVScrlPos(FileSettings* fs, Viewer* vw);

/**
*@brief ������� ���� ���������� ��������� Model, ������ �� ������� ���� �������� �� ����
*
*@param fs ��������� �� ��������� ���������, �������������� ���� �������� ����� �������
*/
__declspec(dllexport) void DestroyFS(FileSettings* fs);

/**
*@brief ��������� ������ �� ����� � ��������� ���� ��������� Model
*
*@param filename ��� �����
*@param fs ��������� �����(Model)
*@return TRUE, ���� ������� ������� ����; FALSE, ���� ���� ������
*/
__declspec(dllexport) BOOL ReadFileData(char const* filename, FileSettings* fs);

/**
*@brief �������������� ��������� ��������� OPENFILENAME
*
*@param hwnd ��������� ��������� ����������
*@param ofn ��������� ��� ������ �� ����������� ���������� ����� Open
*/
__declspec(dllexport) void PopFileInitialize(HWND hwnd, OPENFILENAME* ofn);

/**
*@brief C������ ����������� ���������� ���� "�������", � ������ ��������� �������� ����� ����������� ����� ����� ��� ���� �����
*
*@param hwnd ��������� ��������� ����������
*@param pstrFileName ������ ��� �������� �����
*@param pstrTitleName ��� �������� �����
*@param ofn ��������� ��� ������ �� ����������� ���������� ����� Open
*@return TRUE, ���� ���������� ��������� ����; FALSE, ���� ������ ��� ����� ��� ��� ��������� ������ ��� ���������� ���� ������� ��� ������ �����
*/
__declspec(dllexport) BOOL PopFileOpenDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName, OPENFILENAME* ofn);

#endif // lib_H
