#pragma once

// STL
#include <random>
#include <math.h>
// GUI

// User-Defined
#include "gui.hpp"
#include "./common/globaldata.hpp"
#include "./common/common.hpp"
#include "./menu/menu.hpp"
#include "./menu/swarmmenu.hpp"
#include "./menu/worldmenu.hpp"
#include "./simulation/world.hpp"
#include "./simulation/swarm.hpp"
#include "./simulation/rrt.hpp"

int main(int argc, char const* argv[])
{
    GUI::init_GUI();

    GlobalData& globalData = GlobalData::getInstance();
    Menu SimMenu;

    // Add class render functions pointer to ImGUI render list
    ImGuiRenderWindows.push_back([&SimMenu]() {
        SimMenu.render();
        });

    // Init World
    World& SimWorld = World::getInstance();
    SimWorld.setWorldDims(0, 0, 20, 20);
    WorldMenu SimWorldObj(SimWorld);
    SimMenu.addModules(SimWorldObj);

    // Add class render functions pointer to GUI render list
    RLRenderWindows.push_back([&SimWorld]() {
        SimWorld.render();
        });

    HandleUserInputs.push_back([&SimWorld]() {
        SimWorld.handleInputs();
        });

    // Init Swarm
    Swarm& DroneSwarm = Swarm::getInstance();
    SwarmMenu SwarmMenuObj(DroneSwarm);
    SimMenu.addModules(SwarmMenuObj);

    // Add class render functions pointer to GUI render list
    RLRenderWindows.push_back([&DroneSwarm]() {
        DroneSwarm.render();
        });

    // Main loop
    while (!WindowShouldClose())
    {
        // Update global stuff
        globalData.updateMouseRel(calcMouseRel(GetMousePosition(), gridCamera.zoom, gridCamera.target));
      
        // Inputs
        GUI::HandleInputs(); 

        // TEST
        DroneSwarm.update();
        
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            //testrrt.update();
            //testrrt.growTree(SimWorld.getRandomFogPoint());
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            SimWorld.reset();
            DroneSwarm.reset();
        }

        // Draw
        GUI::renderRL();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}