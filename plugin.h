#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

#include <thread>
#include <atomic>
#include <vector>
#include <string>

class $projectname$: public BakkesMod::Plugin::BakkesModPlugin
{
	//,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
public:
	void onLoad() override;
	void onUnload() override;

private:
	// Media actions
	void PlayPause();
	void NextTrack();
	void PreviousTrack();

	// Helper binds for common controllers
	void SetupDefaultBinds();
	void SetupBindsXbox();
	void SetupBindsPlayStation();
	void SetupBindsNintendo();
	void SetupBindsAll();

	// In-game input listening (ControllerInput hook)
	void SetListenDpadEnabled(bool enabled);
	bool prevDpadUp = false;
	bool prevDpadRight = false;
	bool prevDpadLeft = false;
	bool prevDpadDown = false;

#ifdef _WIN32
	// Windows XInput listener
	void StartXInputListener();
	void StopXInputListener();
	std::thread xinputThread;
	std::atomic<bool> xinputStopFlag{false};
	unsigned short prevXInputDpadMask[4] = {0, 0, 0, 0};

	// Windows media key sender
	void SendMediaKey(WORD virtualKeyCode);
#endif
};
