#ifndef lib_H
#define lib_H

#include <windows.h>
#include <stdio.h>

#define MAX_INT 65536

__declspec(dllexport) typedef struct tagFileSettings {
    char* szData;                               // Массив символов, в котором хранится содержание файла
    int* iIndent;                               // Массив разделителей строк
    size_t iMaxLength;                          // Длина максимальной строки
    size_t iStrNum;                             // Количество сторок
    BOOL fest;                                  // Режим работы: 0 - без вёрстки, 1 - с вёрсткой
    char szFileName[_MAX_PATH];                 // Полное имя текущего файла
    char szTitleName[_MAX_FNAME + _MAX_EXT];    // Имя текущего файла
    OPENFILENAME* ofn;                          // Структура для работы со стандартным диалоговым окном Open
} FileSettings;

__declspec(dllexport) typedef struct tagViewer {
    int cxChar;                                 // Ширина символа
    int cyChar;                                 // Высота символа
    int iVScrlCurPos;                           // Положение вертикального ползунка
    int iVScrlMaxPos;                           // Максимальное положение вертикального ползунка
    int iHScrlCurPos;                           // Положение горизонтального ползунка
    int iHScrlMaxPos;                           // Максимальное положение горизонтального ползунка
    size_t cySize;                              // Высота символа
    size_t cxSize;                              // Ширина символа
    int iIncSize;                               // Число строк, на которое происходит сдвиг за смещение ползунка на одну позицию
    int iScrlInc;                               // Счётчик того, на сколько сдвинулся ползунок
    int iRealScrlInc;                           // Cчётчик того, на сколько строк сдвинулся текст на экране
    char* pstrFirstStr;                         // Начало верхней строки на экране (для режима с вёрсткой)
    int iFirstStrIndex;                         // Индекс(без учёта вёрстки) верхней строки на экране  (для режима с вёрсткой)
    int iFirstStrLength;                        // Длина верхней подстроки(та часть строки с индексом iFirstStrIndex, что сейчас на экране) на экране  (для режима с вёрсткой)
} Viewer;

/**
*@brief Делит x на у с округлением в большую сторону и результат не может быть нулём
*
*@param x Делимое
*@param y Делитель
*@return Результат деления x на y
*/
__declspec(dllexport) int div1(int x, int y);

/**
*@brief Делит x на у с округлением в большую сторону
*
*@param x Делимое
*@param y Делитель
*@return Результат деления x на y
*/
__declspec(dllexport) int div2(int x, int y);

/**
*@brief Вычисляет насколько строк нужно сдвинуть текст до начального положения(нулевого положения ползунка)
*
*@param fs Параметры файла(Model)
*@param vw Параметры для отображения текста(Viewer)
*@return Количество строк для сдвига
*/
__declspec(dllexport) size_t ShiftUp(FileSettings* fs, Viewer* vw);

/**
*@breif Вычисляет новые значения для верхней на экране подстроки после изменения положения ползунка  (для режима с вёрсткой)
*
*@param fs Параметры файла(Model)
*@param vw Параметры для отображения текста(Viewer)
*/
__declspec(dllexport) size_t MaxStrNum(FileSettings* fs, Viewer* vw);

/**
*@breif Вычисляет новые значения для верхней на экране подстроки после изменения положения ползунка  (для режима с вёрсткой)
*
*@param fs Параметры файла(Model)
*@param vw Параметры для отображения текста(Viewer)
*/
__declspec(dllexport) void FindNewFirstStr(Viewer* vw, FileSettings* fs);

/**
*@brief Вычисляет позицию в буфере, с которой отрисовывать текст (после изменения положения ползунка), индекс соответствующий ей строки(без учёта вёрстки), и длину оставшейся подстроки
*
*@param vw Параметры для отображения текста(Viewer)
*@param fs Параметры файла(Model)
*@param iInc Номер строки на экране, с которого нужно перерисовывать
*@param iStrLengthP Индекс соответствующий найденной позиции строки(без учёта вёрстки)
*@param iCurIndex Длину оставшейся подстроки
*@return Позиция в буфере, с которой отрисовывать текст
*/
__declspec(dllexport) PSTR FindStrToPaint(Viewer* vw, FileSettings* fs, int iInc, int* iStrLengthP, int* iCurIndex);

/**
*@brief Инициализирует структруру параметров файла(Model)
*
*@param fs Указатель на экземпляр структуры, который нужно инициализировать
*@param szMem Буфер, в который записано содержимое файла
*@param iFileSize Размер этого буфера
*/
__declspec(dllexport) void CreateFS(FileSettings* fs, char* szMem, int iFileSize);

/**
*@brief Инициализирует структуру параметров для отображения текста(Viewer)
*
*@param vw Указатель на экземпляр структуры, который нужно инициализировать
*@param tm Метрики текста
*/
__declspec(dllexport) void CreateViewer(Viewer* vw, TEXTMETRIC* tm);

/**
*@brief Обнуляет поля структуры Viewer
*
*@param vw Указатель на экземпляр структуры, поля которой нужно обнулить
*@param fs Параметры файла(Model)
*/
__declspec(dllexport) void UpdateViewer(Viewer* vw, FileSettings* fs);

/**
*@brief Вычисляет положение ползунка при переходе на режим с вёрсткой
*
*@param fs Параметры файла(Model)
*@param vw Параметры для отображения текста(Viewer)
*@return Текущее значение ползунка
*/
__declspec(dllexport) int GetCurVScrlPos(FileSettings* fs, Viewer* vw);

/**
*@brief Удаляет поля экземпляра структуры Model, память на которые была выделена на куче
*
*@param fs Указатель на экземпляр структуры, соответстующие поля которого нужно удалить
*/
__declspec(dllexport) void DestroyFS(FileSettings* fs);

/**
*@brief Считывает данные из файла и заполняет поля структуры Model
*
*@param filename имя файла
*@param fs Параметры файла(Model)
*@return TRUE, если удалось считать файл; FALSE, если была ошибка
*/
__declspec(dllexport) BOOL ReadFileData(char const* filename, FileSettings* fs);

/**
*@brief Инициализирует экземпляр структуры OPENFILENAME
*
*@param hwnd Описатель контекста устройства
*@param ofn Структура для работы со стандартным диалоговым окном Open
*/
__declspec(dllexport) void PopFileInitialize(HWND hwnd, OPENFILENAME* ofn);

/**
*@brief Cоздает стандартное диалоговое окно "Открыть", в случае успешного открытия файла присваевает новые имена для имен файла
*
*@param hwnd Описатель контекста устройства
*@param pstrFileName Полное имя текущего файла
*@param pstrTitleName Имя текущего файла
*@param ofn Структура для работы со стандартным диалоговым окном Open
*@return TRUE, если существует указанный файл; FALSE, если такого нет файла нет или произошла ошибка или диалоговое окно закрыли без выбора файла
*/
__declspec(dllexport) BOOL PopFileOpenDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName, OPENFILENAME* ofn);

#endif // lib_H
