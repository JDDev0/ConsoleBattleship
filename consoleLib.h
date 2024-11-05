#ifndef CONSOLE_LIB_H
#define CONSOLE_LIB_H
    #define CONSOLE_LIB_VER "v0.0.1"

    //Keys
    //Arrow keys
    #define CL_KEY_LEFT   5000
    #define CL_KEY_UP     5001
    #define CL_KEY_RIGHT  5002
    #define CL_KEY_DOWN   5003
    //F keys
    #define CL_KEY_F1     5004
    #define CL_KEY_F2     5005
    #define CL_KEY_F3     5006
    #define CL_KEY_F4     5007
    #define CL_KEY_F5     5008
    #define CL_KEY_F6     5009
    #define CL_KEY_F7     5010
    #define CL_KEY_F8     5011
    #define CL_KEY_F9     5012
    #define CL_KEY_F10    5013
    #define CL_KEY_F11    5014
    #define CL_KEY_F12    5015
    //Other keys
    #define CL_KEY_ESC    5016
    #define CL_KEY_DELETE 5017
    #define CL_KEY_ENTER  5018

    void clrscr(void);

    void initConsole(void);
    void reset(void);

    void getConsoleSize(int *columnsRet, int *rowsRet);

    int hasInput(void);
    int getKey(void);
    int isArrowKey(int key);

    void getMousePosClicked(int *column, int *row);

    void sleepMS(unsigned int time);

    void drawf(const char *restrict format, ...);

    enum consoleLibColor {
        CL_COLOR_BLACK, CL_COLOR_BLUE, CL_COLOR_GREEN, CL_COLOR_CYAN, CL_COLOR_RED,
        CL_COLOR_PINK, CL_COLOR_YELLOW, CL_COLOR_WHITE, CL_COLOR_LIGHT_BLACK,
        CL_COLOR_LIGHT_BLUE, CL_COLOR_LIGHT_GREEN, CL_COLOR_LIGHT_CYAN,
        CL_COLOR_LIGHT_RED, CL_COLOR_LIGHT_PINK, CL_COLOR_LIGHT_YELLOW,
        CL_COLOR_LIGHT_WHITE, CL_COLOR_NO_COLOR = -1
    };
    //fg == -1: no foreground color change, bg == -1: no background color change
    void setColor(int fg, int bg);
    void resetColor(void);

    void setUnderline(int underline);

    void setCursorPos(int x, int y);
#endif
