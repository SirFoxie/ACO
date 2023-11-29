#pragma once

#include <vector>
#include <functional>

// GUI
#include "raylib.h"     //raylib
#include "rlgl.h"       //raylib
#include "raymath.h"    //raylib
#include "imgui.h"
#include "rlImGui.h"

// User-defined
#include "common/common.hpp"

const int screenWidth = 1280;
const int screenHeight = 720;
const Vector2 orgin = { 0.0, 0.0 };

Camera2D gridCamera = { 0 };
bool isPanning = false;
Vector2 panStart = { 0.0, 0.0 };

std::vector<std::function<void(void)>> HandleUserInputs;

// Pointer to render functions for RayLib
std::vector<std::function<void(void)>> RLRenderWindows;

// Pointer to render functions for ImGui
std::vector<std::function<void(void)>> ImGuiRenderWindows;

namespace GUI {
    struct render_window {
        int screenWidth;
        int screenHeight;
        int framerate;
        std::string windowName;
    };


    void init_GUI();
    void resetCamera(Camera2D& camera);
    void zoomCamera(float mouseWheelDelta);
    void drawGrid();
    void CameraInputs();

    void HandleInputs();
    void renderRL();
    void renderImGui();

    void init_GUI()
    {
        InitWindow(screenWidth, screenHeight, "Ant Colony Optimization");
        SetTargetFPS(60);

        gridCamera.zoom = 1.0f;
        gridCamera.target = { -(screenWidth/(gridCamera.zoom*10)) , -(screenHeight/(gridCamera.zoom * 10)) };
        HandleUserInputs.push_back(CameraInputs);

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

    void drawGrid()
    {
        // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
            // just so we have something in the XY plane
        rlPushMatrix();
            rlTranslatef(0, 125 * CELL_SIZE, 0);
            rlRotatef(90, 1, 0, 0);
            DrawGrid(500, CELL_SIZE);
        rlPopMatrix();
    }

    void CameraInputs()
    {
        float mouseWheelDelta = GetMouseWheelMove();
        Vector2 const mousePos = GetMousePosition();

        if (1)
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
    }

    void HandleInputs()
    {
        for (const auto& inputs : HandleUserInputs) {
            inputs();
        }
        return;
    }

    void renderRL()
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode2D(gridCamera);
        
        GUI::drawGrid();

        // Call the function using the function pointer
        for (const auto& render : RLRenderWindows) {
            render();
        }

        DrawCircle(orgin.x, orgin.y, 5, RED);


        EndMode2D();
        
        GUI::renderImGui();

        EndDrawing();
    }

    void renderImGui()
    {
        rlImGuiBegin();

        // Call the function using the function pointer
        for (const auto& render : ImGuiRenderWindows) {
            render();
        }

        rlImGuiEnd();
    }
}