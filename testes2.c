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

#define MAX_INPUT_CHARS 9

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Testes - input box");

    char name[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for line ending char '\0'
    int letterCount = 0;

    Rectangle textBox = {screenWidth / 2 - 100, 180, 225, 50};
    bool mouseOnText = false;
    bool mouseonTextBox = false;
    int framesCounter = 0;

    SetTargetFPS(10); // Set our game to run at 10 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
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
                if ((((key >= 48) && (key <= 57)) || ((key >= 96 && key <= 105))) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    letterCount++;
                }

                key = GetCharPressed(); // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0)
                    letterCount = 0;
                name[letterCount] = '\0';
            }
        }
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseonTextBox)
            framesCounter++;
        else
            framesCounter = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("PLACE MOUSE OVER INPUT BOX!", 240, 140, 20, GRAY);

        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)
            DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
        else
            DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, DARKGRAY);

        DrawText(name, textBox.x + 5, textBox.y + 8, 40, MAROON);

        DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_INPUT_CHARS), 315, 250, 20, DARKGRAY);
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouseOnText)
        {
            mouseonTextBox = !mouseonTextBox;
        }

        if (mouseonTextBox)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", textBox.x + 8 + MeasureText(name, 40), textBox.y + 12, 40, MAROON);
            }
            else
                DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if (((key >= 48) && (key <= 57)) || ((key >= 96) && (key <= 105)))
        keyPressed = true;

    return keyPressed;
}