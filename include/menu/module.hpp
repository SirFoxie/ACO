#pragma once

#include <string>

class MenuModule {
public:
	MenuModule() = delete;
	MenuModule(std::string _name, std::string _description, int _key = 0);
	~MenuModule() = default;

	// Virtual functions
	virtual void onEnable() = 0;
	virtual void onDisable() = 0;

	// Accessors
	virtual std::string getName() const { return name; }
	virtual std::string getDescription() const { return description; }
	int getKey() { return key; }
	bool isToggled() { return toggled; }
	int elementsize() { return elementSize; }
	// Setters
	void setToggled(bool toggled, bool log = false);
	void toggle(bool log = false);
private:
	std::string name = "Module", description;
	bool toggled;
	int key;

	// Defaults
	int elementSize = 120;
};

MenuModule::MenuModule(std::string _name, std::string _description, int _key) :
	name(_name), description(_description), key(_key), toggled(false) {}

void MenuModule::setToggled(bool toggled, bool log) {
	this->toggled = toggled;
	if (!log) return;
	return;
}

void MenuModule::toggle(bool log) {
	this->toggled = !(this->toggled);
	
	if (isToggled()) onEnable();
	else onDisable();
	
	if (!log) return;
	return;
}
