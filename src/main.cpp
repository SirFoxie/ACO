#pragma once

// STL
#include <random>
// GUI
#include "raylib.h"
#include "rlgl.h"
// User-Defined
#include "agent.hpp"
#include "world.hpp"

#define MAP_XPOS    20
#define MAP_YPOS    20
#define MAP_WIDTH   200
#define MAP_HEIGHT  200

#define START_XPOS    MAP_XPOS
#define START_YPOS    MAP_YPOS
#define START_WIDTH   60
#define START_HEIGHT  200

#define SEARCH_XPOS    START_XPOS+(START_WIDTH*2)
#define SEARCH_YPOS    START_YPOS
#define SEARCH_WIDTH   140
#define SEARCH_HEIGHT  200


int main(int argc, char const* argv[])
{
  
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Ant Colony Optimization");

  World Map(MAP_XPOS, MAP_YPOS, MAP_WIDTH, MAP_HEIGHT);
  //Map.setAreaType(0, 0, 200, 60, _TileCell::START);
  //Map.setAreaType(0, 140, 200, 140, _TileCell::SEARCH_UNDISCOVERED);

  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<int> RandomXPos(START_XPOS, START_WIDTH);
  std::uniform_int_distribution<int> RandomYPos(START_YPOS, START_HEIGHT);

  Agent Player((RandomXPos(generator)/5)*5, (RandomYPos(generator)/5)*5, 5);

  Camera2D cam = { 0 };
  cam.zoom = 0.2f;

  SetTargetFPS(60);

  // Main loop
  while (!WindowShouldClose())
  {
    // Update Player
    int NewPosX = 0, NexPosY = 0;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        Player.Move(0, -1);
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        Player.Move(-1, 0);
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        Player.Move(0, 1);
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        Player.Move(1, 0);
    }


    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(cam);

        // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
        // just so we have something in the XY plane
        rlPushMatrix();
            rlTranslatef(0, 125 * 20, 0);
            rlRotatef(90, 1, 0, 0);
            DrawGrid(500, 20);
        rlPopMatrix();

        EndMode2D();

        // Draw TestArea
        DrawRectangleLines(MAP_XPOS, MAP_YPOS, MAP_WIDTH*2, MAP_HEIGHT*2, BLACK);
        DrawRectangle(START_XPOS, START_YPOS, START_WIDTH * 2, START_HEIGHT * 2, Fade(GREEN, 0.2f));
        DrawRectangle(SEARCH_XPOS, SEARCH_YPOS, SEARCH_WIDTH * 2, SEARCH_HEIGHT * 2, Fade(BLUE, 0.2f));

        // Draw Player
        DrawRectangleV(
            { Player.PosX(), Player.PosY() },
            { Player.SizeX(), Player.SizeY() }, 
            RED);

        // Draw Player Co-ords
        DrawText(TextFormat("[ %02.0f , %02.0f ]", Player.PosX(), Player.PosY()),
            Player.PosX() - Player.SizeX(),
            Player.PosY() + Player.SizeX(),
            10,
            BLACK);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}