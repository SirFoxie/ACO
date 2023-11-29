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
#include "./simulation/swarm.hpp"
#include "./simulation/world.hpp"

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
    Swarm DroneSwarm(SWARM_SIZE, SimWorld);
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

        // Update
        DroneSwarm.update();

        // Draw
        GUI::renderRL();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}