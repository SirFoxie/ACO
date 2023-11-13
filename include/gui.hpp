#pragma once

// GUI
#include "raylib.h"     //raylib
#include "rlgl.h"       //raylib
#include "raymath.h"    //raylib
#include "imgui.h"
#include "rlImGui.h"

// User-defined
#include "radialMenu.hpp"
#include "agent.hpp"
#include "world.hpp"
#include "simulation.hpp"

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

#define CELL_SIZE     50

const int screenWidth = 1280;
const int screenHeight = 720;
const Vector2 orgin = { 0.0, 0.0 };

Camera2D gridCamera = { 0 };
bool isPanning = false;
Vector2 panStart = { 0.0, 0.0 };

// Radial Menu Init
bool ShowRadialMenu = false;

World Map(MAP_XPOS, MAP_YPOS, MAP_WIDTH, MAP_HEIGHT);
Simulation SwarmSim(Map, 1);
radialMenu radialMenuObj = { "Orange", "Blue", "Purple", "Gray", "Yellow", "Las Vegas" };

namespace GUI {
    void init_GUI()
    {
        InitWindow(screenWidth, screenHeight, "Ant Colony Optimization");
        SetTargetFPS(60);

        gridCamera.zoom = 1.0f;
        gridCamera.target = { -(screenWidth/(gridCamera.zoom*10)) , -(screenHeight/(gridCamera.zoom * 10)) };

        rlImGuiSetup(true);
    }

    void resetCamera(Camera2D& camera)
    {
        camera = Camera2D{ 0 };
        camera.zoom = 0.5f;
    }

    void zoomCamera(float mouseWheelDelta)
    {
        
    }

    void drawDrones()
    {
        static Texture2D droneTextureBlue = LoadTexture("../../../assets/quadcopterBlue.png");

        for(auto const& drone : SwarmSim.getAgentList())
            DrawTexture(
                droneTextureBlue,
                drone.PosX(),
                drone.PosY(),
                WHITE
            );
    }

    void drawGird()
    {
        // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
            // just so we have something in the XY plane
        rlPushMatrix();
            rlTranslatef(0, 125 * CELL_SIZE, 0);
            rlRotatef(90, 1, 0, 0);
            DrawGrid(500, CELL_SIZE);
        rlPopMatrix();
    }

    void drawMaze()
    {
        for (int i = 0; i < screenWidth / CELL_SIZE; i++)
        {
            for (int j = 0; j < screenHeight / CELL_SIZE; j++)
            {
                DrawText(TextFormat("[%i,%i]", i, j), 10 + CELL_SIZE * i, 15 + CELL_SIZE * j, 12, LIGHTGRAY);
            }
        }
    }

    void startGui()
    {
        while (!WindowShouldClose())
        {
            // Update Player
            int NewPosX = 0, NexPosY = 0;
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                //Player.Move(0, -1);
            }

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                //Player.Move(-1, 0);
            }

            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                //Player.Move(0, 1);
            }

            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                //Player.Move(1, 0);
            }

            float mouseWheelDelta = GetMouseWheelMove();
            Vector2 const mousePos = GetMousePosition();
            int const mousePosX = GetMouseX();
            int const mousePosY = GetMouseY();

            // Camera Stuff
            //if (IsKeyDown(KEY_LEFT_CONTROL))
            if(1)
            {
                gridCamera.zoom += (mouseWheelDelta * 0.05f);

                if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
                    panStart = mousePos; // Start panning
                    isPanning = true;
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) { isPanning = false; }

            if (isPanning) {
                Vector2 panDelta = Vector2Subtract(mousePos, panStart);

                gridCamera.target.x -= panDelta.x / gridCamera.zoom;
                gridCamera.target.y -= panDelta.y / gridCamera.zoom;

                panStart = mousePos;
            }

            // Display Radial Menu
            if (IsKeyDown(KEY_Q)) {
                radialMenuObj.setActive();
                radialMenuObj.setCenter(mousePosX, mousePosY);
            }

            // Draw
            BeginDrawing();
                ClearBackground(RAYWHITE);

                BeginMode2D(gridCamera);
                GUI::drawGird();
            
                // Draw TestArea
                //DrawRectangleLines(MAP_XPOS, MAP_YPOS, MAP_WIDTH * 2, MAP_HEIGHT * 2, BLACK);
                //DrawRectangle(START_XPOS, START_YPOS, START_WIDTH * 2, START_HEIGHT * 2, Fade(GREEN, 0.2f));
                //DrawRectangle(SEARCH_XPOS, SEARCH_YPOS, SEARCH_WIDTH * 2, SEARCH_HEIGHT * 2, Fade(BLUE, 0.2f));

                // Draw Drones
                GUI::drawDrones();
                GUI::drawMaze();

                DrawCircle(orgin.x, orgin.y, 5, RED);

            EndMode2D();
            
            Vector2 mousePosGrid = {
                ((((mousePos.x / gridCamera.zoom) + gridCamera.target.x) / CELL_SIZE)),
                ((((mousePos.y / gridCamera.zoom) + gridCamera.target.y) / CELL_SIZE)),
            };
            // Debug Items
            DrawText(TextFormat("TILE TYPE: %i", 1), 10, 10, 20, LIME);
            DrawText(TextFormat("CURRENT POS: [%i, %i]", 10,20), 10, 40, 20, GREEN);
            DrawText(TextFormat("CAMERA TARGET: [%.2f, %.2f]", gridCamera.target.x, gridCamera.target.y), 10, 60, 20, GREEN);
            DrawText(TextFormat("CAMERA ZOOM: %.2f", gridCamera.zoom), 10, 80, 20, GREEN);

            DrawText(TextFormat("GRID POS: [%.2f, %.2f]", mousePosGrid.x, mousePosGrid.y), 10, 210, 20, LIME);
            // UI Items
            rlImGuiBegin();
                // Draw Radial Menu
                if (radialMenuObj.checkActive()) { radialMenuObj.DrawRadialMenu(); }
            rlImGuiEnd();            

            EndDrawing();
        }
        rlImGuiShutdown();
        CloseWindow();
    }
}