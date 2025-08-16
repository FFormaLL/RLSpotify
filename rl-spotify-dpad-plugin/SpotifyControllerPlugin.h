#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "bakkesmod/plugin/bakkesmodplugin.h"

class SpotifyControllerPlugin final : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	void onLoad() override;
	void onUnload() override;

private:
	void PlayPause();
	void NextTrack();
	void PreviousTrack();
	void SetupDefaultBinds();
	void SetupBindsXbox();
	void SetupBindsPlayStation();
	void SetupBindsNintendo();
	void SetupBindsAll();

#ifdef _WIN32
	void SendMediaKey(WORD virtualKeyCode);
#endif
};