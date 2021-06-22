/******************************************************************************
 * This file is part of project ORCA. More information on the project
 * can be found at the following repositories at GitHub's website.
 *
 * http://https://github.com/andersondomingues/orca-rt-bench
 * https://github.com/bennoXav/ORB_KProfiller
 *
 * Copyright (C) 2021 João Benno Weber, <jbweber05@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
******************************************************************************/
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define MAX_INPUT_CHARS 4
#define MAX_COLORS_COUNT 21
int UniqueElements(int arr1[], int n);
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 650;
    InitWindow(screenWidth, screenHeight, "ORCA-RT-BENCH: Kprofiller");
    //checkboxes
    Color checkboxSelectedRM = BLACK;
    Color checkboxSelectedEDF = BLACK;
    Color checkboxSelectedLST = BLACK;

    char *edfAlgorithm = "The earlier the deadline of a task, the higher is its priority.";
    char *rmAlgorithm = "The higher the frequency (1/period) of a task, the higher is its priority.";
    char *lstAlgorithm = "The least amount of slack time of a task(amount of time left after work), the higher is its priority.";
    bool checkboxEDF = false;
    bool checkboxLST = false;
    bool checkboxRM = false;
    int count = 0;
    //input file
    char **droppedFiles = {0};
    //run time text box
    char runTimeMS[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for line ending char '\0'
    int letterCount = 0;
    Rectangle textBox = {screenWidth * 0.75 - 75, 60, 120, 40};
    bool mouseOnText = false;
    bool mouseonTextBox = false;
    int framesCounter = 0;
    //run|close buttons
    bool runButton = false;
    bool closeButton = false;
    bool running = false;
    //teste de input direto
    int testeinput[18 * 7] = {
        1, 1, 1, 2, 3, 1, 1, 1, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2,
        1, 1, 1, 2, 3, 1, 1, 1, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2,
        1, 1, 1, 2, 3, 1, 1, 1, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2,
        1, 1, 1, 2, 3, 1, 1, 1, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2,
        1, 1, 1, 2, 3, 1, 1, 1, 2, 1, 1, 1, 2, 3, 1, 1, 1, 2,
        4, 4, 4, 5, 6, 4, 4, 4, 5, 4, 4, 4, 5, 6, 4, 4, 4, 5,
        7, 7, 7, 8, 9, 7, 7, 7, 8, 7, 7, 7, 8, 9, 7, 7, 7, 8};
    size_t nelementos = sizeof(testeinput) / sizeof(int);
    int uniqueelements = UniqueElements(testeinput, nelementos);
    int runtime = 110;
    int runtimesimu;
    if (runtime > 50)
        runtimesimu = (runtime / 50) + 1;
    else
        runtimesimu = 1;

    int ntasks;
    if (uniqueelements >= 4 && uniqueelements % 4 == 0)
        ntasks = (uniqueelements / 4);
    else if (uniqueelements >= 4 && uniqueelements % 4 != 0)
        ntasks = (uniqueelements / 4) + 1;
    else
        ntasks = 1;
    int heightpanelcr;

    if (uniqueelements < 4)
        heightpanelcr = 4;
    else
        heightpanelcr = uniqueelements;

    //colors for simulation
    Color colors[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE};
    Rectangle panelRec = {14, 109, (GetScreenWidth() - 29), 388};
    printf("%d\n\n", ntasks);
    Rectangle panelContentRec = {14, 110, GetScreenWidth() * runtimesimu, 94 * heightpanelcr};
    Vector2 panelScroll = {99, 0};
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        //runtime inputtext box mouse verifier
        //----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(GetMousePosition(), textBox))
            mouseOnText = true;
        else
            mouseOnText = false;

        if (mouseonTextBox)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if (((key >= 48) && (key <= 57)) && (letterCount < MAX_INPUT_CHARS))
                {
                    runTimeMS[letterCount] = (char)key;
                    letterCount++;
                }

                key = GetCharPressed(); // Check next character in the queue
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0)
                    letterCount = 0;
                runTimeMS[letterCount] = '\0';
            }
        }
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        if (mouseonTextBox)
            framesCounter++;
        else
            framesCounter = 0;
        //file dropped
        //----------------------------------------------------------------------------------
        if (IsFileDropped())
        {
            droppedFiles = GetDroppedFiles(&count);
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        //Runtime Selection area
        //---------------------------------------------------------------------------------
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseonTextBox || mouseOnText)
            DrawRectangleRoundedLines(textBox, 0.1f, 1, 2, BLUE);
        else
            DrawRectangleRoundedLines(textBox, 0.1f, 1, 2, BLACK);
        DrawText(runTimeMS, textBox.x + 7, textBox.y + 7, 40, MAROON);
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouseOnText)
        {
            mouseonTextBox = true;
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !mouseOnText)
        {
            mouseonTextBox = false;
        }
        if (mouseonTextBox)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter / 60) % 2) == 0)
                    DrawText("_", textBox.x + 10 + MeasureText(runTimeMS, 40), textBox.y + 7, 40, MAROON);
            }
        }
        // Dropped File Area
        //---------------------------------------------------------------------------------
        DrawRectangleRounded((Rectangle){15, 15, screenWidth / 2 - 30, 90}, 0.1f, 1, Fade(DARKPURPLE, 0.3f));
        // DrawRectangleRoundedLines((Rectangle){15, 15, screenWidth / 2 - 15, 90}, 0.1f, 1, 1, BLACK);
        if (count == 0)
            DrawText("Drop your data file here", 20, 20, 20, DARKGRAY);
        else
        {
            DrawText("Dropped file:", 20, 20, 20, DARKGRAY);
            DrawRectangleRounded((Rectangle){20, 60, screenWidth / 2 - 40, 40}, 0.1f, 1, Fade(DARKGRAY, 0.3f));
            DrawText(droppedFiles[0], 90, 75, 14, DARKGRAY);
        }
        //Scheduling running time area
        //----------------------------------------------------------------------------------------
        DrawRectangleRounded((Rectangle){screenWidth / 2 + 15, 15, screenWidth / 2 - 30, 90}, 0.1f, 1, Fade(DARKPURPLE, 0.3f));
        DrawText("Input the simulation time ", screenWidth / 2 + 20, 20, 20, DARKGRAY);
        DrawText("Milliseconds", screenWidth * 0.75 + 60, 85, 20, DARKGRAY);
        //----------------------------------------------------------------------------------------
        //scheduling algorithm area design
        //----------------------------------------------------------------------------------------
        DrawRectangleRounded((Rectangle){14, 499, GetScreenWidth() - 29, 41}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
        DrawRectangleRoundedLines((Rectangle){15, 500, GetScreenWidth() - 30, 40}, 0.1f, 1, 1, checkboxSelectedRM);
        DrawText(edfAlgorithm, 100, 512, 14, DARKGRAY);
        DrawRectangleRounded((Rectangle){14, 544, GetScreenWidth() - 29, 41}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
        DrawRectangleRoundedLines((Rectangle){15, 545, GetScreenWidth() - 30, 40}, 0.1f, 1, 1, checkboxSelectedEDF);
        DrawText(rmAlgorithm, 100, 557, 14, DARKGRAY);
        DrawRectangleRounded((Rectangle){14, 589, GetScreenWidth() - 29, 41}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
        DrawRectangleRoundedLines((Rectangle){15, 590, GetScreenWidth() - 30, 40}, 0.1f, 1, 1, checkboxSelectedLST);
        DrawText(lstAlgorithm, 100, 602, 14, DARKGRAY);
        //  Scheduling Algorithms Checkboxes
        //-----------------------------------------------------------------------------------------
        if (GuiCheckBox((Rectangle){30, 510, 20, 20}, "RM", checkboxRM))
        {
            checkboxEDF = false;
            checkboxLST = false;
            checkboxRM = true;
            checkboxSelectedRM = GOLD;
            checkboxSelectedEDF = BLACK;
            checkboxSelectedLST = BLACK;
        }
        if (GuiCheckBox((Rectangle){30, 555, 20, 20}, "EDF", checkboxEDF))
        {
            checkboxEDF = true;
            checkboxLST = false;
            checkboxRM = false;

            checkboxSelectedRM = BLACK;
            checkboxSelectedEDF = GOLD;
            checkboxSelectedLST = BLACK;
        }
        if (GuiCheckBox((Rectangle){30, 600, 20, 20}, "LST", checkboxLST))
        {
            checkboxEDF = false;
            checkboxLST = true;
            checkboxRM = false;
            checkboxSelectedRM = BLACK;
            checkboxSelectedEDF = BLACK;
            checkboxSelectedLST = GOLD;
        }

        //Run Button && Close button
        //------------------------------------------------------------------------------------
        runButton = GuiButton((Rectangle){GetScreenWidth() - 115, 445, 100, 50}, "RUN");
        if (runButton)
        {
            closeButton = false;
            running = true;
        }
        if (closeButton)
        {
            runButton = false;
            running = false;
        }
        //Scheduling simulation area
        //-----------------------------------------------------------------------------------
        if (running)
        {
            Rectangle view = GuiScrollPanel(panelRec, panelContentRec, &panelScroll);
            BeginScissorMode(view.x, view.y, view.width, view.height);
            DrawRectangle(14, 109, GetScreenWidth() * runtimesimu, 495 - 110, Fade(LIGHTGRAY, 0.2f));
            DrawRectangleLinesEx((Rectangle){15, 110, GetScreenWidth() * runtimesimu, 495 - 110}, 2, BLACK);

            for (int i = 0; i < uniqueelements; i++)
            {
                DrawText(TextFormat("Task %02i", i + 1), 20 + panelRec.x + panelScroll.x, 80 + 80 * i + panelRec.y + panelScroll.y, 20, BLACK);
                DrawLine(120 + panelRec.x + panelScroll.x, 100 + 80 * i + panelRec.y + panelScroll.y, ((GetScreenWidth() - 29) * runtimesimu) + panelRec.x + panelScroll.x, 100 + 80 * i + panelRec.y + panelScroll.y, BLACK);
            }

            for (int i = 0; i < sizeof(testeinput) / sizeof(int); i++)
            {
                DrawRectangle(120 + 20 * i + panelRec.x + panelScroll.x, 80 + 80 * (testeinput[i] - 1) + panelRec.y + panelScroll.y, 20, 20, colors[testeinput[i]]);
                DrawRectangleLinesEx((Rectangle){120 + 20 * i + panelRec.x + panelScroll.x, 80 + 80 * (testeinput[i] - 1) + panelRec.y + panelScroll.y, 20, 20}, 1, BLACK);
            }

            for (int i = 0; i <= runtime / 5; i++)
            {
                DrawText(TextFormat("%02i", 5 * i), 122 + 20 * i * 5 + panelRec.x + panelScroll.x, 260 * ntasks + panelRec.y + panelScroll.y, 1, BLACK);
                DrawLine(120 + 20 * i * 5 + panelRec.x + panelScroll.x, 100 + panelRec.y + panelScroll.y, 120 + 20 * i * 5 + panelRec.x + panelScroll.x, 270 * ntasks + panelRec.y + panelScroll.y, BLACK);
            }
            closeButton = GuiButton((Rectangle){GetScreenWidth() - 115, 110, 100, 50}, "CLOSE");
        }
        EndScissorMode();
        //END DRAWING
        //-----------------------------------------------------------------------------------
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    ClearDroppedFiles(); // Clear internal buffers
    CloseWindow();       // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
// NOTE: We limit keys check to keys between 48 and 57 (only nymbers allowed)
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();
    if (((key >= 48) && (key <= 57)))
        keyPressed = true;

    return keyPressed;
}

int UniqueElements(int arr1[], int n)
{
    int res = 1;
    for (int i = 1; i < n; i++)
    {
        int j = 0;
        for (j = 0; j < i; j++)
            if (arr1[i] == arr1[j])
                break;
        if (i == j)
            res++;
    }
    return res;
}