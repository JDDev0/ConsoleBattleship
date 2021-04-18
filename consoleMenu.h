#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H
    #define CONSOLE_MENU_VER "v0.0.1"

    struct tableOfContents {
        int pages;
        int columns;
        int rows;
        int rowNow;
        int lastSection;
        int lastSubSection;
        int lastSubSubSection;
        char **chapterNames; //[pages][colums * rows]
    };

    struct menuContent {
        int xStart;
        int yStart;
        int columns;
        int rows;
        int pos;
        int fgColorNormal;
        int bgColorNormal;
        int fgColorSelected;
        int bgColorSelected;
        int entryCount;
        char **entryNames;
    };

    void fillKeyField(char field[], int sizeField, char empty, const char minMax[],
    int toUpper, int key);

    void initTableOfContents(struct tableOfContents *content, int columns, int rows);
    void freeTableOfContents(struct tableOfContents *content);

    void addSection(struct tableOfContents *content, char *name, int page);
    void addSubSection(struct tableOfContents *content, char *name, int page);
    void addSubSubSection(struct tableOfContents *content, char *name, int page);

    void drawContent(struct tableOfContents *content, int page);

    int getPageMouseClicked(struct tableOfContents *content, int page, int row);

    void initMenu(struct menuContent *content, char **entryNames, int entryCount,
    int xStart, int yStart, int columns, int rows, int fgColorNormal,
    int bgColorNormal, int fgColorSelected, int bgColorSelected);
    void drawMenu(struct menuContent *content);
    int updateKeyMenu(struct menuContent *content, int key);
    int updateMouseMenu(struct menuContent *content, int column, int row);
#endif
