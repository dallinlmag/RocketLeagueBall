#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"

constexpr auto plugin_version = "1.0";

class RLGoal: public BakkesMod::Plugin::BakkesModPlugin/*, public BakkesMod::Plugin::PluginWindow*/
{

public:
	virtual void onLoad();
	virtual void onUnload();
	void startWatching();
	void getPlayerIDs();
	void printGameData(ArrayWrapper<TeamWrapper> teams);
	void printPlayerData();
	void goalScored();
	void gameEnded();
	ServerWrapper GetCurrentGameState();

private:
	bool running = true;
	std::string jsonMessage;






	// Inherited via PluginWindow
	/*

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "RLGoal";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
	
	*/
};

