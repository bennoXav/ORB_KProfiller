/*******************************************************************************************
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#define _GNU_SOURCE
#define NUM_FRAMES 3
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//functions declaration
void orcaOutput(char *fileChooser);
int drawingPanelButtonAndFileDropped();
int drawingPanelVetor();
int drawingPanelButtons();
int *outputOrca(char **fileDropped);
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Testes Gerais - Buttons/dropfile/vectors");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    drawingPanelButtonAndFileDropped(screenWidth, screenHeight);
    return 0;
}

int *outputOrca(char **fileDropped)
{
    int i = 0;
    int retorno[255];
    char line_buf[255];
    char *token;
    FILE *file = fopen(fileDropped[0], "r");
    if (file == NULL)
    {
        perror("file vazia");
        return NULL;
    }
    fgets(line_buf, 255, file);
    while (fgets(line_buf, 255, file) != NULL)
    {
        token = strtok(line_buf, " ");
        retorno[i] = atoi(token);
        i++;
    }

    fclose(file);
    return retorno;
}

int drawingPanelButtonAndFileDropped(int screenWidth, int screenHeight)
{
    int count = 0;
    char **droppedFiles;
    int *outputorcafoda;
    //Teste Botões
    Texture2D button = LoadTexture("resources/button.png");
    //Button
    float frameHeight = (float)button.height / NUM_FRAMES;
    Rectangle sourceRec = {0, 0, (float)button.width, frameHeight};
    bool isInMenu = true;
    // Define button bounds on screen
    Rectangle btnBounds = {screenWidth / 2.0f - button.width / 2.0f, screenHeight / 2.0f - button.height / NUM_FRAMES / 2.0f, (float)button.width, frameHeight};
    int btnState = 0;       // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false; // Button action should be activated

    Vector2 mousePoint = {0.0f, 0.0f};

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        mousePoint = GetMousePosition();
        btnAction = false;

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                btnState = 2;
            else
                btnState = 1;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                btnAction = true;
        }
        else
            btnState = 0;

        if (btnAction)
        {
            printf("Button Pressed");
        }
        // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState * frameHeight;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        if (isInMenu)
        {
            if (btnAction)
                isInMenu = false;
        }
        else
        {
            if (btnAction)
                isInMenu = true;
        }
        if (IsFileDropped())
        {
            droppedFiles = GetDroppedFiles(&count);
            outputorcafoda = outputOrca(droppedFiles);
            ClearDroppedFiles();
        }
        BeginDrawing();
        if (isInMenu)
        {
            ClearBackground(RAYWHITE);

            DrawTextureRec(button, sourceRec, (Vector2){btnBounds.x, btnBounds.y}, WHITE); // Draw button frame
        }
        else
        {
            ClearBackground(BLACK);

            DrawTextureRec(button, sourceRec, (Vector2){btnBounds.x, btnBounds.y}, WHITE); // Draw button frame
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    UnloadTexture(button);
    CloseWindow(); // Close window and OpenGL context
    return 0;
}

int drawingPanelVetor()
{
    Texture2D texture2 = LoadTexture("resources/checked.png");
    Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);

    Model models;

    models = LoadModelFromMesh(GenMeshPlane(10, 10, 5, 5));

    // Set checked texture as default diffuse component for all models material
    models.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture2;

    Camera camera = {{0.1f, 10.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.1f, 0.0f, 0.0f}, 50.0f, 0};
    Vector3 position = {1.0f, 0.0f, 0.0f};

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateCamera(&camera); // Update internal camera and our camera

        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawModel(models, position, 1.0f, WHITE);
        DrawGrid(10, 1.0);

        EndMode3D();

        DrawRectangle(30, 400, 310, 30, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(30, 400, 310, 30, Fade(DARKBLUE, 0.5f));

        DrawText("PLANE", 680, 10, 20, DARKBLUE);

        EndDrawing();
    }
    UnloadTexture(texture); // Unload texture
    UnloadModel(models);
    CloseWindow();
    return 0;
}

int drawingPanelButtons()
{
    float innerRadius = 80.0f;
    float outerRadius = 190.0f;
    Vector2 center = {(GetScreenWidth() - 300) / 2, GetScreenHeight() / 2};
    float startAngle = 0.0f;
    float endAngle = 360.0f;
    int segments = 0;
    int minSegments = 4;

    bool drawRing = true;
    bool drawRingLines = false;
    bool drawCircleLines = false;
    bool bottao = false;
    bool isInMenu = false;
    // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // NOTE: All variables update happens inside GUI control functions
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        if (isInMenu)
        {
            if (bottao)
                isInMenu = false;
        }
        else
        {
            if (bottao)
                isInMenu = true;
        }

        DrawLine(500, 0, 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.6f));
        DrawRectangle(500, 0, GetScreenWidth() - 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.3f));

        if (drawRing)
            DrawRing(center, innerRadius, outerRadius, startAngle, endAngle, segments, Fade(MAROON, 0.3));
        if (drawRingLines)
            DrawRingLines(center, innerRadius, outerRadius, startAngle, endAngle, segments, Fade(BLACK, 0.4));
        if (drawCircleLines)
            DrawCircleSectorLines(center, outerRadius, startAngle, endAngle, segments, Fade(BLACK, 0.4));
        if (isInMenu)
        {
            ClearBackground(RAYWHITE);
        }
        else
        {
            ClearBackground(BLACK);
        }
        // Draw GUI controls
        //------------------------------------------------------------------------------
        startAngle = GuiSliderBar((Rectangle){600, 40, 120, 20}, "StartAngle", NULL, startAngle, -450, 450);
        endAngle = GuiSliderBar((Rectangle){600, 70, 120, 20}, "EndAngle", NULL, endAngle, -450, 450);

        innerRadius = GuiSliderBar((Rectangle){600, 140, 120, 20}, "InnerRadius", NULL, innerRadius, 0, 100);
        outerRadius = GuiSliderBar((Rectangle){600, 170, 120, 20}, "OuterRadius", NULL, outerRadius, 0, 200);

        segments = GuiSliderBar((Rectangle){600, 240, 120, 20}, "Segments", NULL, segments, 0, 100);

        drawRing = GuiCheckBox((Rectangle){600, 320, 20, 20}, "Draw Ring", drawRing);
        drawRingLines = GuiCheckBox((Rectangle){600, 350, 20, 20}, "Draw RingLines", drawRingLines);
        drawCircleLines = GuiCheckBox((Rectangle){600, 380, 20, 20}, "Draw CircleLines", drawCircleLines);
        bottao = GuiButton((Rectangle){600, 410, 20, 20}, "x");
        //------------------------------------------------------------------------------

        int minSegments = (int)ceilf((endAngle - startAngle) / 90);
        DrawText(TextFormat("MODE: %s", (segments >= minSegments) ? "MANUAL" : "AUTO"), 600, 270, 10, (segments >= minSegments) ? MAROON : DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
