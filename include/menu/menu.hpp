#pragma once

#include <string>
#include <vector>

#include "imgui.h"
#include "ImGuiHelper.hpp"

#include "module.hpp"
#include "menu/worldmenu.hpp"

class Menu {
public:
	Menu();
	~Menu() = default;

	void setColors();
	void loadTheme();

	void addModules(MenuModule& newModule);

	void render();
private:
	bool isPanelHidden = false;
	int menuW = 600;
	int menuH = 300;
	int selectedTab = 0;

	std::vector <MenuModule*> Modules;
	std::string ModuleNames;

	ImGuiStyle* style;

	void renderEdgeButton();
	void renderPanel();
};

Menu::Menu() {
	loadTheme();
}

void Menu::addModules(MenuModule& newModule) {
	Modules.push_back(&newModule);
	ModuleNames.append(newModule.getName());
}

void Menu::render() {
	ImGui::SetNextWindowPos(ImVec2(0, GetScreenWidth() / 2 - menuH));
	ImGui::SetNextWindowSize(ImVec2(menuW, menuH));

	static ImGuiWindowFlags windowFlags{
		ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoCollapse 
		| ImGuiWindowFlags_NoTitleBar 
		| ImGuiWindowFlags_NoBackground
	};

	if (ImGui::Begin("Menu", NULL, windowFlags)) {
		// Draw Submenus
		renderEdgeButton();

		if (!isPanelHidden && Modules.size() > 0) {
			ImGui::Columns(2);
			ImGui::SetColumnOffset(1, 170);
			renderPanel();

			ImGui::NextColumn();

			Modules[selectedTab]->onEnable();
		}
	}

	ImGui::End();
}


void Menu::renderPanel() {
	ImGui::BeginChild("panel", ImVec2(158, ImGui::GetContentRegionAvail().y), true);


	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
	for (int i = 0; i < Modules.size(); i++) {
		std::string it = Modules[i]->getName();
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
		if (ImGui::Button(it.c_str(), ImVec2(140, 40))) selectedTab = i;
		ImGui::PopStyleVar();
		//ImGui::PopStyleColor(2);
	}
	ImGui::PopStyleVar();

	ImGui::EndChild();
}


void Menu::setColors() {
	style->Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 230);
	style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(239, 73, 88, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(173, 55, 65, 255);
	style->Colors[ImGuiCol_FrameBg] = ImColor(31, 30, 31, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(44, 43, 44, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(37, 36, 37, 255);
	style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_ChildBg] = ImColor(38, 39, 50, 255);
	style->Colors[ImGuiCol_CheckMark] = ImColor(240, 50, 66, 255);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(240, 74, 88, 255);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(240, 50, 66, 255);
	style->Colors[ImGuiCol_Header] = ImColor(240, 50, 66, 255);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(240, 74, 88, 255);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(240, 50, 66, 255);
	style->Colors[ImGuiCol_ResizeGrip] = ImColor(220, 50, 66, 120);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(250, 50, 66, 140);
	style->Colors[ImGuiCol_ResizeGripActive] = ImColor(240, 50, 66, 255);
	style->Colors[ImGuiCol_SeparatorHovered] = ImColor(250, 50, 66, 140);
	style->Colors[ImGuiCol_SeparatorActive] = ImColor(240, 50, 66, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(240, 50, 66, 255);
}

void Menu::loadTheme() {

	style = &ImGui::GetStyle();

	// ROUNDINGS
	style->WindowRounding = 6;
	style->ChildRounding = 6;
	style->FrameRounding = 2;
	style->GrabRounding = 2;
	style->PopupRounding = 2;

	style->FramePadding = ImVec2(6, 3);
	style->ItemSpacing = ImVec2(4, 4);

	setColors();
}

void Menu::renderEdgeButton() {
	ImGui::BeginChild("edge-button", ImVec2(40, 35), true);
	if (isPanelHidden) {
		if (ImGui::ArrowButton("show-panel", ImGuiDir_Left)) this->isPanelHidden = false;
	}
	else {
		if (ImGui::ArrowButton("hide-panel", ImGuiDir_Right)) this->isPanelHidden = true;
	}
	ImGui::EndChild();
}
