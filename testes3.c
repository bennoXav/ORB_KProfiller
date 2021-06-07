/*******************************************************************************************
*
*   raylib [core] example - Windows drop files
*
*   This example only works on platforms that support drag & drop (Windows, Linux, OSX, Html5?)
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <unistd.h>
#include <string.h>
#define MAX_INPUT_CHARS 3
#define MAX_COLORS_COUNT 21
int UniqueElements(int arr1[], int n);
int *outputOrca(char *fileoutput, int runtime);

char **droppedFiles = {0};
char **fileoutput = {0};
size_t nelementos = 0;
int uniqueelements = 1;
int runtime = 50;
int runtimesimu = 1;
long size;
char *buf;
char ptr[200];
int testeinput[999];

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    char orcachamada[200];

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
    char *schedulingAlgortihm = "";
    //input file

    buf = (char *)malloc((size_t)size);
    strcpy(ptr, getcwd(buf, (size_t)PATH_MAX));
    strcat(ptr, "/output.orca");
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
    //colors for simulation
    Color colors[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE};
    Rectangle panelRec = {14, 109, (GetScreenWidth() - 29), 388};
    Rectangle panelContentRec = {14, 110, GetScreenWidth() * runtimesimu, 373};
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
        DrawText(rmAlgorithm, 100, 512, 14, DARKGRAY);
        DrawRectangleRounded((Rectangle){14, 544, GetScreenWidth() - 29, 41}, 0.01f, 1, Fade(DARKPURPLE, 0.2f));
        DrawRectangleRoundedLines((Rectangle){15, 545, GetScreenWidth() - 30, 40}, 0.1f, 1, 1, checkboxSelectedEDF);
        DrawText(edfAlgorithm, 100, 557, 14, DARKGRAY);
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
            schedulingAlgortihm = "RM";
        }
        if (GuiCheckBox((Rectangle){30, 555, 20, 20}, "EDF", checkboxEDF))
        {
            checkboxEDF = true;
            checkboxLST = false;
            checkboxRM = false;

            checkboxSelectedRM = BLACK;
            checkboxSelectedEDF = GOLD;
            checkboxSelectedLST = BLACK;

            schedulingAlgortihm = "EDF";
        }
        if (GuiCheckBox((Rectangle){30, 600, 20, 20}, "LST", checkboxLST))
        {
            checkboxEDF = false;
            checkboxLST = true;
            checkboxRM = false;
            checkboxSelectedRM = BLACK;
            checkboxSelectedEDF = BLACK;
            checkboxSelectedLST = GOLD;

            schedulingAlgortihm = "LST";
        }

        //Run Button && Close button
        //------------------------------------------------------------------------------------

        runButton = GuiButton((Rectangle){GetScreenWidth() - 115, 445, 100, 50}, "RUN");

        if (runButton && IsFileDropped() && runtime != 0 && sizeof(schedulingAlgortihm) != 0)
        {
            runtime = atoi(runTimeMS);

            if (runtime > 50)
                runtimesimu = (runtime / 50) + 1;
            else
                runtimesimu = 1;
            panelContentRec.width = GetScreenWidth() * runtimesimu;
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
        }
        if (closeButton)
        {
            strcpy(orcachamada, "");
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
            for (int i = 0; i < uniqueelements - 1; i++)
            {
                DrawText(TextFormat("Task %02i", i + 1), 20 + panelRec.x + panelScroll.x, 200 + 80 * i, 20, BLACK);
                DrawLine(120 + panelRec.x + panelScroll.x, 220 + 80 * i, ((GetScreenWidth() - 29) * runtimesimu) + panelRec.x + panelScroll.x, 220 + 80 * i, BLACK);
            }
            for (int i = 0; i < (sizeof(testeinput) / sizeof(testeinput[0])); i++)
            {
                if (testeinput[i] > 0 && testeinput[i] < 100)
                {
                    DrawRectangle(120 + 20 * i + panelRec.x + panelScroll.x, 200 + 80 * (testeinput[i] - 1), 20, 20, colors[testeinput[i]]);
                    DrawRectangleLinesEx((Rectangle){120 + 20 * i + panelRec.x + panelScroll.x, 200 + 80 * (testeinput[i] - 1), 20, 20}, 1, BLACK);
                }
            }
            for (int i = 0; i <= runtime / 5; i++)
            {
                DrawText(TextFormat("%02i", 5 * i), 122 + 20 * i * 5 + panelRec.x + panelScroll.x, 470, 1, BLACK);
                DrawLine(120 + 20 * i * 5 + panelRec.x + panelScroll.x, 220, 120 + 20 * i * 5 + panelRec.x + panelScroll.x, 480, BLACK);
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

int *outputOrca(char *fileoutput, int runtime)
{
    int i = 0;
    char line_buf[255];
    char *token;
    FILE *file = fopen(fileoutput, "r");
    if (file == NULL)
    {
        perror("file vazia");
        return 0;
    }
    while (fgets(line_buf, 255, file) != NULL)
    {
        if (i == runtime)
        {
            break;
        }
        token = strtok(line_buf, " ");
        testeinput[i] = atoi(token);
        i++;
    }
    nelementos = i;
    uniqueelements = UniqueElements(testeinput, nelementos);
    fclose(file);
    return 0;
}
