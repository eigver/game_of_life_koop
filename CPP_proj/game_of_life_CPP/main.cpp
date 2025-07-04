/*
liberes ..\game_of_life_CPP\packages\raylib-5.5_win64_msvc16\raygui-4.0;
        ..\game_of_life_CPP\packages\raylib-5.5_win64_msvc16\lib
        raygui.lib;raylib.lib;raylibdll.lib;winmm.lib;




*/

#define _CRT_SECURE_NO_WARNINGS
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_CUSTOM_ICONS     // Custom icons set required 
#include "../game_of_life_CPP/packages/raylib-5.5_win64_msvc16/raygui-4.0/icons/game_of_life_icons.h"
#include "../game_of_life_CPP/packages/raylib-5.5_win64_msvc16/raygui-4.0/src/raygui.h"
#include "../game_of_life_CPP/packages/raylib-5.5_win64_msvc16/include/raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include "../game_of_life_CPP/packages/raylib-5.5_win64_msvc16/raygui-4.0/styles/dark/style_dark.h"
#include "../game_of_life_CPP/packages/raylib-5.5_win64_msvc16/raygui-4.0/styles/jungle/style_jungle.h"
using namespace std;

/*******************************************************************************************
*
*   raylib - classic game: snake
*
*   Sample game developed by Ian Eito, Albert Martos and Ramon Santamaria
*
*   This game has been created using raylib v1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
struct Snake {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
};

struct Food {
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
};
enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING, SETTINGS };

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int DefScreenWidth = 800;
static const int DefScreenHeight = 450;
GameScreen currentScreen = LOGO;
static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;
bool fullscreen = false;
static int monitorWidth = GetScreenWidth();
static int monitorHeight = GetScreenHeight();
static int CurrentScreenWidth = DefScreenWidth;
static int CurrentScreenHeight = DefScreenHeight;
int visualStyleActive = 2;
int prevVisualStyleActive = 2;

static Food fruit = { 0 };
static Snake snake[SNAKE_LENGTH] = { 0 };
static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
static bool allowMove = false;
static Vector2 offset = { 0 };
static int counterTail = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void ScreenUpdate(void);
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)
static void DrawSetingsGui(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(CurrentScreenWidth, CurrentScreenHeight, "game of life");
    InitGame();
    


    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

void DrawSetingsGui(void)
{
    int setingsButtonWeidth = CurrentScreenWidth * 0.2;
    int setingsButtonHeight = CurrentScreenHeight * 0.15;
    int setingsButtonPosX = CurrentScreenWidth * 0.5 - setingsButtonWeidth * 0.5;
    int setingsButtonPosY = CurrentScreenHeight * 0.7 - setingsButtonHeight * 0.5;
    if (IsWindowFullscreen()) {
        if (GuiButton(Rectangle{ 24, 24, 120, 30,}, "#220#fullscreen")) ToggleFullscreen();
    }
    else {
        if (GuiButton(Rectangle{ 24, 24, 120, 30 }, "#53#fullscreen")) ToggleFullscreen();
    }
}

// Initialize game variables
void InitGame(void)
{
    framesCounter = 0;
    gameOver = false;
    pause = false;

    counterTail = 1;
    allowMove = false;

    offset.x = CurrentScreenWidth % SQUARE_SIZE;
    offset.y = CurrentScreenHeight % SQUARE_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = Vector2{ offset.x / 2, offset.y / 2 };
        snake[i].size = Vector2{ SQUARE_SIZE, SQUARE_SIZE };
        snake[i].speed = Vector2{ SQUARE_SIZE, 0 };

        if (i == 0) snake[i].color = DARKBLUE;
        else snake[i].color = BLUE;
    }

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snakePosition[i] = Vector2{ 0.0f, 0.0f };
    }

    fruit.size = Vector2{ SQUARE_SIZE, SQUARE_SIZE };
    fruit.color = SKYBLUE;
    fruit.active = false;
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            // Player control
            if (IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = Vector2{ SQUARE_SIZE, 0 };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = Vector2{ -SQUARE_SIZE, 0 };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = Vector2{ 0, -SQUARE_SIZE };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = Vector2{ 0, SQUARE_SIZE };
                allowMove = false;
            }

            // Snake movement
            for (int i = 0; i < counterTail; i++) snakePosition[i] = snake[i].position;

            if ((framesCounter % 5) == 0)
            {
                for (int i = 0; i < counterTail; i++)
                {
                    if (i == 0)
                    {
                        snake[0].position.x += snake[0].speed.x;
                        snake[0].position.y += snake[0].speed.y;
                        allowMove = true;
                    }
                    else snake[i].position = snakePosition[i - 1];
                }
            }

            // Wall behaviour
            if (((snake[0].position.x) > (CurrentScreenWidth - offset.x)) ||
                ((snake[0].position.y) > (CurrentScreenHeight - offset.y)) ||
                (snake[0].position.x < 0) || (snake[0].position.y < 0))
            {
                gameOver = true;
            }

            // Collision with yourself
            for (int i = 1; i < counterTail; i++)
            {
                if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y)) gameOver = true;
            }

            // Fruit position calculation
            if (!fruit.active)
            {
                fruit.active = true;
                fruit.position = Vector2{ GetRandomValue(0, (CurrentScreenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (CurrentScreenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2 };

                for (int i = 0; i < counterTail; i++)
                {
                    while ((fruit.position.x == snake[i].position.x) && (fruit.position.y == snake[i].position.y))
                    {
                        fruit.position = Vector2{ GetRandomValue(0, (CurrentScreenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (CurrentScreenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2 };
                        i = 0;
                    }
                }
            }

            // Collision
            if ((snake[0].position.x < (fruit.position.x + fruit.size.x) && (snake[0].position.x + snake[0].size.x) > fruit.position.x) &&
                (snake[0].position.y < (fruit.position.y + fruit.size.y) && (snake[0].position.y + snake[0].size.y) > fruit.position.y))
            {
                snake[counterTail].position = snakePosition[counterTail - 1];
                counterTail += 1;
                fruit.active = false;
            }

            framesCounter++;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            currentScreen = GAMEPLAY;
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    if (visualStyleActive != prevVisualStyleActive)
    {
        GuiLoadStyleDefault();

        switch (visualStyleActive)
        {
        case 0: break;      // Default style
        case 1: GuiLoadStyleJungle(); break;
        case 2: GuiLoadStyleDark(); break;
        default: break;
        }

        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

        prevVisualStyleActive = visualStyleActive;
    }

    switch (currentScreen)
    {
    case LOGO:
    {
        // TODO: Draw LOGO screen here!
        DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
        DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

    } break;
    case TITLE:
    {
        // TODO: Draw TITLE screen here!

        
        DrawRectangle(0, 0, CurrentScreenWidth, CurrentScreenHeight, GREEN);
        DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
        float setingsButtonWeidth = CurrentScreenWidth * 0.2;
        float setingsButtonHeight = CurrentScreenHeight * 0.15;
        float setingsButtonPosX = CurrentScreenWidth * 0.5 - setingsButtonWeidth * 0.5;
        float setingsButtonPosY = CurrentScreenHeight * 0.7 - setingsButtonHeight * 0.5;
        if (GuiButton(Rectangle{ setingsButtonPosX, setingsButtonPosY, setingsButtonWeidth, setingsButtonHeight, }, "#221#settings")) 
        {
            visualStyleActive = 2;
            currentScreen = SETTINGS;
        }

    } break;
    case GAMEPLAY:
    {
        // TODO: Draw GAMEPLAY screen here!
        if (!gameOver)
        {
            // Draw grid lines
            for (int i = 0; i < CurrentScreenWidth / SQUARE_SIZE + 1; i++)
            {
                DrawLineV(Vector2{ SQUARE_SIZE * i + offset.x / 2, offset.y / 2 }, Vector2{ SQUARE_SIZE * i + offset.x / 2, CurrentScreenHeight - offset.y / 2 }, LIGHTGRAY);
            }

            for (int i = 0; i < CurrentScreenHeight / SQUARE_SIZE + 1; i++)
            {
                DrawLineV(Vector2{ offset.x / 2, SQUARE_SIZE * i + offset.y / 2 }, Vector2{ CurrentScreenWidth - offset.x / 2, SQUARE_SIZE * i + offset.y / 2 }, LIGHTGRAY);
            }

            // Draw snake
            for (int i = 0; i < counterTail; i++) DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);

            // Draw fruit to pick
            DrawRectangleV(fruit.position, fruit.size, fruit.color);

            if (pause) DrawText("GAME PAUSED", CurrentScreenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, CurrentScreenHeight / 2 - 40, 40, GRAY);
        }
        else currentScreen = ENDING;
       

    } break;
    case ENDING:
    {
        // TODO: Draw ENDING screen here!
        DrawRectangle(0, 0, CurrentScreenWidth, CurrentScreenHeight, BLUE);
        DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
        DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

    } break;
    case SETTINGS:
    {
        DrawRectangle(0, 0, CurrentScreenWidth, CurrentScreenHeight, GRAY);
        DrawText("SETTINGS", 20, 20, 40, DARKGRAY);
        DrawText("PRESS ENTER", 120, 220, 20, DARKGRAY);
        DrawSetingsGui();

    }break;
    default: break;
    }
    



    EndDrawing();
}

// Update screen
void ScreenUpdate(void)
{
    switch (currentScreen)
    {
    case LOGO:
    {
        // TODO: Update LOGO screen variables here!

        framesCounter++;    // Count frames

        // Wait for 2 seconds (120 frames) before jumping to TITLE screen
        if (framesCounter > 120)
        {   
            visualStyleActive = 1;
            currentScreen = TITLE;
        }
    } break;
    case TITLE:
    {
        // TODO: Update TITLE screen variables here!
        InitGame();

        // Press enter to change to GAMEPLAY screen
        if (IsKeyPressed(KEY_ENTER))
        {
            visualStyleActive = 0;
            currentScreen = GAMEPLAY;
        }
    } break;
    case GAMEPLAY:
    {
        // TODO: Update GAMEPLAY screen variables here!


        // Press enter to change to ENDING screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        {   
            visualStyleActive = 0;
            currentScreen = ENDING;
        }
    } break;
    case ENDING:
    {
        // TODO: Update ENDING screen variables here!

        // Press enter to return to TITLE screen
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        {
            visualStyleActive = 1;
            currentScreen = TITLE;
        }
    } break;
    case SETTINGS:
    {
        // TODO: Update ENDING screen variables here!

        // Press enter to return to TITLE screen
        if (IsKeyPressed(KEY_ENTER))
        {
            visualStyleActive = 1;
            currentScreen = TITLE;
        }
    } break;
    default: break;
    }

}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{   
    ScreenUpdate();
    DrawGame();
    UpdateGame();
}



