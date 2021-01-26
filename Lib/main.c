#include "lib.h"

int div1(int x, int y)
{
    int res = x / y;
    if(res * y != x || res == 0) {
        ++res;
    }
    return res;
}

int div2(int x, int y)
{
    int res = x / y;
    if(x % y == 0)
    {
        return res;
    }
    return res + 1;
}

size_t ShiftUp(FileSettings* fs, Viewer* vw)
{
    int i;
    size_t mvsp = 0;
    int div = div1(vw->cxSize, vw->cxChar);
    for(i = 0; i < vw->iFirstStrIndex; ++i)
    {
        mvsp += div2((fs->iIndent[i + 1] - fs->iIndent[i]), div);
    }
    mvsp += div2(max(0, (vw->pstrFirstStr - (fs->szData + fs->iIndent[i] + 1))) , div);
    return mvsp;
}

size_t MaxStrNum(FileSettings* fs, Viewer* vw)
{
    int i;
    size_t mvsp = 0;
    if(fs->fest) {
        for(i = 0; i < fs->iStrNum - 1; ++i)
        {
            mvsp += div2((fs->iIndent[i + 1] - fs->iIndent[i]), vw->cxSize / vw->cxChar + 1);
        }
    }
    else {
        return fs->iStrNum;
    }
    return mvsp;
}

void FindNewFirstStr(Viewer* vw, FileSettings* fs)
{
    int iInc = abs(vw->iScrlInc) * vw->iIncSize;
    int delta = 0;
    int d = vw->cxSize / vw->cxChar + 1;

    iInc = abs(vw->iRealScrlInc);
    if(vw->iScrlInc > 0)
    {
        while(iInc > div2(vw->iFirstStrLength, d))
        {
            iInc -= div2(vw->iFirstStrLength, d);
            ++vw->iFirstStrIndex;
            vw->pstrFirstStr = fs->szData + fs->iIndent[vw->iFirstStrIndex] + 1;
            vw->iFirstStrLength = fs->iIndent[vw->iFirstStrIndex + 1] - fs->iIndent[vw->iFirstStrIndex];
        }
        vw->iScrlInc = 0;
        if(iInc)
        {
            vw->pstrFirstStr += min(d * iInc, vw->iFirstStrLength);
            vw->iFirstStrLength -= min(d * iInc, vw->iFirstStrLength);
            if(vw->iFirstStrLength == 0)
            {
                ++vw->iFirstStrIndex;
                vw->pstrFirstStr = fs->szData + fs->iIndent[vw->iFirstStrIndex] + 1;
                vw->iFirstStrLength = fs->iIndent[vw->iFirstStrIndex + 1] - fs->iIndent[vw->iFirstStrIndex];
            }

            if(vw->iVScrlCurPos == vw->iVScrlMaxPos)
            {
                vw->iFirstStrIndex = fs->iStrNum - 1;
                vw->pstrFirstStr = fs->szData + fs->iIndent[vw->iFirstStrIndex];
                vw->iFirstStrLength = fs->iIndent[vw->iFirstStrIndex + 1] - fs->iIndent[vw->iFirstStrIndex];
                vw->iScrlInc = -1;
                iInc = vw->cySize / vw->cyChar - div2(vw->iFirstStrLength, div2(vw->cxSize, vw->cxChar));
            }

        }
    }
    if(vw->iScrlInc < 0)
    {
        delta = (fs->iIndent[vw->iFirstStrIndex + 1] - fs->iIndent[vw->iFirstStrIndex]) - vw->iFirstStrLength;

        while(delta / div1(vw->cxSize, vw->cxChar) < iInc && vw->iFirstStrIndex > 0)
        {
            iInc -= div2(delta, div1(vw->cxSize, vw->cxChar));
            --vw->iFirstStrIndex;
            vw->pstrFirstStr = fs->szData + max(0, fs->iIndent[vw->iFirstStrIndex]);
            vw->iFirstStrLength = fs->iIndent[vw->iFirstStrIndex + 1] - fs->iIndent[vw->iFirstStrIndex];
            delta = (div2(vw->iFirstStrLength, div2(vw->cxSize, vw->cxChar)));
            delta *= div2(vw->cxSize, vw->cxChar);
            vw->pstrFirstStr += delta;
            vw->iFirstStrLength -= delta;
        }
        if(delta / div1(vw->cxSize, vw->cxChar) >= iInc)
        {
            vw->pstrFirstStr -= iInc * div1(vw->cxSize, vw->cxChar);
            vw->iFirstStrLength += iInc * div1(vw->cxSize, vw->cxChar);
        }
        else {
            vw->pstrFirstStr = fs->szData;
            vw->iFirstStrLength = fs->iIndent[1] - fs->iIndent[0];
        }

        if(vw->iVScrlCurPos == 0)
        {
            vw->iFirstStrIndex = 0;
            vw->iFirstStrLength = fs->iIndent[1] - fs->iIndent[0];
            vw->pstrFirstStr = fs->szData;
        }

    }
    vw->iScrlInc = 0;
}

PSTR FindStrToPaint(Viewer* vw, FileSettings* fs, int iInc, int* iStrLengthP, int* iCurIndex)
{
    PSTR pstr = vw->pstrFirstStr;
    int i = 0;
    int iStrLength = vw->iFirstStrLength;
    if(iInc > 0)
    {
        while(iInc > div2(iStrLength, vw->cxSize / vw->cxChar + 1))
        {
            iInc -= div2(iStrLength, vw->cxSize / vw->cxChar + 1);
            ++i;
            pstr = fs->szData + fs->iIndent[vw->iFirstStrIndex + i] + 1;
            iStrLength = fs->iIndent[vw->iFirstStrIndex + i + 1] - fs->iIndent[vw->iFirstStrIndex + i];
        }
        if(iInc)
        {
            pstr += min((vw->cxSize / vw->cxChar + 1) * iInc, iStrLength);
            iStrLength -= min((vw->cxSize / vw->cxChar + 1) * iInc, iStrLength);
            if(iStrLength == 0)
            {
                ++i;
                pstr = fs->szData + fs->iIndent[vw->iFirstStrIndex + i] + 1;
                iStrLength = fs->iIndent[vw->iFirstStrIndex + i + 1] - fs->iIndent[vw->iFirstStrIndex + i];
            }
        }
    }
    *iStrLengthP = iStrLength;
    *iCurIndex = vw->iFirstStrIndex + i;
    return pstr;
}

void CreateFS(FileSettings* fs, char* szMem, int iFileSize)
{
    size_t i, k;
    size_t maxLenght = 0;
    fs->szData = szMem;
    fs->iMaxLength = 0;
    fs->iStrNum = 1;
    fs->fest = 0;

    for (i = 0; i < iFileSize; ++i)
    {
        ++maxLenght;
        if (szMem[i] == '\n')
        {
            ++fs->iStrNum;
            if (maxLenght - 1 > fs->iMaxLength)
            {
                fs->iMaxLength = maxLenght - 1;
            }
            maxLenght = 0;
        }
    }
    fs->iIndent = malloc((fs->iStrNum + 1) * sizeof(size_t));
    fs->iIndent[0] = -1;
    for (i = 0, k = 1; i < iFileSize; ++i)
    {
        if (szMem[i] == '\n')
        {
            fs->iIndent[k++] = i;
        }
    }
    fs->iIndent[k++] = i;
}

void CreateViewer(Viewer* vw, TEXTMETRIC* tm)
{
    vw->cxChar = tm->tmAveCharWidth;
    vw->cyChar = tm->tmHeight + tm->tmExternalLeading;
    vw->iVScrlCurPos = 0;
    vw->iVScrlMaxPos = 0;
    vw->iScrlInc = 0;
}

void UpdateViewer(Viewer* vw, FileSettings* fs)
{
    vw->iVScrlCurPos = 0;
    vw->iVScrlMaxPos = 0;
    vw->iScrlInc = 0;
    vw->iHScrlCurPos = 0;
    vw->iHScrlMaxPos = 0;
    vw->pstrFirstStr = fs->szData;
    vw->iFirstStrIndex = 0;
    if(fs->iIndent)
    {
        vw->iFirstStrLength = fs->iIndent[1] - fs->iIndent[0];
    }
    else {
        vw->iFirstStrLength = 0;
    }
}

int GetCurVScrlPos(FileSettings* fs, Viewer* vw)
{
    int iCurVScrlPos = 0;
    int i = 0;
    for(i = 0; i < vw->iFirstStrIndex; ++i)
    {
        iCurVScrlPos += div2(fs->iIndent[i + 1] - fs->iIndent[i], (vw->cxSize / vw->cxChar + 1));
    }
    iCurVScrlPos += div2(fs->iIndent[vw->iFirstStrIndex + 1] - fs->iIndent[vw->iFirstStrIndex] - vw->iFirstStrLength, (vw->cxSize / vw->cxChar + 1));
    vw->iVScrlMaxPos = MaxStrNum(fs, vw) - vw->cySize / vw->cyChar;
    vw->iIncSize = vw->iVScrlMaxPos / MAX_INT + 1;
    vw->iVScrlMaxPos = div2(vw->iVScrlMaxPos, vw->iIncSize);
    iCurVScrlPos /= vw->iIncSize;

    return iCurVScrlPos;
}

void DestroyFS(FileSettings* fs)
{
    if(fs->iIndent)
    {
        free(fs->iIndent);
    }
    if(fs->szData)
    {
        free(fs->szData);
    }
}

BOOL ReadFileData(char const* filename, FileSettings* fs) {
    FILE* file = fopen(filename, "r");
    char* szMem = NULL;
    int iFileSize;
    lstrcpy(fs->szFileName, (PSTR)filename);
    GetFileTitle(fs->szFileName, fs->szTitleName, sizeof(fs->szTitleName));
    if(file) {
        fseek(file, 0, SEEK_END);
        iFileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        szMem = malloc(iFileSize * sizeof(char));
        if(szMem) {
            iFileSize = fread(szMem, sizeof(char), iFileSize, file);
            szMem[iFileSize] = 0;
            CreateFS(fs, szMem, iFileSize);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

void PopFileInitialize(HWND hwnd, OPENFILENAME* ofn)
{
    static char szFilter[] = "Text Files(*.TXT)\0*.txt\0" \
                             "All Files(*.*)\0*.*\0\0";

    ofn->lStructSize = sizeof(OPENFILENAME);
    ofn->hwndOwner = hwnd;
    ofn->hInstance = NULL;
    ofn->lpstrFilter = szFilter;
    ofn->lpstrCustomFilter = NULL;
    ofn->nMaxCustFilter = 0;
    ofn->nFilterIndex = 0;
    ofn->lpstrFile = NULL;
    ofn->nMaxFile = _MAX_PATH;
    ofn->lpstrFileTitle = NULL;
    ofn->nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
    ofn->lpstrInitialDir = NULL;
    ofn->lpstrTitle = NULL;
    ofn->Flags = 0;
    ofn->nFileOffset = 0;
    ofn->nFileExtension = 0;
    ofn->lpstrDefExt = "txt";
    ofn->lCustData = 0L;
    ofn->lpfnHook = NULL;
    ofn->lpTemplateName = NULL;
}

BOOL PopFileOpenDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName, OPENFILENAME* ofn)
{
     ofn->hwndOwner = hwnd;
     ofn->lpstrFile = pstrFileName;
     ofn->lpstrFileTitle = pstrTitleName;
     ofn->Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
     return GetOpenFileName(&(*ofn));
}
