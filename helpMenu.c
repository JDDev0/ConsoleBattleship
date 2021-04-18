#include "helpMenu.h"
#include "consoleLib.h"
#include "consoleMenu.h"
#include "gameField.h"

struct tableOfContents content;
int helpPage;
const int maxHelpPage = 11;

void initHelpMenu(int width, int height) {
    initTableOfContents(&content, width, height);

    addSection(&content, "Control", 3);
    addSubSection(&content, "Keyboard", 3);
    addSubSubSection(&content, "Help menu", 3);
    addSubSubSection(&content, "Exit window", 3);
    addSubSubSection(&content, "Start menu", 3);
    addSubSubSection(&content, "Settings menu", 3);
    addSubSubSection(&content, "Settings \"CUSTOM\" mode", 4);
    addSubSubSection(&content, "Game control", 4);
    addSubSubSection(&content, "\"BOMBER\" mode game control", 5);
    addSubSection(&content, "Mouse input", 5);
    addSubSubSection(&content, "Help menu", 5);
    addSubSubSection(&content, "Exit window", 6);
    addSubSubSection(&content, "Start menu", 6);
    addSubSubSection(&content, "Settings menu", 6);
    addSubSubSection(&content, "Settings \"CUSTOM\" mode", 6);
    addSubSubSection(&content, "Game control", 7);
    addSection(&content, "Menus", 8);
    addSubSection(&content, "Help menu", 8);
    addSubSection(&content, "Exit window", 8);
    addSubSection(&content, "Settings menu", 8);
    addSubSection(&content, "Settings \"CUSTOM\" mode menu", 8);
    addSubSection(&content, "Game screen", 9);
    addSection(&content, "Gameplay", 10);
    addSubSection(&content, "Set boats", 10);
    addSubSubSection(&content, "\"NORMAL\" mode", 10);
    addSubSubSection(&content, "\"BOMBER\" mode", 10);
    addSubSubSection(&content, "\"MINES\" mode", 11);
    addSubSection(&content, "Attack", 11);
    addSubSection(&content, "Computer shot", 11);
    addSubSection(&content, "Game over", 12);
}

void drawHelp(void) {
    setColor(CL_COLOR_YELLOW, CL_COLOR_NO_COLOR);
    setUnderline(1);
    drawf("Help menu");

    setCursorPos(0, 2);
    switch(helpPage) {
        case 0:
            setUnderline(0);
            drawContent(&content, 0);

            break;
        case 1:
            setUnderline(0);
            drawContent(&content, 1);

            break;
        case 2:
            setColor(CL_COLOR_BLUE, CL_COLOR_NO_COLOR);
            drawf("1 Control\n");

            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("1.1 Keyboard\n");

            setUnderline(0);
            resetColor();
            drawf("Press: ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("KEY\n");

            drawf("F1");
            resetColor();
            drawf(": Open help menu");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 7);
            drawf("1.1.1 Help menu\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ESC");
            resetColor();
            drawf("/");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("F1");
            resetColor();
            drawf(": Exit help menu\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("UP");
            resetColor();
            drawf("/");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("DOWN");
            resetColor();
            drawf(": Switch page");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 11);
            drawf("1.1.2 Exit window\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("y");
            resetColor();
            drawf(": Yes (Exit)\n");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("n");
            resetColor();
            drawf(": No (Not exit)");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 15);
            drawf("1.1.3 Start menu\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(": Open settings menu\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ESC");
            resetColor();
            drawf(": Exit window");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 19);
            drawf("1.1.4 Settings menu\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(": Start the game");

            break;
        case 3:
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            drawf("1.1.4 Settings menu\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ESC");
            resetColor();
            drawf(": Go to start menu\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("UP");
            resetColor();
            drawf("/");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("DOWN");
            resetColor();
            drawf(": Switch difficulty\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("LEFT");
            resetColor();
            drawf("/");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("RIGHT");
            resetColor();
            drawf(": Switch mode\n");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 7);
            drawf("1.1.5 Settings \"CUSTOM\" mode\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(": Start the game\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ESC");
            resetColor();
            drawf(": Go to start menu\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("b");
            resetColor();
            drawf(": Enable/disable bomber boat\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("m");
            resetColor();
            drawf(": Enable/disable mines\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("UP");
            resetColor();
            drawf("/");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("DOWN");
            resetColor();
            drawf(": Change mines count (If \"Has mines\" is \"true\")");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 14);
            drawf("1.1.6 Game control\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ESC");
            resetColor();
            drawf(": Exit window\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("x");
            resetColor();
            drawf(": Switch field (Not in \"Set boats\")\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("a");
            resetColor();
            drawf(" - ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("j");
            resetColor();
            drawf(": Select row\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("0");
            resetColor();
            drawf(" - ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("9");
            resetColor();
            drawf(": Select column\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("0");
            resetColor();
            drawf(", ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("1");
            resetColor();
            drawf(": Set orinentation (0: row, 1: column) (\"Set boats\" only)\n");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("t");
            resetColor();
            drawf(": Turn boat orinentation (\"Set boats\" only)\n");

            break;
        case 4:
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            drawf("1.1.6 Game control\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("Arrow keys");
            resetColor();
            drawf(": Move position\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(": Confirm\n");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 6);
            drawf("1.1.7 \"BOMBER\" mode game control\n");

            setUnderline(0);
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("n");
            resetColor();
            drawf(": Set shot mode to \"normal\"\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("o");
            resetColor();
            drawf(": Set shot mode to \"bomb\"\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("r");
            resetColor();
            drawf(": Set shot mode to \"row\"\n");

            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("l");
            resetColor();
            drawf(": Set shot mode to \"column\"");

            setUnderline(1);
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 12);
            drawf("1.2 Mouse input\n");

            setUnderline(0);
            resetColor();
            drawf("Left click: [");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"Position\"\n");
            drawf("Right click: [");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("R");
            resetColor();
            drawf("] \"Position\"\n");
            drawf("Middle click: [");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("M");
            resetColor();
            drawf("] \"Position\"");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 17);
            drawf("1.2.1 Help menu\n");

            setUnderline(0);
            resetColor();
            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"Page: 00\": Switch page (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("DOWN");
            resetColor();
            drawf(")\n[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] Chapter at first pages: Goto page");

            break;
        case 5:
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            drawf("1.2.2 Exit window\n");

            setUnderline(0);
            resetColor();
            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"[y]es\": Yes (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("y");
            resetColor();
            drawf(")\n[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"[n]o\": No (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("n");
            resetColor();
            drawf(")");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 6);
            drawf("1.2.3 Start menu\n");

            setUnderline(0);
            resetColor();
            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"ENTER\": Start game (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(")\n[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"Help: F1\": Open help menu (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("F1");
            resetColor();
            drawf(")");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 10);
            drawf("1.2.4 Settings menu\n");

            setUnderline(0);
            resetColor();
            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"ENTER\": Start the game (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(")\n");

            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"Difficulty\": Switch difficulty (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("UP");
            resetColor();
            drawf(")\n");

            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"Mode\": Switch mode (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("RIGHT");
            resetColor();
            drawf(")\n");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 15);
            drawf("1.2.5 Settings \"CUSTOM\" mode\n");

            setUnderline(0);
            resetColor();
            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"ENTER\": Start the game (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(")\n");

            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] Line \"Has bomber\": Enable/disable bomber boat "
            "(The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("b");
            resetColor();
            drawf(")\n");

            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] Line \"Has mines\": Enable/disable mines (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("m");
            resetColor();
            drawf(")\n");

            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] Line \"Mines count\": Change mines count (If \"Has mines\" is "
            "\"true\") (The\nsame as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("UP");
            resetColor();
            drawf(")");

            break;
        case 6:
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            drawf("1.2.6 Game control\n");

            setUnderline(0);
            resetColor();
            drawf("[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"Field: Player\": Switch to computer field (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("x");
            resetColor();
            drawf(")\n[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] \"Field: Computer\": Switch to player field (The same as ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("x");
            resetColor();
            drawf(")\n[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] Number row (columns): Set position\n[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] Letter column (rows): Set position\n[");
            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
            drawf("L");
            resetColor();
            drawf("] In a field: Set position");

            break;
        case 7:
            setColor(CL_COLOR_BLUE, CL_COLOR_NO_COLOR);
            drawf("2 Menus\n");
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("2.1 Help menu\n");

            setUnderline(0);
            resetColor();
            drawf("\"Page: x of y\": x: (Current page), y: (Last page)\n\"");
            setColor(CL_COLOR_BLUE, CL_COLOR_NO_COLOR);
            setUnderline(1);
            drawf("x Title");
            setUnderline(0);
            resetColor();
            drawf("\":     Heading 1 (Chapter Name)\n\"");
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            setUnderline(1);
            drawf("x.x Title");
            setUnderline(0);
            resetColor();
            drawf("\":   Heading 2 (Chapter.Chapter Name)\n\"");
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setUnderline(1);
            drawf("x.x.x Title");
            setUnderline(0);
            resetColor();
            drawf("\": Heading 3 (Chapter.Chapter.Chapter Name)\n");

            setUnderline(1);
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 9);
            drawf("2.2 Exit window\n");

            setUnderline(0);
            resetColor();
            drawf("Confirm exit\n");

            setUnderline(1);
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 12);
            drawf("2.3 Settings menu\n");

            setUnderline(0);
            resetColor();
            drawf("Difficulty: Set computer difficulty\nMode: Set mode");

            setUnderline(1);
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 16);
            drawf("2.4 Settings \"CUSTOM\" mode menu\n");

            setUnderline(0);
            resetColor();
            drawf("Has bomber: Use \"BOMBER\" mode features\nHas mines: Use \"MINE"
            "S\" mode features\nMines count: Set mines count for \"MINES\" mode");

            break;
        case 8:
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("2.6 Game screen\n");

            setUnderline(0);
            resetColor();
            drawf("Left-hand side: Game field\nRight-hand side: Information and in"
            "put field\n\n");

            setColor(CL_COLOR_PINK, CL_COLOR_NO_COLOR);
            drawf("Game field:");

            resetColor();
            setCursorPos(2, 7);
            drawf(": Empty\n  : No hit\n  : Hit\n\n        : Boat (Orientation: 0)"
            "\nBoat (Orientation: 1):");
            setCursorPos(2, 17);
            drawf(": EP-Boat (\"BOMBER\" mode only)\n  : Mine (\"MINES\" mode only"
            ")");

            setColor(CL_COLOR_YELLOW, CL_COLOR_NO_COLOR);
            setCursorPos(0, 8);
            drawf("--");
            setColor(CL_COLOR_RED, CL_COLOR_NO_COLOR);
            setCursorPos(0, 9);
            drawf("XX");
            setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_NO_COLOR);
            setCursorPos(0, 11);
            drawf("<# ## #>\n\n/\\\n##\n\\/\n\n[]\n**");

            break;
        case 9:
            setColor(CL_COLOR_BLUE, CL_COLOR_NO_COLOR);
            drawf("3 Gameplay\n");
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("3.1 Set boats\n");

            setUnderline(0);
            resetColor();
            drawf("Set  the  position  of  the   boat  and  press   ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf("  to  confirm  (See:\n");
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("1.1 Keyboard");
            resetColor();
            drawf(").");

            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 7);
            drawf("3.1.1 \"NORMAL\" mode\n");

            setUnderline(0);
            resetColor();
            drawf("Set %d boats without overlapping of two boats.", BOAT_COUNT);


            setUnderline(1);
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 10);
            drawf("3.1.2 \"BOMBER\" mode\n");

            setUnderline(0);
            resetColor();
            drawf("Set  %d  boats  and  one  power  (Energy   Points  (EP))  produ"
            "cer  without\noverlapping of two  boats. If  you  have  enough  EP, y"
            "ou  can  use  other\nattacks:\nBomb: Neighbour fields\nRow bomb: A wh"
            "ole row\nColumn bomb: A whole column", BOAT_COUNT);

            break;
        case 10:
            setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
            drawf("3.1.3 \"MINES\" mode\n");

            setUnderline(0);
            resetColor();
            drawf("\"NORMAL\"  mode with 2 mines  randomly  placed in the  field. "
                  "If a mine was\nhit, it will explode (3 x 3):\n");
            setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
            drawf("----------\n|  |  |  |\n----------\n|  |  |  |\n----------\n|  "
                  "|  |  |\n----------");
            setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_BLUE);
            setCursorPos(4, 8);
            drawf("**");
            setColor(CL_COLOR_YELLOW, CL_COLOR_BLUE);
            setCursorPos(1, 6);
            drawf("--");
            setCursorPos(4, 6);
            drawf("--");
            setCursorPos(7, 6);
            drawf("--");
            setCursorPos(1, 8);
            drawf("--");
            setCursorPos(1, 10);
            drawf("--");
            setCursorPos(4, 10);
            drawf("--");
            setColor(CL_COLOR_RED, CL_COLOR_BLUE);
            setCursorPos(7, 8);
            drawf("XX");
            setCursorPos(7, 10);
            drawf("XX");

            setUnderline(1);
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 13);
            drawf("3.2 Attack\n");

            setUnderline(0);
            resetColor();
            drawf("Set  the  position  of  the   boat  and  press   ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf("  to  confirm  (See:\n");
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("1.1 Keyboard");
            resetColor();
            drawf(").  Now you can see the hit size of a boat or \"No hit\".  Pres"
            "s\n");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(" to confirm.");

            setUnderline(1);
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            setCursorPos(0, 18);
            drawf("3.3 Computer shot\n");

            setUnderline(0);
            resetColor();
            drawf("You can see were the computer shot. Press ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ENTER");
            resetColor();
            drawf(" to confirm.");

            break;
        case 11:
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("3.4 Game over\n");

            setUnderline(0);
            resetColor();
            drawf("Press ");
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("ESC");
            resetColor();
            drawf(" to left the game (See: ");
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("1.1 Keyboard");
            resetColor();
            drawf(").");

            break;
    }

    setCursorPos(0, 22);
    resetColor();
    drawf("Page: ");
    setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
    drawf("%d", helpPage + 1);
    resetColor();
    drawf(" of ");
    setColor(CL_COLOR_CYAN, CL_COLOR_NO_COLOR);
    drawf("%d", maxHelpPage + 1);
}
