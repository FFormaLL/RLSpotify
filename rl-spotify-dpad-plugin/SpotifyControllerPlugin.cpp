#include "SpotifyControllerPlugin.h"
#include <vector>
#include <string>

BAKKESMOD_PLUGIN(SpotifyControllerPlugin, "Control Spotify with controller D-pad via media keys", "1.0.0", 0)

void SpotifyControllerPlugin::onLoad()
{
	cvarManager->registerNotifier(
		"spotify_play_pause",
		[this](std::vector<std::string> /*args*/) { PlayPause(); },
		"Toggle play/pause via media key",
		PERMISSION_ALL);

	cvarManager->registerNotifier(
		"spotify_next",
		[this](std::vector<std::string> /*args*/) { NextTrack(); },
		"Next track via media key",
		PERMISSION_ALL);

	cvarManager->registerNotifier(
		"spotify_prev",
		[this](std::vector<std::string> /*args*/) { PreviousTrack(); },
		"Previous track via media key",
		PERMISSION_ALL);

	cvarManager->registerNotifier(
		"spotify_setup_binds",
		[this](std::vector<std::string> /*args*/) { SetupDefaultBinds(); },
		"Bind Xbox D-pad Up/Right/Left to play/pause, next, previous",
		PERMISSION_ALL);

	cvarManager->log("SpotifyController loaded. Example binds: bind XboxTypeS_DPad_Up spotify_play_pause");
}

void SpotifyControllerPlugin::onUnload()
{
	// Nothing to clean up
}

void SpotifyControllerPlugin::PlayPause()
{
#ifdef _WIN32
	SendMediaKey(VK_MEDIA_PLAY_PAUSE);
#else
	cvarManager->log("spotify_play_pause: Unsupported platform. This plugin requires Windows.");
#endif
}

void SpotifyControllerPlugin::NextTrack()
{
#ifdef _WIN32
	SendMediaKey(VK_MEDIA_NEXT_TRACK);
#else
	cvarManager->log("spotify_next: Unsupported platform. This plugin requires Windows.");
#endif
}

void SpotifyControllerPlugin::PreviousTrack()
{
#ifdef _WIN32
	SendMediaKey(VK_MEDIA_PREV_TRACK);
#else
	cvarManager->log("spotify_prev: Unsupported platform. This plugin requires Windows.");
#endif
}

void SpotifyControllerPlugin::SetupDefaultBinds()
{
	if (!gameWrapper) return;
	// Issue bind commands through the console
	cvarManager->executeCommand("bind XboxTypeS_DPad_Up spotify_play_pause");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Right spotify_next");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Left spotify_prev");
	cvarManager->log("SpotifyController: Default binds applied for Xbox D-pad (Up/Right/Left).");
}

#ifdef _WIN32
void SpotifyControllerPlugin::SendMediaKey(WORD vk)
{
	INPUT inputs[2] = {};
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = vk;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = vk;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	UINT sent = SendInput(2, inputs, sizeof(INPUT));
	if (sent != 2)
	{
		cvarManager->log("SpotifyController: SendInput failed.");
	}
}
#endif