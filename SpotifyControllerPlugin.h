#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include <thread>
#include <atomic>

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

	// Input listening state (in-game ControllerInput)
	bool prevDpadUp = false;
	bool prevDpadRight = false;
	bool prevDpadLeft = false;
	bool prevDpadDown = false;
	void SetListenDpadEnabled(bool enabled);

#ifdef _WIN32
	// Windows XInput listener
	void StartXInputListener();
	void StopXInputListener();
	std::thread xinputThread;
	std::atomic<bool> xinputStopFlag{false};
	unsigned short prevXInputDpadMask[4] = {0, 0, 0, 0};
#endif
};