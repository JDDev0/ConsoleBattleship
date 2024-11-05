#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "consoleMenu.h"
#include "consoleLib.h"

void fillKeyField(char field[], int sizeField, char empty, const char minMax[],
int toUpper, int key) {
    if(key == CL_KEY_DELETE) {
        for(int i = sizeField - 1;i > -1;i--) {
            if(field[i] != empty) {
                field[i] = empty;

                return;
            }
        }
    }

    for(int i = 0;i < sizeField;i++) {
        if(field[i] == empty) {
            if(key >= minMax[2*i] && key <= minMax[2*i + 1]) {
                if(toUpper) {
                    field[i] = (char)toupper(key);
                }else {
                    field[i] = (char)key;
                }
            }

            return;
        }
    }
}

void initTableOfContents(struct tableOfContents *content, int columns, int rows) {
    if(content == NULL) {
        return;
    }

    content->pages = 0;
    content->columns = columns + 1; //'\0'
    content->rows = rows;
    content->rowNow = 0;
    content->lastSection = content->lastSubSection = content->lastSubSubSection = 0;
    content->chapterNames = NULL;
}
void freeTableOfContents(struct tableOfContents *content) {
    if(content == NULL) {
        return;
    }

    if(content->chapterNames != NULL) {
        for(int i = 0;i < content->pages;i++) {
            if(content->chapterNames[i] != NULL) {
                free(content->chapterNames[i]);
            }
        }
        free(content->chapterNames);
    }
}

void addPageIfNeeded(struct tableOfContents *content) {
    if(content == NULL) {
        return;
    }

    if(content->rows-1 == content->rowNow) {
        content->rowNow = 0;
        content->pages++;
        content->chapterNames = realloc(content->chapterNames, sizeof(char *) *
        (size_t)content->pages);
        content->chapterNames[content->pages - 1] = malloc(sizeof(char) * (size_t)(
        content->columns * content->rows));

        return;
    }
    if(content->pages == 0) {
        content->pages++;
        content->chapterNames = malloc(sizeof(char *) * (size_t)content->pages);
        content->chapterNames[0] = malloc(sizeof(char) * (size_t)(content->columns *
        content->rows));
    }
}
void addSection(struct tableOfContents *content, char *name, int page) {
    if(content == NULL) {
        return;
    }

    content->lastSection++;
    content->lastSubSection = 0;
    content->lastSubSubSection = 0;

    addPageIfNeeded(content);

    //Get chapter row
    char *chapter = content->chapterNames[content->pages - 1] +
                    content->rowNow * content->columns;
    int size = 0;
    sprintf(chapter, "%d %s", content->lastSection, name);
    size = (int)strlen(chapter);
    memset(chapter + size, '.', sizeof(char) * (size_t)(content->columns -
    size - 2));

    size = (int)(floor(log10((double)page)));
    sprintf(chapter + content->columns - size - 2, "%d", page);

    content->rowNow++;
}
void addSubSection(struct tableOfContents *content, char *name, int page) {
    if(content == NULL) {
        return;
    }

    content->lastSubSection++;
    content->lastSubSubSection = 0;

    addPageIfNeeded(content);

    //Get chapter row
    char *chapter = content->chapterNames[content->pages - 1] +
                    content->rowNow * content->columns;
    int size = (int)(floor(log10(abs(content->lastSection)))) + 2;
    memset(chapter, ' ', sizeof(char) * (size_t)size);
    sprintf(chapter + size, "%d.%d %s", content->lastSection,
    content->lastSubSection, name);
    size = (int)strlen(chapter);
    memset(chapter + size, '.', sizeof(char) * (size_t)(content->columns -
    size - 2));

    size = (int)(ceil(log10((double)page + 1.)));
    sprintf(chapter + content->columns - size - 1, "%d", page);

    content->rowNow++;
}
void addSubSubSection(struct tableOfContents *content, char *name, int page) {
    if(content == NULL) {
        return;
    }

    content->lastSubSubSection++;

    addPageIfNeeded(content);

    //Get chapter row
    char *chapter = content->chapterNames[content->pages - 1] +
                    content->rowNow * content->columns;
    int size = 2*(int)(floor(log10(abs(content->lastSection)))) +
    (int)(floor(log10(abs(content->lastSubSection)))) + 6;
    memset(chapter, ' ', sizeof(char) * (size_t)size);
    sprintf(chapter + size, "%d.%d.%d %s", content->lastSection,
    content->lastSubSection, content->lastSubSubSection, name);
    size = (int)strlen(chapter);
    memset(chapter + size, '.', sizeof(char) * (size_t)(content->columns -
    size - 2));

    size = (int)(ceil(log10((double)page + 1.)));
    sprintf(chapter + content->columns - size - 1, "%d", page);

    content->rowNow++;
}

static const enum consoleLibColor colors[] = {
    CL_COLOR_BLUE, CL_COLOR_GREEN, CL_COLOR_CYAN
};
void drawContent(struct tableOfContents *content, int page) {
    if(content == NULL || content->pages <= page) {
        return;
    }

    char *pos = content->chapterNames[page];
    int maxRow;
    if(page < content->pages - 1) {
        maxRow = content->rows - 1;
    }else {
        maxRow = content->rowNow;
    }
    for(int i = 0;i < maxRow;i++) {
        int dotCount = 0;
        int numStart = 0;

        int j = 0;
        while(pos[j] != ' ' || !numStart) {
            if(pos[j] != ' ' && !numStart) {
                numStart = 1;
            }

            if(pos[j] == '.') {
                dotCount++;
            }

            j++;
        }

        setColor(colors[dotCount], CL_COLOR_NO_COLOR);
        drawf("%s\n", pos);

        pos += content->columns;
    }
    resetColor();
}

int getPageMouseClicked(struct tableOfContents *content, int page, int row) {
    if(content == NULL || content->pages <= page || content->rows-1 <= row) {
        return -1;
    }
    //For last page
    if(page == content->pages-1 && content->rowNow <= row) {
        return -1;
    }

    //Get last char
    char *pos = content->chapterNames[page] + content->columns * (row + 1) - 1;
    //Get page number pos
    while(1) {
        //"page": now: return value
        int tmp = sscanf(pos, "%d", &page);
        if(tmp == 0) {
            pos++;

            break;
        }

        pos--;
    }
    sscanf(pos, "%d", &page);

    return page - 1; //Convert to start by 0
}

void initMenu(struct menuContent *content, char **entryNames, int entryCount,
int xStart, int yStart, int columns, int rows, int fgColorNormal, int bgColorNormal,
int fgColorSelected, int bgColorSelected) {
    if(content == NULL || rows < entryCount)
        return;

    content->xStart = xStart;
    content->yStart = yStart;
    content->columns = columns;
    content->rows = rows;
    content->pos = 0;
    content->entryCount = entryCount;
    content->entryNames = entryNames;
    content->fgColorNormal = fgColorNormal;
    content->bgColorNormal = bgColorNormal;
    content->fgColorSelected = fgColorSelected;
    content->bgColorSelected = bgColorSelected;
}
void drawMenu(struct menuContent *content) {
    if(content == NULL)
        return;

    int y = content->yStart;
    int x = content->xStart;
    int width = content->columns;
    int strWidth;
    int xDraw;

    for(int i = 0;i < content->entryCount;i++) {
        strWidth = (int)strlen(content->entryNames[i]);
        xDraw = (int)(((double)(width - strWidth))*.5);
        xDraw += x;
        if(content->pos == i) {
            setColor(content->fgColorSelected, content->bgColorSelected);

            setCursorPos(xDraw - 1, y + i);
            drawf("*");

            setCursorPos(xDraw + strWidth, y + i);
            drawf("*");
        }else {
            setColor(content->fgColorNormal, content->bgColorNormal);
        }

        setCursorPos(xDraw, y + i);
        drawf("%s", content->entryNames[i]);
    }
}
int updateKeyMenu(struct menuContent *content, int key) {
    if(content == NULL)
        return -1;

    switch(key) {
        case CL_KEY_UP:
            content->pos--;
            if(content->pos == -1)
                content->pos = content->entryCount - 1;
            break;
        case CL_KEY_DOWN:
            content->pos++;
            if(content->pos == content->entryCount)
                content->pos = 0;
            break;
        case CL_KEY_ENTER:
            return content->pos;
    }

    return -1;
}
int updateMouseMenu(struct menuContent *content, int column, int row) {
    if(content == NULL || column == -1 || row == -1)
        return - 1;

    int pos = row - content->yStart;
    if(pos < 0 || pos >= content->entryCount)
        return -1;

    int width = content->columns;
    int strWidth = (int)strlen(content->entryNames[pos]);
    int xElementStart = (int)(((double)(width - strWidth))*.5) + content->xStart - 1;
    int xElementEnd = xElementStart + strWidth + 1;
    if(content->pos == pos) {
        xElementStart--;
        xElementEnd++;
    }

    if(column > xElementStart && column < xElementEnd) {
        if(content->pos == pos)
            return pos;
        else
            content->pos = pos;
    }

    return -1;
}
