#include "colors.h"

namespace cmd_col {
    void ColoredText(WORD wAttributes)
    {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hCon, wAttributes);
    }
}