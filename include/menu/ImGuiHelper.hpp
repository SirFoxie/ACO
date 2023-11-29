#pragma once

#include "Menu.hpp"
namespace ImGuiHelper {
	void drawTabHorizontally(std::string childName, ImVec2 childSize, std::vector<std::string>tabNames, int& selectedSubTab);
	float getWidth();
	float getHeight();
	ImVec2 getTextLength(std::string text);
}

float ImGuiHelper::getWidth() {
	return ImGui::GetContentRegionAvail().x;
}
float ImGuiHelper::getHeight() {
	return ImGui::GetContentRegionAvail().y;
}

ImVec2 ImGuiHelper::getTextLength(std::string text) {
	return ImGui::CalcTextSize(text.c_str());
}

void ImGuiHelper::drawTabHorizontally(std::string childName, ImVec2 childSize, std::vector<std::string>tabNames, int& selectedSubTab) {
	int length = tabNames.front().length(); // get shortest string length
	int strIndex = 1;
	for (int i = 1; i < tabNames.size(); i++) {
		if (length > tabNames.at(i).length()) {
			length = tabNames.at(i).length();
			strIndex = i;
		}
	}

	ImGui::BeginChild(childName.c_str(), childSize, true, ImGuiWindowFlags_HorizontalScrollbar);

	int minWidth = ImGuiHelper::getTextLength(tabNames.at(strIndex).c_str()).x;
	int maxWidth = 200;

	int btnWidth = (ImGuiHelper::getWidth() - ImGui::GetStyle().ItemSpacing.x * (tabNames.size())) / tabNames.size();
	int btnHeight =ImGuiHelper::getHeight();
	btnWidth = (std::max)(minWidth, (std::min)(btnWidth, maxWidth));

	{ // center buttons
		// tell Dear ImGui to render the button at the new pos
		ImGui::SetCursorPosX((childSize.x - btnWidth * tabNames.size() - ImGui::GetStyle().ItemSpacing.x) / 2);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5); // round buttons
	for (int i = 0; i < tabNames.size(); i++) {
		std::string it = tabNames.at(i);
		//ImGui::PushStyleColor(ImGuiCol_Button, selectedSubTab == i ? ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] : ImGui::GetStyle().Colors[ImGuiCol_Button]);
		//ImGui::PushStyleColor(ImGuiCol_Text, selectedSubTab == i ? ImGui::GetStyle().Colors[ImGuiCol_Text] : *Menu::notSelectedTextColor);


		if (ImGui::Button(it.c_str(), ImVec2(btnWidth, btnHeight))) selectedSubTab = i;
		ImGui::SameLine();
		//ImGui::PopStyleColor(2);

	}
	ImGui::PopStyleVar();

	ImGui::EndChild();
}