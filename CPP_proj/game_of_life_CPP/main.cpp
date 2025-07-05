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

}

// Initialize game variables
void InitGame(void)
{
   
}

// Update game (one frame)
void UpdateGame(void)
{
   
}

// Draw game (one frame)
void DrawGame(void)
{

}

// Update screen
void ScreenUpdate(void)
{


}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{   

}



