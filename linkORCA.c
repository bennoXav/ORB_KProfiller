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
#include <unistd.h>
#include <math.h>
#include <string.h>
#define MAX_INPUT_CHARS 4
#define MAX_COLORS_COUNT 21
int UniqueElements(int arr1[], int n);
int *outputOrca(char *fileoutput, int runtime);
double schedtest(char *fileoutput, int typeoftest);
int largest(int arr[], int n);

char **droppedFiles = {0};
char **fileoutput = {0};
size_t nelementos = 0;
int uniqueelements = 1;
int runtime = 0;
int runtimesimu = 1;
int ntasks = 1;
long size;
char *buf;
char ptr[PATH_MAX];
int testeinput[10000];
int heightpanelcr = 1;
char orcachamada[200];
int missedddl[3][1024];
int missed = 0;
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 650;
    InitWindow(screenWidth, screenHeight, "ORCA-RT-BENCH: Kprofiller");
    Image icon = LoadImage("resources/none_16x16.png");
    SetWindowIcon(icon);
    //font arial
    Font font = LoadFont("resources/arial.ttf");
    //get current path
    char ptr[PATH_MAX];
    if (getcwd(ptr, sizeof(ptr)) != NULL)
    {
        strcat(ptr, "\\output.orca");
    }
    //checkboxes
    Color checkboxSelectedRM = BLACK;
    Color checkboxSelectedEDF = BLACK;
    Color checkboxSelectedLST = BLACK;
    Color checkboxSelectedLLF = BLACK;
    Color checkboxSelectedDM = BLACK;
    char *edfAlgorithm = "The earlier the deadline of a task, the higher is its priority.";
    char *rmAlgorithm = "The higher the frequency (1/period) of a task, the higher is its priority.";
    char *lstAlgorithm = "The least amount of slack time of a task, the higher is its priority.";
    char *llfAlgorithm = "The least amount of laxity time (deadline - capacity) of a task, the higher is its priority.";
    char *dmAlgorithm = "The latest the deadline of a task, the higher is its priority.";
    bool checkboxEDF = false;
    bool checkboxLST = false;
    bool checkboxRM = false;
    bool checkboxLFF = false;
    bool checkboxDM = false;
    int count = 0;
    char *schedulingAlgortihm = "";
    char *schedulingAlgortihmTxt = "";
    //performance analysis
    double performanceAnalysisResult = 0;
    //run time text box
    char runTimeMS[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for line ending char '\0'
    int letterCount = 0;
    Rectangle textBox = {screenWidth * 0.75 - 75, 60, 120, 40};
    bool mouseOnText = false;
    bool mouseonTextBox = false;
    int framesCounter = 0;
    //close buttons
    bool closeButton = false;
    bool running = false;
    //colors for simulation
    Color colors[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE};
    //escalotest area
    bool escTest = false;
    double testresult = 0;
    bool showtest = false;
    char *edftesttxt = "if < 1, then the system is schedulable with EDF";
    double rmtesttxt;
    //simulation scrollbar area
    Rectangle panelRec = {14, 254, (GetScreenWidth() - 29), 388};
    Rectangle panelContentRec = {14, 255, GetScreenWidth() * runtimesimu, 96 * heightpanelcr};
    Vector2 panelScroll = {99, 0};
    //Deadlines missed scrollbar area
    Rectangle panelRec2 = {14, 114, screenWidth / 2 - 31, 129};
    Rectangle panelContentRec2 = {15, 115, screenWidth / 2 - 43, 600};
    Vector2 panelScroll2 = {99, 0};

    SetTargetFPS(60);
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
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if (((key >= 48) && (key <= 57)) && (letterCount < MAX_INPUT_CHARS))
                {
                    runTimeMS[letterCount] = (char)key;
                    letterCount++;
                }
                key = GetCharPressed();
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
        DrawTextEx(font, runTimeMS, (Vector2){textBox.x + 7, textBox.y}, 48, 4, MAROON);
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
                    DrawTextEx(font, "_", (Vector2){textBox.x + 10 + MeasureText(runTimeMS, 48), textBox.y}, 40, 2, MAROON);
            }
        }
        // Dropped File Area
        //---------------------------------------------------------------------------------
        DrawRectangleRounded((Rectangle){15, 15, screenWidth / 2 - 30, 90}, 0.1f, 1, Fade(DARKPURPLE, 0.3f));
        if (count == 0)
            DrawTextEx(font, "Drop your data file here", (Vector2){20, 20}, 20, 2, DARKGRAY);
        else
        {
            DrawTextEx(font, "Dropped file:", (Vector2){20, 20}, 20, 2, DARKGRAY);
            DrawRectangleRounded((Rectangle){20, 45, screenWidth / 2 - 40, 55}, 0.1f, 1, Fade(DARKGRAY, 0.3f));
            DrawTextRec(font, droppedFiles[0], (Rectangle){25, 50, screenWidth / 2 - 40, 60}, 20, 2, true, DARKGRAY);
        }
        //Scheduling running time area
        //----------------------------------------------------------------------------------------
        DrawRectangleRounded((Rectangle){screenWidth / 2 + 15, 15, screenWidth / 2 - 30, 90}, 0.1f, 1, Fade(DARKPURPLE, 0.3f));
        DrawTextEx(font, "Input the simulation time ", (Vector2){screenWidth / 2 + 20, 20}, 20, 2, DARKGRAY);
        DrawTextEx(font, "Time Units", (Vector2){screenWidth * 0.75 + 60, 85}, 20, 2, DARKGRAY);
        //----------------------------------------------------------------------------------------
        //scheduling algorithm area design
        //----------------------------------------------------------------------------------------
        if (!running)
        {
            DrawRectangleRounded((Rectangle){14, 114, GetScreenWidth() / 2 - 29, 51}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
            DrawRectangleRoundedLines((Rectangle){15, 115, GetScreenWidth() / 2 - 30, 50}, 0.1f, 1, 1, checkboxSelectedRM);
            DrawTextRec(font, rmAlgorithm, (Rectangle){100, 118, GetScreenWidth() / 2 - 120, 220}, 16, 2, true, DARKGRAY);

            DrawRectangleRounded((Rectangle){14, 169, GetScreenWidth() / 2 - 29, 51}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
            DrawRectangleRoundedLines((Rectangle){15, 170, GetScreenWidth() / 2 - 30, 50}, 0.1f, 1, 1, checkboxSelectedEDF);
            DrawTextRec(font, edfAlgorithm, (Rectangle){100, 185, GetScreenWidth() / 2 - 120, 220}, 16, 2, true, DARKGRAY);

            DrawRectangleRounded((Rectangle){14, 224, GetScreenWidth() / 2 - 29, 51}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
            DrawRectangleRoundedLines((Rectangle){15, 225, GetScreenWidth() / 2 - 30, 50}, 0.1f, 1, 1, checkboxSelectedLST);
            DrawTextRec(font, lstAlgorithm, (Rectangle){100, 228, GetScreenWidth() / 2 - 120, 220}, 16, 2, true, DARKGRAY);

            DrawRectangleRounded((Rectangle){screenWidth / 2 + 15, 114, GetScreenWidth() / 2 - 29, 51}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
            DrawRectangleRoundedLines((Rectangle){screenWidth / 2 + 15, 115, GetScreenWidth() / 2 - 30, 50}, 0.1f, 1, 1, checkboxSelectedLLF);
            DrawTextRec(font, llfAlgorithm, (Rectangle){screenWidth / 2 + 90, 118, GetScreenWidth() / 2 - 120, 220}, 16, 2, true, DARKGRAY);

            DrawRectangleRounded((Rectangle){screenWidth / 2 + 15, 169, GetScreenWidth() / 2 - 29, 51}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
            DrawRectangleRoundedLines((Rectangle){screenWidth / 2 + 15, 170, GetScreenWidth() / 2 - 30, 50}, 0.1f, 1, 1, checkboxSelectedDM);
            DrawTextRec(font, dmAlgorithm, (Rectangle){screenWidth / 2 + 90, 185, GetScreenWidth() / 2 - 120, 220}, 16, 2, true, DARKGRAY);

            //  Scheduling Algorithms Checkboxes
            //-----------------------------------------------------------------------------------------
            if (GuiCheckBox((Rectangle){30, 130, 20, 20}, "RM", checkboxRM))
            {
                checkboxEDF = false;
                checkboxLST = false;
                checkboxRM = true;
                checkboxLFF = false;
                checkboxDM = false;
                checkboxSelectedRM = GOLD;
                checkboxSelectedEDF = BLACK;
                checkboxSelectedLST = BLACK;
                checkboxSelectedLLF = BLACK;
                checkboxSelectedDM = BLACK;
                schedulingAlgortihm = "RM";
                schedulingAlgortihmTxt = rmAlgorithm;
            }
            if (GuiCheckBox((Rectangle){30, 185, 20, 20}, "EDF", checkboxEDF))
            {
                checkboxEDF = true;
                checkboxLST = false;
                checkboxRM = false;
                checkboxLFF = false;
                checkboxDM = false;
                checkboxSelectedRM = BLACK;
                checkboxSelectedEDF = GOLD;
                checkboxSelectedLST = BLACK;
                checkboxSelectedLLF = BLACK;
                checkboxSelectedDM = BLACK;
                schedulingAlgortihm = "EDF";

                schedulingAlgortihmTxt = edfAlgorithm;
            }
            if (GuiCheckBox((Rectangle){30, 240, 20, 20}, "LST", checkboxLST))
            {
                checkboxEDF = false;
                checkboxLST = true;
                checkboxRM = false;
                checkboxLFF = false;
                checkboxDM = false;
                checkboxSelectedRM = BLACK;
                checkboxSelectedEDF = BLACK;
                checkboxSelectedLST = GOLD;
                checkboxSelectedLLF = BLACK;
                checkboxSelectedDM = BLACK;
                schedulingAlgortihm = "LST";

                schedulingAlgortihmTxt = lstAlgorithm;
            }

            if (GuiCheckBox((Rectangle){screenWidth / 2 + 30, 130, 20, 20}, "LLF", checkboxLFF))
            {
                checkboxEDF = false;
                checkboxLST = false;
                checkboxRM = false;
                checkboxLFF = true;
                checkboxDM = false;
                checkboxSelectedRM = BLACK;
                checkboxSelectedEDF = BLACK;
                checkboxSelectedLST = BLACK;
                checkboxSelectedLLF = GOLD;
                checkboxSelectedDM = BLACK;
                schedulingAlgortihm = "LLF";

                schedulingAlgortihmTxt = llfAlgorithm;
            }
            if (GuiCheckBox((Rectangle){screenWidth / 2 + 30, 185, 20, 20}, "DM", checkboxDM))
            {
                checkboxEDF = false;
                checkboxLST = false;
                checkboxRM = false;
                checkboxLFF = false;
                checkboxDM = true;
                checkboxSelectedRM = BLACK;
                checkboxSelectedEDF = BLACK;
                checkboxSelectedLST = BLACK;
                checkboxSelectedLLF = BLACK;
                checkboxSelectedDM = GOLD;
                schedulingAlgortihm = "DM";
                schedulingAlgortihmTxt = dmAlgorithm;
            }
            //SCHEDULABILITY test Button
            //------------------------------------------------------------------------------------

            escTest = GuiButton((Rectangle){15, 590, 150, 50}, "TEST SCHEDULABILITY");
            if (escTest)
            {
                showtest = !showtest;
            }
            if (escTest && IsFileDropped() && strcmp("", schedulingAlgortihm) != 0)
            {
                testresult = schedtest(droppedFiles[0], 1);
            }
            if (showtest && IsFileDropped() && strcmp("", schedulingAlgortihm) != 0)
            {
                DrawRectangle(14, 280, 175, 60, Fade(DARKBLUE, 0.2f));
                DrawRectangleLinesEx((Rectangle){15, 280, 175, 60}, 2, BLACK);
                DrawTextEx(font, TextFormat("%03.04f", testresult), (Vector2){25, 285}, 50, 2, BLACK);
                if (checkboxEDF)
                    DrawTextEx(font, edftesttxt, (Vector2){195, 315}, 24, 2, BLACK);
                else if (checkboxRM)
                {
                    rmtesttxt = ntasks * (pow(2.0, (1.0 / ntasks)) - 1.0);
                    DrawTextEx(font, TextFormat("if <= %02.02f, then the system is schedulable with RM", rmtesttxt), (Vector2){195, 315}, 24, 2, BLACK);
                }
            }
        }
        //Run Button && Close button
        //------------------------------------------------------------------------------------
        runtime = atoi(runTimeMS);
        if (!running)
        {
            if (GuiButton((Rectangle){GetScreenWidth() - 115, 590, 100, 50}, "RUN") && IsFileDropped() && runtime > 0 && strcmp("", schedulingAlgortihm) != 0)
            {
                strcpy(orcachamada, "C:/Users/jbweb/Orca_Bolsa/orca-rt-bench/bin/orca-rt-scheduler.exe");
                strcat(orcachamada, " ");
                strcat(orcachamada, runTimeMS);
                strcat(orcachamada, " ");
                strcat(orcachamada, droppedFiles[0]);
                strcat(orcachamada, " ");
                strcat(orcachamada, schedulingAlgortihm);
                printf(orcachamada);
                system(orcachamada);
                outputOrca(ptr, runtime);
                closeButton = false;
                running = true;

                //updates the scrollbar width
                if (runtime <= 51)
                    panelContentRec.width = GetScreenWidth() - 29;
                else
                    panelContentRec.width = 130 + 20 * runtime;

                uniqueelements = largest(testeinput, (sizeof(testeinput) / sizeof(testeinput[0])));
                if (uniqueelements >= 4 && uniqueelements % 4 == 0)
                {
                    ntasks = (uniqueelements / 4);
                }
                else if (uniqueelements >= 4 && uniqueelements % 4 != 0)
                {
                    ntasks = (uniqueelements / 4) + 1;
                }
                else
                {
                    ntasks = 1;
                }
                //updates the scrollbar height
                int heightpanelcr;
                if (uniqueelements < 4)
                    heightpanelcr = 4;
                else
                    heightpanelcr = uniqueelements + 1;
                panelContentRec.height = 94 * heightpanelcr;

                if (missed < 3)
                    panelContentRec2.height = 129;
                else
                    panelContentRec2.height = 60 + missed * 25;

                strcpy(orcachamada, "");
                performanceAnalysisResult = (schedtest(droppedFiles[0], 2) + runtime / 1) * ((uniqueelements) * (log2(uniqueelements)) + 4);
            }
        }
        if (closeButton)
        {
            strcpy(orcachamada, "");
            running = false;
            memset(testeinput, 0, sizeof(testeinput));
        }
        //Scheduling simulation area
        //-----------------------------------------------------------------------------------
        if (running)
        {
            //choosen algorithm
            DrawRectangleRounded((Rectangle){screenWidth / 2 + 15, 115, screenWidth / 2 - 30, 130}, 0.1f, 1, Fade(DARKGRAY, 0.3f));
            DrawTextEx(font, schedulingAlgortihm, (Vector2){screenWidth - (screenWidth / 4) - 15, 125}, 40, 2, BLACK);
            DrawTextRec(font, schedulingAlgortihmTxt, (Rectangle){(screenWidth - screenWidth / 2) + 30, 170, screenWidth / 2 - 80, 245}, 18, 2, true, BLACK);
            DrawTextRec(font, TextFormat("Performance Analysis: %02.02f ", performanceAnalysisResult), (Rectangle){(screenWidth - screenWidth / 2) + 30, 200, (screenWidth - screenWidth / 2) - 35, 245}, 18, 2, true, BLACK);
            //deadlines missed
            Rectangle view2 = GuiScrollPanel(panelRec2, panelContentRec2, &panelScroll2);
            BeginScissorMode(view2.x, view2.y, view2.width, view2.height);
            DrawRectangleRounded((Rectangle){15, 115, screenWidth / 2 - 30, 130}, 0.1f, 1, Fade(DARKGRAY, 0.3f));
            DrawTextEx(font, "Missed Deadlines", (Vector2){20, 10 + panelRec2.y + panelScroll2.y}, 20, 2, BLACK);
            for (int i = 0; i < missed; i++)
            {
                DrawTextEx(font, TextFormat("task [%d] - deadline [%d] violated - task ended at time unit = [%d]", missedddl[0][i], missedddl[1][i], missedddl[2][i]), (Vector2){20, 40 + 30 * i + panelRec2.y + panelScroll2.y}, 16, 2, BLACK);
            }
            EndScissorMode();
            //simulation draw
            Rectangle view = GuiScrollPanel(panelRec, panelContentRec, &panelScroll);
            BeginScissorMode(view.x, view.y, view.width, view.height);
            DrawRectangle(14, 255, GetScreenWidth() * runtimesimu, 495 - 110, Fade(LIGHTGRAY, 0.2f));
            DrawRectangleLinesEx((Rectangle){14, 254, (GetScreenWidth() - 29), 388}, 4, BLACK);

            DrawTextEx(font, "Scheduler", (Vector2){20 + panelRec.x + panelScroll.x, 80 + panelRec.y + panelScroll.y}, 20, 2, BLACK);
            DrawLine(120 + panelRec.x + panelScroll.x, 100 + panelRec.y + panelScroll.y, 120 + 20 * runtime + panelRec.x + panelScroll.x, 100 + panelRec.y + panelScroll.y, BLACK);

            for (int i = 0; i < uniqueelements; i++)
            {
                DrawTextEx(font, TextFormat("Task %02i", i + 1), (Vector2){20 + panelRec.x + panelScroll.x, 160 + 80 * i + panelRec.y + panelScroll.y}, 20, 2, BLACK);
                DrawLine(120 + panelRec.x + panelScroll.x, 180 + 80 * i + panelRec.y + panelScroll.y, 120 + 20 * runtime + panelRec.x + panelScroll.x, 180 + 80 * i + panelRec.y + panelScroll.y, BLACK);
            }
            for (int i = 0; i < runtime; i++)
            {
                if (testeinput[i] < 100)
                {
                    DrawRectangle(120 + 20 * i + panelRec.x + panelScroll.x, 80 + 80 * testeinput[i] + panelRec.y + panelScroll.y, 20, 20, colors[testeinput[i]]);
                    DrawRectangleLinesEx((Rectangle){120 + 20 * i + panelRec.x + panelScroll.x, 80 + 80 * testeinput[i] + panelRec.y + panelScroll.y, 20, 20}, 1, BLACK);
                }
            }
            for (int i = 0; i <= runtime / 5; i++)
            {
                DrawTextEx(font, TextFormat("%02i", 5 * i), (Vector2){122 + 20 * i * 5 + panelRec.x + panelScroll.x, 32 + 80 * (uniqueelements + 1) + panelRec.y + panelScroll.y}, 16, 2, BLACK);
                DrawLine(120 + 20 * i * 5 + panelRec.x + panelScroll.x, 100 + panelRec.y + panelScroll.y, 120 + (20 * i * 5) + panelRec.x + panelScroll.x, 48 + 80 * (uniqueelements + 1) + panelRec.y + panelScroll.y, BLACK);
            }
            closeButton = GuiButton((Rectangle){GetScreenWidth() - 119, 258, 100, 50}, "CLOSE");
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
// Verifica se alguma tecla número está pressionada
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();
    if (((key >= 48) && (key <= 57)))
        keyPressed = true;

    return keyPressed;
}
// Verifica quantos elementos únicos existem no vetor output.orca
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
    for (int i = 1; i < n; i++)
    {
        if (arr1[i] == 0)
        {
            res--;
            break;
        }
    }
    return res;
}
//le o arquivo output.orca
int *outputOrca(char *fileoutput, int runtime)
{
    int i = 0;
    char line_buf[255];
    char *token;
    char *deadline = 0;
    char *finish = 0;

    FILE *file = fopen(fileoutput, "r");
    if (file == NULL)
    {
        perror("file vazia");
        return 0;
    }
    while (fgets(line_buf, 255, file) != NULL)
    {
        if (i == runtime)
            break;

        token = strtok(line_buf, " ");
        if (atoi(token) < 50)
        {
            testeinput[i] = atoi(token);
            i++;
        }
        if (i > 1 && testeinput[i - 1] != testeinput[i - 2] && (atoi(deadline) < atoi(finish)))
        {
            missedddl[0][missed] = testeinput[i - 2];
            missedddl[1][missed] = atoi(deadline);
            missedddl[2][missed] = atoi(finish);
            missed++;
        }
        deadline = strtok(NULL, " ");
        token = strtok(NULL, " ");
        finish = strtok(NULL, " ");
    }
    nelementos = i;
    uniqueelements = UniqueElements(testeinput, nelementos);
    fclose(file);
    return 0;
}
//teste de escalonabilidade
double schedtest(char *fileoutput, int typeoftest)
{
    double i = 0;
    double n = 0;
    double m = 0;
    char line_buf[255];
    char *token;
    char *ptr;
    FILE *file = fopen(fileoutput, "r");
    if (file == NULL)
    {
        perror("file vazia");
        return 0;
    }
    while (fgets(line_buf, 255, file) != NULL)
    {
        token = strtok(line_buf, " ");
        if (strstr(token, "#id") != NULL)
        {
            fgets(line_buf, 255, file);
            token = strtok(line_buf, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            n = strtod(token, &ptr);
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            m = strtod(token, &ptr);
            if (typeoftest == 1)
            {
                i += n / m;
            }
            else if (typeoftest == 2)
            {
                i += runtime / m;
            }
            fgets(line_buf, 255, file);
            while (strstr(token, "#") == NULL)
            {
                token = strtok(line_buf, " ");
                if (strstr(token, " ") != NULL || strstr(token, "#") != NULL || strstr(token, "\n") != NULL)
                {
                    break;
                }
                token = strtok(NULL, " ");
                token = strtok(NULL, " ");
                n = strtod(token, &ptr);
                token = strtok(NULL, " ");
                token = strtok(NULL, " ");
                token = strtok(NULL, " ");
                m = strtod(token, &ptr);
                if (typeoftest == 1)
                {
                    i += n / m;
                }
                else if (typeoftest == 2)
                {
                    i += runtime / m;
                }
                ntasks++;
                fgets(line_buf, 255, file);
            }
            break;
        }
    }
    return i;
}
//encontra a task de maior id dentro do output.orca
int largest(int arr[], int n)
{
    int i;
    int max = arr[0];
    for (i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}