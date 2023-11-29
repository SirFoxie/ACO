#pragma once

#include "imgui.h"

#include "ImGuiHelper.hpp"
#include "module.hpp"

#include "./simulation/world.hpp"

class WorldMenu : public MenuModule {
public:
	WorldMenu() = delete;
	WorldMenu(World& _world);
	~WorldMenu() = default;

	void update();

	void renderImGui();
	void onEnable() override;
	void onDisable() override;
private:
	World& world;
	int selectedSubTab;
};

void WorldMenu::onEnable() {
	renderImGui();
}

void WorldMenu::onDisable() {
	return;
}

WorldMenu::WorldMenu(World& _world) :
	MenuModule("Map", "Configure Map"), world(_world)
{
	this->selectedSubTab = 0;
	return;
}

void WorldMenu::renderImGui() {
	std::vector<std::string> tabs = { "Actions","Visuals" };
	ImGuiHelper::drawTabHorizontally("map-actions-tab", ImVec2(ImGui::GetContentRegionAvail().x, 50), tabs, selectedSubTab);

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("map-actions", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
	ImGui::PopStyleColor();

	switch (selectedSubTab) {
	case 0:

		ImGui::BeginChild("Area", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2), true);

		ImGui::PushItemWidth(this->elementsize());

		ImGui::SliderInt("Map Width", &(world.getMap().width), 1, 20);
		ImGui::SliderInt("Map Height", &(world.getMap().height),1, 20);
		
		ImGui::SliderFloat("Border", &(world.getStartSearchBorder()), 0, 1);


		ImGui::EndChild();

		ImGui::Spacing();

		ImGui::BeginChild("Walls", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
		//ImGui::Text("%s", randomPoint.first, randomPoint.second);
		ImGui::Checkbox("Place Markers", &(world.checkPlacingMarkers()));
		ImGui::Checkbox("Place Wall", &(world.checkPlacingWalls()));
		//ImGui::Checkbox("Follow Mouse", &isFollowMouse);
		ImGui::EndChild();

		break;
	case 1:
		break;
	default:
		break;
	}

	ImGui::EndChild();
	return;
}