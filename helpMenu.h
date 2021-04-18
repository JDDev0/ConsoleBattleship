#ifndef HELP_MENU_H
#define HELP_MENU_H
    extern struct tableOfContents content;
    extern int helpPage;
    extern const int maxHelpPage;

    void initHelpMenu(int width, int height);

    void drawHelp(void);
#endif
