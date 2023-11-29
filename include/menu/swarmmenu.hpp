#pragma once

#include "imgui.h"

#include "../simulation/swarm.hpp"

#include "ImGuiHelper.hpp"
#include "module.hpp"

// MODEL-VIEW-CONTROLLER
class SwarmMenu : public MenuModule {
public:
	SwarmMenu() = delete;
	SwarmMenu(Swarm& _swarm);
	~SwarmMenu() = default;

	void renderImGui();
	void onEnable() override;
	void onDisable() override;

private:
	Swarm& swarm;
	int selectedSubTab;
};

SwarmMenu::SwarmMenu(Swarm& _swarm) :
	MenuModule("Swarm", "Configure Swarm"), swarm(_swarm)
{
	this->selectedSubTab = 0;
	return;
}

void SwarmMenu::renderImGui() {
	std::vector<std::string> tabs = { "Actions","Visuals" };
	ImGuiHelper::drawTabHorizontally("swarm-actions-tab", ImVec2(ImGui::GetContentRegionAvail().x, 50), tabs, selectedSubTab);

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("swarm-actions", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
	ImGui::PopStyleColor();

	switch (selectedSubTab) {
	case 0: {
		ImGui::BeginChild("Population", ImVec2(ImGui::GetContentRegionAvail().x, 120), true);

		ImGui::PushItemWidth(this->elementsize());
		ImGui::SliderInt("Population", &(swarm.size()), 0, 500);
		// ImGui::SliderFloat("Alignment", &(swarm.weights().alignment), 0.0, 2.0);
		// ImGui::SliderFloat("Cohesion", &(swarm.weights().cohesion), 0.0, 2.0);
		// ImGui::SliderFloat("Seperation", &(swarm.weights().seperation), 0.0, 5.0);
		ImGui::EndChild();

		ImGui::Spacing();

		ImGui::BeginChild("Behaviours", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);

		ImGui::PushItemWidth(this->elementsize());
		//ImGui::Checkbox("Follow Mouse", &isFollowMouse);
		ImGui::EndChild();
		break;
	}

	case 1: {
		break;
	}
	default: {

		break;
	}
	}


	ImGui::EndChild();
	return;
}

void SwarmMenu::onEnable() {
	renderImGui();
}

void SwarmMenu::onDisable() {
	return;
}