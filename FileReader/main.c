#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <commdlg.h>
#include "menu.h"
#include "lib.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "MyMenu";                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           640,       /* The programs width */
           476,       /* and height in pixels */
           NULL,                /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument         /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow (hwnd);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    TEXTMETRIC tm;
    CREATESTRUCT* hSt = (CREATESTRUCT*) lParam;

    int delta;

    static FileSettings fs;
    static Viewer vw;

    int iScrlInc = 0;
    int iFirstStr, iLastStr;

    int i, j, x, y;
    int iStrNum;
    char* pCurStrPos;
    int iCurStrSize, iCurStr;

    DWORD lPar = 0L;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            ReadFileData(hSt->lpCreateParams, &fs);

            hdc = GetDC(hwnd);
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            GetTextMetrics(hdc, &tm);
            ReleaseDC(hwnd, hdc);

            CreateViewer(&vw, &tm);
            UpdateViewer(&vw, &fs);
            fs.ofn = (OPENFILENAME*)malloc(sizeof(OPENFILENAME));
            PopFileInitialize(hwnd, fs.ofn);
            CheckMenuItem(GetMenu(hwnd), IDM_STYLE, MF_UNCHECKED);

            return 0;

        case WM_SIZE:
            x = vw.cxSize;
            vw.cxSize = LOWORD(lParam);

            if(fs.fest == 0)
            {
                vw.iHScrlMaxPos = fs.iMaxLength - vw.cxSize / vw.cxChar;
                if(vw.iHScrlMaxPos < 0)
                {
                    vw.iHScrlMaxPos = 0;
                }
                vw.iHScrlCurPos = min(vw.iHScrlCurPos, vw.iHScrlMaxPos);
            }
            else
            {
                vw.iHScrlMaxPos = 0;
                vw.iHScrlCurPos = 0;
            }

            SetScrollRange(hwnd, SB_HORZ, 0, vw.iHScrlMaxPos, FALSE);
            SetScrollPos(hwnd, SB_HORZ, vw.iHScrlCurPos, TRUE);


            y = vw.cySize;
            vw.cySize = HIWORD(lParam);

            vw.iVScrlMaxPos = MaxStrNum(&fs, &vw) - div2(vw.cySize, vw.cyChar) + 1;

            i = vw.iVScrlMaxPos;
            vw.iIncSize = vw.iVScrlMaxPos / MAX_INT + 1;

            vw.iVScrlMaxPos = div2(vw.iVScrlMaxPos, vw.iIncSize);

            if(vw.iVScrlMaxPos < 0)
            {
                vw.iVScrlMaxPos = 0;
            }

            vw.iVScrlCurPos = min(vw.iVScrlCurPos, vw.iVScrlMaxPos);

            if(fs.fest)
            {
                vw.iVScrlCurPos = GetCurVScrlPos(&fs, &vw);
                if(vw.iVScrlCurPos > vw.iVScrlMaxPos)
                {
                    SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
                }
            }

            SetScrollRange(hwnd, SB_VERT, 0, vw.iVScrlMaxPos, FALSE);
            SetScrollPos(hwnd, SB_VERT, vw.iVScrlCurPos, TRUE);

            if(fs.fest && vw.iVScrlCurPos == vw.iVScrlMaxPos) {
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
            }

            UpdateWindow(hwnd);
            return 0;

        case WM_VSCROLL:
            switch(LOWORD(wParam))
            {
            case SB_LINEUP:
                --iScrlInc;
                break;
            case SB_LINEDOWN:
                ++iScrlInc;
                break;
            case SB_PAGEUP:
                iScrlInc -= (vw.cySize / vw.cyChar);
                break;
            case SB_PAGEDOWN:
                iScrlInc += (vw.cySize / vw.cyChar);
                break;
            case SB_THUMBTRACK:
                iScrlInc = (HIWORD(wParam) - vw.iVScrlCurPos);
                break;
            }

            if(iScrlInc > vw.iVScrlMaxPos - vw.iVScrlCurPos)
            {
                iScrlInc = vw.iVScrlMaxPos - vw.iVScrlCurPos;
            }
            if(iScrlInc < -vw.iVScrlCurPos)
            {
                iScrlInc = -vw.iVScrlCurPos;
            }


            if(iScrlInc)
            {
                vw.iScrlInc = iScrlInc;
                vw.iRealScrlInc = (-iScrlInc) * vw.iIncSize;

                vw.iVScrlCurPos += iScrlInc;

                if(vw.iVScrlCurPos == 0  && fs.fest)
                {
                    vw.iRealScrlInc = ShiftUp(&fs, &vw);
                }

                ScrollWindow(hwnd, 0, vw.iRealScrlInc * vw.cyChar, NULL, NULL);
                SetScrollPos(hwnd, SB_VERT, vw.iVScrlCurPos, TRUE);

                if(vw.iVScrlCurPos == vw.iVScrlMaxPos && fs.fest)
                {
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                UpdateWindow(hwnd);
            }

            return 0;

        case WM_HSCROLL:
            switch(LOWORD(wParam))
            {
            case SB_LINEUP:
                    --iScrlInc;
                    break;
            case SB_LINEDOWN:
                    ++iScrlInc;
                    break;
            case SB_PAGEUP:
                    iScrlInc -= vw.cxSize / vw.cxChar;
                    break;
            case SB_PAGEDOWN:
                    iScrlInc += vw.cxSize / vw.cxChar;
                    break;
            case SB_THUMBTRACK:
                    iScrlInc = HIWORD(wParam) - vw.iHScrlCurPos;
                    break;
            }

            if(iScrlInc > vw.iHScrlMaxPos - vw.iHScrlCurPos)
            {
                iScrlInc = vw.iHScrlMaxPos - vw.iHScrlCurPos;
            }
            if(iScrlInc < -vw.iHScrlCurPos)
            {
                iScrlInc = -vw.iHScrlCurPos;
            }

            if(iScrlInc)
            {
                vw.iHScrlCurPos += iScrlInc;
                ScrollWindow(hwnd, -iScrlInc * vw.cxChar, 0 , NULL, NULL);
                SetScrollPos(hwnd, SB_HORZ, vw.iHScrlCurPos, TRUE);
                UpdateWindow(hwnd);
            }

            return 0;

        case WM_KEYDOWN:
            switch(wParam)
            {
            case VK_UP:
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
                return 0;
            case VK_DOWN:
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
                return 0;
            case VK_PRIOR:
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0L);
                return 0;
            case VK_NEXT:
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
                return 0;
            case VK_RIGHT:
                SendMessage(hwnd, WM_HSCROLL, SB_LINEDOWN, 0L);
                return 0;
            case VK_LEFT:
                SendMessage(hwnd, WM_HSCROLL, SB_LINEUP, 0L);
                return 0;
            }
            return 0;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
            case IDM_OPEN:
                if(PopFileOpenDlg(hwnd, fs.szFileName, fs.szTitleName, fs.ofn))
                {
                    if(ReadFileData(fs.szFileName, &fs))
                    {
                        UpdateViewer(&vw, &fs);
                        fs.fest = 0;
                        lPar = ((WORD)vw.cySize<<16) | (WORD)vw.cxSize;
                        SendMessage(hwnd, WM_SIZE, 0, lPar);
                        InvalidateRect(hwnd, NULL, TRUE);
                        CheckMenuItem(GetMenu(hwnd), IDM_STYLE, MF_UNCHECKED);
                    }
                }
                return 0;
            case IDM_CLOSE:
                DestroyFS(&fs);
                fs.fest = 0;
                fs.iMaxLength = 0;
                fs.iStrNum = 0;
                fs.iIndent = NULL;
                fs.szData = NULL;
                SetScrollRange(hwnd, SB_HORZ, 0, 0, TRUE);
                SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            case IDM_EXIT:
                SendMessage(hwnd, WM_CLOSE, 0, 0L);
                return 0;
            case IDM_STYLE:
                if(fs.szData)
                {
                    fs.fest = fs.fest ? 0 : 1;
                    if(fs.fest)
                    {
                        vw.iFirstStrIndex = vw.iVScrlCurPos * vw.iIncSize;
                        vw.pstrFirstStr = fs.szData + fs.iIndent[vw.iFirstStrIndex] + 1;
                        vw.iFirstStrLength = fs.iIndent[vw.iFirstStrIndex + 1] - fs.iIndent[vw.iFirstStrIndex];
                        vw.iScrlInc = 0;
                        vw.iVScrlCurPos = GetCurVScrlPos(&fs, &vw);
                        SetScrollRange(hwnd, SB_HORZ, 0, 0, FALSE);
                        CheckMenuItem(GetMenu(hwnd), IDM_STYLE, MF_CHECKED);
                    }
                    else
                    {
                        vw.iVScrlMaxPos = fs.iStrNum - vw.cySize / vw.cyChar;
                        vw.iIncSize = vw.iVScrlMaxPos / MAX_INT + 1;
                        SetScrollRange(hwnd, SB_HORZ, 0, 1, FALSE);
                        vw.iVScrlCurPos = vw.iFirstStrIndex / vw.iIncSize;
                        CheckMenuItem(GetMenu(hwnd), IDM_STYLE, MF_UNCHECKED);
                        SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
                        SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
                    }
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                return 0;
            }
            return 0;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);
            iFirstStr = vw.iVScrlCurPos * vw.iIncSize + ps.rcPaint.top / vw.cyChar;
            iLastStr = vw.iVScrlCurPos * vw.iIncSize + ps.rcPaint.bottom / vw.cyChar + 1;
            if (!fs.fest)
            {

                if(iLastStr > fs.iStrNum)
                {
                    iLastStr = fs.iStrNum;
                }

                x = vw.cxChar * (- vw.iHScrlCurPos);
                for (i = iFirstStr; i < iLastStr; ++i)
                {
                    y = vw.cyChar * (- vw.iVScrlCurPos * vw.iIncSize + i);
                    TextOut(hdc, x, y, (char* )(fs.szData + fs.iIndent[i] + 1), fs.iIndent[i + 1] - fs.iIndent[i]);
                }
            }
            else
            {
                delta = iLastStr - iFirstStr;
                x = vw.cxChar * (- vw.iHScrlCurPos);
                y = ps.rcPaint.top;
                y = (y / vw.cyChar) * vw.cyChar;
                FindNewFirstStr(&vw, &fs);
                pCurStrPos = FindStrToPaint(&vw, &fs, y / vw.cyChar, &iCurStrSize, &iFirstStr);
                iStrNum = div2(iCurStrSize, div2(vw.cxSize, vw.cxChar));
                for(j = 0, iCurStr = 0; j < delta && iFirstStr + iCurStr < fs.iStrNum; ++iCurStr)
                {
                    if(j > 0)
                    {
                        iCurStrSize = fs.iIndent[iFirstStr + iCurStr + 1] - fs.iIndent[iFirstStr + iCurStr];
                        iStrNum = div2(fs.iIndent[iFirstStr + iCurStr + 1] - fs.iIndent[iFirstStr + iCurStr], vw.cxSize / vw.cxChar + 1);
                    }
                    for(i = 0; i < iStrNum; ++i)
                    {
                        TextOut(hdc, x, y, pCurStrPos, min(iCurStrSize, vw.cxSize / vw.cxChar + 1));

                        y += vw.cyChar;
                        pCurStrPos += min(iCurStrSize, vw.cxSize / vw.cxChar + 1);
                        iCurStrSize = max(0, iCurStrSize - (vw.cxSize / vw.cxChar + 1));
                        ++j;
                        if(j == delta || iFirstStr + iCurStr >= fs.iStrNum)
                        {
                            break;
                        }
                    }
                }
            }
            EndPaint(hwnd, &ps);
            vw.iScrlInc = 0;
            return 0;

        case WM_DESTROY:
            DestroyFS(&fs);
            free(fs.ofn);
            PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
            break;

        default:                       /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
