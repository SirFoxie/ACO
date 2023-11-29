#pragma once

#include "Menu.hpp"
namespace ImGuiHelper {
	void drawTabHorizontally(std::string childName, ImVec2 childSize, std::vector<std::string>tabNames, int& selectedSubTab);
	void renderCombo(std::string title, std::vector<std::string>items, int& index, int comboWidth);
	float getWidth();
	float getHeight();
	ImVec2 getTextLength(std::string text);
	int elementSize = 120;
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

void ImGuiHelper::renderCombo(std::string title, std::vector<std::string>items, int& index, int comboWidth) {
	ImGui::PushItemWidth(comboWidth);
	if (ImGui::BeginCombo(title.c_str(), items.at(index).c_str(), 0)) {

		for (int n = 0; n < items.size(); n++) {
			const bool is_selected = (index == n);
			if (ImGui::Selectable(items.at(n).c_str(), is_selected))index = n;

			if (is_selected)ImGui::SetItemDefaultFocus();// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
}