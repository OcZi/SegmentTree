/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Logo Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include <iostream>
#include <math.h>
#include <stdio.h>

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

static int logoPositionX = 0;
static int logoPositionY = 0;

static int lettersCount = 0;

static int topSideRecWidth = 0;
static int leftSideRecHeight = 0;

static int bottomSideRecWidth = 0;
static int rightSideRecHeight = 0;

static const int logoWH = 16;
static int logoPosXY = logoWH * 15;
static int topSideMaxWidth = logoWH * logoWH;
static int bottomSideMaxWidth = logoWH * logoWH;

static int state = 0;              // Logo animation states
static float alpha = 1.0f;         // Useful for fading

static int splash_title_delay = 3;
static const char splash_title[] = "Segment Tree\nVisualization";
static const char splash_subtitle[] = "powered by";
static const char raylib_text[] = "raylib";

//----------------------------------------------------------------------------------
// Logo Screen Functions Definition
//----------------------------------------------------------------------------------

// Logo Screen Initialization logic
void InitLogoScreen(void)
{
    finishScreen = 0;
    framesCounter = 0;
    lettersCount = 0;

    logoPositionX = 2 * GetScreenWidth() / 3;
    logoPositionY = GetScreenHeight() / 3;

    topSideRecWidth = logoWH;
    leftSideRecHeight = logoWH;
    bottomSideRecWidth = logoWH;
    rightSideRecHeight = logoWH;

    state = 0;
    alpha = 1.0f;
}

// Logo Screen Update logic
void UpdateLogoScreen(void)
{
    if (state == 0)                 // State 0: Top-left square corner blink logic
    {
        framesCounter++;

        if (framesCounter == 80)
        {
            state = 1;
            framesCounter = 0;      // Reset counter... will be used later...
        }
    }
    else if (state == 1)            // State 1: Bars animation logic: top and left
    {
        topSideRecWidth += 5;
        leftSideRecHeight += 5;

        if (topSideRecWidth >= topSideMaxWidth) state = 2;
    }
    else if (state == 2)            // State 2: Bars animation logic: bottom and right
    {
        bottomSideRecWidth += 5;
        rightSideRecHeight += 5;

        if (bottomSideRecWidth >= bottomSideMaxWidth) state = 3;
    }
    else if (state == 3)            // State 3: "raylib" text-write animation logic
    {
        framesCounter++;
        if (lettersCount < sizeof(splash_title))
        {
            if (framesCounter / splash_title_delay)   // Every 12 frames, one more letter!
            {
                lettersCount++;
                framesCounter = 0;
            }
        }
        else    // When all letters have appeared, just fade out everything
        {
            if (framesCounter > 200)
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    finishScreen = 1;   // Jump to next screen
                }
            }
        }
    }
}

// Logo Screen Draw logic
void DrawLogoScreen(void)
{
    if (state == 0)         // Draw blinking top-left square corner
    {
        if ((framesCounter/10)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
    }
    else if (state == 1)    // Draw bars animation: top and left
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, logoWH, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, logoWH, leftSideRecHeight, BLACK);
    }
    else if (state == 2)    // Draw bars animation: bottom and right
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, logoWH, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, logoWH, leftSideRecHeight, BLACK);

        DrawRectangle(logoPositionX + logoPosXY, logoPositionY, logoWH, rightSideRecHeight, BLACK);
        DrawRectangle(logoPositionX, logoPositionY + logoPosXY, bottomSideRecWidth, logoWH, BLACK);
    }
    else if (state == 3)    // Draw "raylib" text-write animation + "powered by"
    {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, logoWH, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY, logoWH, leftSideRecHeight, Fade(BLACK, alpha));

        DrawRectangle(logoPositionX + logoPosXY, logoPositionY, logoWH, rightSideRecHeight, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + logoPosXY, bottomSideRecWidth, logoWH, Fade(BLACK, alpha));

        DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

        DrawText(TextSubtext(splash_title, 0, lettersCount), GetScreenWidth() / 12 - 44, GetScreenHeight() / 2 + 48, 50, Fade(BLACK, alpha));
        DrawText(splash_subtitle, logoPositionX, logoPositionY - 27, 20, Fade(DARKGRAY, alpha));
        DrawText(raylib_text, logoPositionX + logoPosXY - 148, logoPositionY + 180, 48, Fade(BLACK, alpha));
    }
}

// Logo Screen Unload logic
void UnloadLogoScreen(void)
{
    // Unload LOGO screen variables here!
}

// Logo Screen should finish?
int FinishLogoScreen(void)
{
    return finishScreen;
}
