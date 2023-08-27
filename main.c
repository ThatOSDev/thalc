
// TODO : Set-up order of math. MATH ORDER MATTERS !
// TODO : Fix Decimals (float)
// TODO : Add ICON
// TODO : Fix percentages
// TODO : Fix zeros
// TODO : Allow more then 3 tokens

// FIXED : Font Embedded as a C file
// FIXED : Can continue new math set without requiring a CLEAR "C" button being pressed

// UNDECIDED : Decide on long long (64-Bit) or 32-Bit for all math problems. Or use Doubles for everything.
//             If doubles, then total rewrite of math section will be required.

#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ElaineSans_FONT.h"

#define MAXBUTTONS 72

int screenWidth = 450;
int screenHeight = 550;

Vector2 calcDisplayNumPosXY = {404, 97};
Vector2 calcDisplayTotalsPosXY = {400, 87};

Texture2D mainGUI;
Font buttonFont;
Font displayFont;
Font totalsFont;

void createWindow()
{
#ifndef DEBUGME
    SetTraceLogLevel(LOG_NONE);
#endif // DEBUGME

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Thalc v1.0  --  ThatOSDev");

    totalsFont  = LoadFontFromMemory(".ttf", asciifont, 347736, 24, 0, 0);
    buttonFont  = LoadFontFromMemory(".ttf", asciifont, 347736, 44, 0, 0);
    displayFont = LoadFontFromMemory(".ttf", asciifont, 347736, 70, 0, 0);

    SetTargetFPS(60);
}

void drawRoundedRectangle(Image *dst, int posX, int posY, int width, int height, Color color)
{
    ImageDrawRectangle(dst, posX + height, posY, width, (height * 2) + 1, color);
    ImageDrawCircle(dst, posX + height, posY + height, height, color);
    ImageDrawCircle(dst, posX + (width + height), posY + height, height, color);
}

Image drawCalcTexture(Color bttnclr, Color txtclr)
{
    Image tempTexture = GenImageGradientV(screenWidth, screenHeight, (Color){20, 20, 20, 255}, (Color){40, 40, 65, 255});

    drawRoundedRectangle(&tempTexture, 30, 190, 140, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 230, 190, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 330, 190, 40, 25, bttnclr);

    drawRoundedRectangle(&tempTexture,  30, 260, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 130, 260, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 230, 260, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 330, 260, 40, 25, bttnclr);

    drawRoundedRectangle(&tempTexture,  30, 330, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 130, 330, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 230, 330, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 330, 330, 40, 25, bttnclr);

    drawRoundedRectangle(&tempTexture,  30, 400, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 130, 400, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 230, 400, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 330, 400, 40, 25, bttnclr);

    drawRoundedRectangle(&tempTexture, 30, 470, 140, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 230, 470, 40, 25, bttnclr);
    drawRoundedRectangle(&tempTexture, 330, 470, 40, 25, bttnclr);

    ImageDrawTextEx(&tempTexture, displayFont, "C        %     /", (Vector2){ 110.0f, 178.0f }, (float)displayFont.baseSize, 0.0f, txtclr);
    ImageDrawTextEx(&tempTexture, displayFont, "7     8     9     X", (Vector2){ 60.0f, 248.0f }, (float)displayFont.baseSize, 0.0f, txtclr);
    ImageDrawTextEx(&tempTexture, displayFont, "4     5     6     -", (Vector2){ 60.0f, 320.0f }, (float)displayFont.baseSize, 0.0f, txtclr);
    ImageDrawTextEx(&tempTexture, displayFont, "1     2     3     +", (Vector2){ 60.0f, 390.0f }, (float)displayFont.baseSize, 0.0f, txtclr);
    ImageDrawTextEx(&tempTexture, displayFont, "0         .      =", (Vector2){ 110.0f, 458.0f }, (float)displayFont.baseSize, 0.0f, txtclr);
    return tempTexture;
}

void createGUI()
{
    Image mainTexture = GenImageGradientV(screenWidth, screenHeight, (Color){20, 20, 20, 255}, (Color){40, 40, 65, 255});

    Image tempImage = drawCalcTexture(BLUE, BLACK);
    ImageDraw(&mainTexture, tempImage, (Rectangle){0, 0, screenWidth, screenHeight}, (Rectangle){0, 0, screenWidth, screenHeight}, WHITE);
    UnloadImage(tempImage);

    Image barImage = GenImageGradientV(screenWidth, 60, (Color){90, 90, 90, 255}, (Color){25, 20, 20, 255});
    ImageDraw(&mainTexture, barImage, (Rectangle){0, 0, screenWidth, 60}, (Rectangle){0, 0, screenWidth, 60}, WHITE);
    UnloadImage(barImage);

    ImageDrawTextEx(&mainTexture, displayFont, "THALC", (Vector2){ 20.0f, 0.0f }, (float)displayFont.baseSize, 0.0f, BLACK);
    ImageDrawTextEx(&mainTexture, displayFont, "THALC", (Vector2){ 16.0f, -4.0f }, (float)displayFont.baseSize, 0.0f, GRAY);

    ImageDrawRectangle(&mainTexture, 25, 80, screenWidth - 50, 80, SKYBLUE);

    mainGUI = LoadTextureFromImage(mainTexture);
    UnloadImage(mainTexture);
}

enum symbols
{
    NONE,
    ADDITION,
    SUBTRACTION,
    MULTIPLY,
    DEVIDE,
    PERCENT,
    DECIMAL
};

char tokenSet[14] = {'\0'};
char displayTotals[100] = {'\0'};
char displayNumber[34] = {'\0'};
int  tokens[14] = {'0'};

int equalTriggered = 0, stringIndex = 0, numIndex = 0, offsetTotals = 0, offsetNumber = 0, repeat = 0, numLimit = 0, tokenIndex = 0, tokenSetIndex = 0;

void resetArrays()
{
    for(int t = 0; t < 14; t++)
    {
        tokens[t] = '\0';
    }
    for(int t = 0; t < 14; t++)
    {
        tokenSet[t] = '\0';
    }

    tokenIndex = 0;
    tokenSetIndex = 0;
    equalTriggered = 0;
    offsetNumber = 0;
    offsetTotals = 0;
}

void combineTokens()
{
    for(int r = 0; r < tokenSetIndex; r++)
    {
        if(tokenSet[r] == MULTIPLY)
        {
            printf("* @ %d\n", r);
        }
    }

    tokenIndex = 1;
    int result = 0;
    if(tokens[tokenIndex] == ADDITION)
    {
        result = tokens[tokenIndex - 1] + tokens[tokenIndex + 1];
        sprintf(displayNumber, "%d", result);
        int len = strlen(displayNumber);
        offsetNumber = 30 * len;
    } else if(tokens[tokenIndex] == SUBTRACTION)
    {
        result = tokens[tokenIndex - 1] - tokens[tokenIndex + 1];
        sprintf(displayNumber, "%d", result);
        int len = strlen(displayNumber);
        offsetNumber = 30 * len;
    } else if(tokens[tokenIndex] == MULTIPLY)
    {
        result = tokens[tokenIndex - 1] * tokens[tokenIndex + 1];
        sprintf(displayNumber, "%d", result);
        int len = strlen(displayNumber);
        offsetNumber = 30 * len;
    } else if(tokens[tokenIndex] == DEVIDE)
    {
        result = tokens[tokenIndex - 1] / tokens[tokenIndex + 1];
        sprintf(displayNumber, "%d", result);
        int len = strlen(displayNumber);
        offsetNumber = 30 * len;
    } else if(tokens[tokenIndex] == PERCENT)
    {
        result = tokens[tokenIndex - 1] % tokens[tokenIndex + 1];
        sprintf(displayNumber, "%d", result);
        int len = strlen(displayNumber);
        offsetNumber = 30 * len;
    }
}

void addToString(char* tempString)
{
    if((strcmp(tempString, "*") != 0) && (strcmp(tempString, "=") != 0) && (strcmp(tempString, "+") != 0) &&
       (strcmp(tempString, "-") != 0) && (strcmp(tempString, "/") != 0) && (strcmp(tempString, ".") != 0))
    {
        repeat = 0;
    }

    if(strcmp(tempString, "*") == 0 && stringIndex != 0 && repeat == 0)
    {
        if(numLimit == 1)
            return;

        if(equalTriggered == 1)
        {
            resetArrays();
        }

        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;
        tokenSet[tokenSetIndex] = MULTIPLY;
        tokenSetIndex++;

        tokens[tokenIndex] = atoi(displayNumber);
        tokenIndex++;
        tokens[tokenIndex] = MULTIPLY;
        tokenIndex++;
        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;

        numIndex = 0;
        offsetNumber = 30;
        for(int t = 0; t < 30; t++)
        {
            displayNumber[t] = '\0';
        }
        displayNumber[0] = '0';
        repeat = 1;
        offsetTotals += 10;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        displayTotals[stringIndex] = *tempString;
        stringIndex++;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        offsetTotals += 12;
    } else if(strcmp(tempString, "%") == 0 && stringIndex != 0 && repeat == 0)
    {
        if(numLimit == 1)
            return;

        if(equalTriggered == 1)
        {
            resetArrays();
        }

        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;
        tokenSet[tokenSetIndex] = PERCENT;
        tokenSetIndex++;

        tokens[tokenIndex] = atoi(displayNumber);
        tokenIndex++;
        tokens[tokenIndex] = PERCENT;
        tokenIndex++;
        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;

        numIndex = 0;
        offsetNumber = 30;
        for(int t = 0; t < 30; t++)
        {
            displayNumber[t] = '\0';
        }
        displayNumber[0] = '0';
        repeat = 1;
        offsetTotals += 10;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        displayTotals[stringIndex] = *tempString;
        stringIndex++;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        offsetTotals += 12;
    } else if(strcmp(tempString, "-") == 0 && stringIndex != 0 && repeat == 0)
    {
        if(numLimit == 1)
            return;

        if(equalTriggered == 1)
        {
            resetArrays();
        }

        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;
        tokenSet[tokenSetIndex] = SUBTRACTION;
        tokenSetIndex++;

        tokens[tokenIndex] = atoi(displayNumber);
        tokenIndex++;
        tokens[tokenIndex] = SUBTRACTION;
        tokenIndex++;
        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;

        numIndex = 0;
        offsetNumber = 30;
        for(int t = 0; t < 30; t++)
        {
            displayNumber[t] = '\0';
        }
        displayNumber[0] = '0';
        repeat = 1;
        offsetTotals += 10;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        displayTotals[stringIndex] = *tempString;
        stringIndex++;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        offsetTotals += 12;
    } else if(strcmp(tempString, "/") == 0 && stringIndex != 0 && repeat == 0)
    {
        if(numLimit == 1)
            return;

        if(equalTriggered == 1)
        {
            resetArrays();
        }

        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;
        tokenSet[tokenSetIndex] = DEVIDE;
        tokenSetIndex++;

        tokens[tokenIndex] = atoi(displayNumber);
        tokenIndex++;
        tokens[tokenIndex] = DEVIDE;
        tokenIndex++;
        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;

        numIndex = 0;
        offsetNumber = 30;
        for(int t = 0; t < 30; t++)
        {
            displayNumber[t] = '\0';
        }
        displayNumber[0] = '0';
        repeat = 1;
        offsetTotals += 10;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        displayTotals[stringIndex] = *tempString;
        stringIndex++;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        offsetTotals += 12;
    } else if(strcmp(tempString, "+") == 0 && stringIndex != 0 && repeat == 0)
    {
        if(numLimit == 1)
            return;

        if(equalTriggered == 1)
        {
            resetArrays();
        }

        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;
        tokenSet[tokenSetIndex] = ADDITION;
        tokenSetIndex++;

        tokens[tokenIndex] = atoi(displayNumber);
        tokenIndex++;
        tokens[tokenIndex] = ADDITION;
        tokenIndex++;
        tokenSet[tokenSetIndex] = tokenIndex;
        tokenSetIndex++;

        numIndex = 0;
        offsetNumber = 30;
        for(int t = 0; t < 30; t++)
        {
            displayNumber[t] = '\0';
        }
        displayNumber[0] = '0';
        repeat = 1;
        offsetTotals += 10;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        displayTotals[stringIndex] = *tempString;
        stringIndex++;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        offsetTotals += 12;
    } else if(strcmp(tempString, "=") == 0 && repeat == 0)
    {
        tokens[tokenIndex] = atoi(displayNumber);
        tokenIndex++;
        printf("TOTAL TOKENS : %d\n", tokenIndex);   //  TODO : Remove this when no longer needed
        numLimit = 1;
        repeat = 1;
        equalTriggered = 1;
        numIndex = 0;
        offsetTotals += 10;
        displayTotals[stringIndex] = ' ';
        stringIndex++;
        displayTotals[stringIndex] = *tempString;
        combineTokens();
    } else if(strcmp(tempString, "C") != 0 && repeat == 0)
    {
        if((strcmp(tempString, "0") == 0) && (numIndex == 0))
            return;


        if(numIndex == 0)
        {
            offsetNumber = 0;
        }

        if(strcmp(tempString, ".") == 0)
        {
            tokens[tokenIndex] = atoi(displayNumber);
            tokenIndex++;
            tokens[tokenIndex] = DECIMAL;
            tokenIndex++;
        }

        if(equalTriggered == 1)
        {
            resetArrays();

            numLimit = 0;

            for(int t = 0; t < 30; t++)
            {
                displayNumber[t] = '\0';
            }
            displayNumber[0] = '0';
            numIndex = 0;
            for(int t = 0; t < 100; t++)
            {
                displayTotals[t] = '\0';
            }
            stringIndex = 0;
        }

        displayTotals[stringIndex] = *tempString;
        stringIndex++;
        displayNumber[numIndex] = *tempString;
        numIndex++;
        offsetTotals += 10;
        offsetNumber += 30;
    }

    if(strcmp(tempString, "C") == 0)
    {
        resetArrays();
        offsetNumber = 30;

        for(int t = 0; t < 100; t++)
        {
            displayTotals[t] = '\0';
        }

        for(int t = 0; t < 30; t++)
        {
            displayNumber[t] = '\0';
        }
        displayNumber[0] = '0';
        numIndex = 0;
        stringIndex = 0;
        numLimit = 0;
        repeat = 0;
    }
}

int main(void)
{
    createWindow();
    createGUI();

    Image tempMainTexture1 = GenImageGradientV(screenWidth, screenHeight, (Color){20, 20, 20, 255}, (Color){40, 40, 65, 255});
    tempMainTexture1 = drawCalcTexture(ORANGE, DARKGRAY);
    Texture2D mainButtonHover = LoadTextureFromImage(tempMainTexture1);
    UnloadImage(tempMainTexture1);

    Image tempMainTexture2 = GenImageGradientV(screenWidth, screenHeight, (Color){20, 20, 20, 255}, (Color){40, 40, 65, 255});
    tempMainTexture2 = drawCalcTexture(BEIGE, DARKGRAY);
    Texture2D mainButtonPressed = LoadTextureFromImage(tempMainTexture2);
    UnloadImage(tempMainTexture2);

    int buttonPosXY[MAXBUTTONS] = {30, 190, 220, 240,  // C
                                  230, 190, 320, 240,  // %
                                  330, 190, 420, 240,  // /
                                   30, 260, 120, 310,  // 7
                                  130, 260, 220, 310,  // 8
                                  230, 260, 320, 310,  // 9
                                  330, 260, 420, 310,  // *
                                   30, 330, 120, 380,  // 4
                                  130, 330, 220, 380,  // 5
                                  230, 330, 320, 380,  // 6
                                  330, 330, 420, 380,  // -
                                   30, 400, 120, 450,  // 1
                                  130, 400, 220, 450,  // 2
                                  230, 400, 320, 450,  // 3
                                  330, 400, 420, 450,  // +
                                   30, 470, 220, 510,  // 0
                                  230, 470, 320, 510,  // .
                                  330, 470, 420, 510}; // =

    int mouseButtonPressed = 0, mPressed = 0;

    displayNumber[0] = '0';
    offsetNumber = 30;

    while (!WindowShouldClose())
    {
        int DD = 0, DDWidth = 90;
        Vector2 mPos = GetMousePosition();
        for(int r = 0; r < MAXBUTTONS; r++)
        {
            if((mPos.x > buttonPosXY[r]) && (mPos.y > buttonPosXY[r + 1]) &&
               (mPos.x < buttonPosXY[r + 2]) && (mPos.y < buttonPosXY[r + 3]))
            {
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseButtonPressed == 0)
                {
                    mouseButtonPressed = 1;
                } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    mouseButtonPressed = 0;
                    mPressed = 0;
                }

                switch(r)
                {
                    case 0:
                    {
                        DD = 1;
                        DDWidth = 190;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("C");}
                        break;
                    }
                    case 4:
                    {
                        DD = 2;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("%");}
                        break;
                    }
                    case 8:
                    {
                        DD = 3;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("/");}
                        break;
                    }
                      case 12:
                    {
                        DD = 4;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("7");}
                        break;
                    }
                    case 16:
                    {
                        DD = 5;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("8");}
                        break;
                    }
                    case 20:
                    {
                        DD = 6;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("9");}
                        break;
                    }
                    case 24:
                    {
                        DD = 7;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("*");}
                        break;
                    }
                    case 28:
                    {
                        DD = 8;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("4");}
                        break;
                    }
                    case 32:
                    {
                        DD = 9;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("5");}
                        break;
                    }
                    case 36:
                    {
                        DD = 10;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("6");}
                        break;
                    }
                    case 40:
                    {
                        DD = 11;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("-");}
                        break;
                    }
                    case 44:
                    {
                        DD = 12;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("1");}
                        break;
                    }
                    case 48:
                    {
                        DD = 13;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("2");}
                        break;
                    }
                    case 52:
                    {
                        DD = 14;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("3");}
                        break;
                    }
                    case 56:
                    {
                        DD = 15;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("+");}
                        break;
                    }
                    case 60:
                    {
                        DD = 16;
                        DDWidth = 190;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("0");}
                        break;
                    }
                    case 64:
                    {
                        DD = 17;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString(".");}
                        break;
                    }
                    case 68:
                    {
                        DD = 18;
                        if(mouseButtonPressed == 1 && mPressed == 0) {mPressed = 1; addToString("=");}
                        break;
                    }
                }
            }
            r += 3;
        }

        const char* calcDisplayTotals = TextFormat("%s", displayTotals);
        const char* calcDisplayNumber = TextFormat("%s", displayNumber);

        BeginDrawing();
            DrawTextureV(mainGUI, (Vector2){0, 0}, WHITE);
            DrawTextEx(totalsFont, calcDisplayTotals, (Vector2){(calcDisplayTotalsPosXY.x) - offsetTotals, calcDisplayTotalsPosXY.y}, 24, 1, DARKGRAY);
            DrawTextEx(totalsFont, calcDisplayTotals, (Vector2){(calcDisplayTotalsPosXY.x - 2) - offsetTotals, calcDisplayTotalsPosXY.y - 2}, 24, 1, BLACK);

            DrawTextEx(displayFont, calcDisplayNumber, (Vector2){(calcDisplayNumPosXY.x) - offsetNumber, calcDisplayNumPosXY.y}, 70, 1, DARKGRAY);
            DrawTextEx(displayFont, calcDisplayNumber, (Vector2){(calcDisplayNumPosXY.x - 2) - offsetNumber, calcDisplayNumPosXY.y - 2}, 70, 1, BLACK);

            if(DD > 0 && mouseButtonPressed == 0){DrawTextureRec(mainButtonHover, (Rectangle){buttonPosXY[(DD * 4) - 4], buttonPosXY[((DD * 4) - 4) + 1], DDWidth, 51},
                                                                  (Vector2){buttonPosXY[(DD * 4) - 4], buttonPosXY[((DD * 4) - 4) + 1]}, WHITE);}
            if(DD > 0 && mouseButtonPressed == 1){DrawTextureRec(mainButtonPressed, (Rectangle){buttonPosXY[(DD * 4) - 4], buttonPosXY[((DD * 4) - 4) + 1], DDWidth, 51},
                                                                  (Vector2){buttonPosXY[(DD * 4) - 4], buttonPosXY[((DD * 4) - 4) + 1]}, WHITE);}

        EndDrawing();
    }

    UnloadTexture(mainButtonHover);
    UnloadTexture(mainButtonPressed);
    UnloadTexture(mainGUI);

    UnloadFont(totalsFont);
    UnloadFont(buttonFont);
    UnloadFont(displayFont);

    CloseWindow();

    return 0;
}

