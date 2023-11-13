#pragma once

// STL
#include <random>
#include <math.h>
// GUI

// User-Defined
#include "gui.hpp"

int main(int argc, char const* argv[])
{

  // Cam Init
  GUI::init_GUI(); 

  // Main loop
  GUI::startGui();

  return 0;
}