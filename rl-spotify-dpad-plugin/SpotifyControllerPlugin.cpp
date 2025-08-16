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

	cvarManager->registerNotifier(
		"spotify_setup_binds_xbox",
		[this](std::vector<std::string> /*args*/) { SetupBindsXbox(); },
		"Bind Xbox D-pad Up/Right/Left to play/pause, next, previous",
		PERMISSION_ALL);

	cvarManager->registerNotifier(
		"spotify_setup_binds_ps",
		[this](std::vector<std::string> /*args*/) { SetupBindsPlayStation(); },
		"Bind PlayStation D-pad Up/Right/Left to play/pause, next, previous",
		PERMISSION_ALL);

	cvarManager->registerNotifier(
		"spotify_setup_binds_nintendo",
		[this](std::vector<std::string> /*args*/) { SetupBindsNintendo(); },
		"Bind Nintendo/Switch D-pad Up/Right/Left to play/pause, next, previous",
		PERMISSION_ALL);

	cvarManager->registerNotifier(
		"spotify_setup_binds_all",
		[this](std::vector<std::string> /*args*/) { SetupBindsAll(); },
		"Apply binds for Xbox, PlayStation, and Nintendo/Switch D-pads",
		PERMISSION_ALL);

	// Toggle to enable/disable listening to ControllerInput D-pad
	auto listenCvar = cvarManager->registerCvar("spotify_listen_dpad", "1", "Listen to controller D-pad via ControllerInput hook (1=on,0=off)");
	listenCvar.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		SetListenDpadEnabled(cvar.getBoolValue());
	});

	// Apply initial state
	SetListenDpadEnabled(listenCvar.getBoolValue());

	cvarManager->log("SpotifyController loaded. Example binds: bind XboxTypeS_DPad_Up spotify_play_pause");
}

void SpotifyControllerPlugin::onUnload()
{
	if (gameWrapper)
	{
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
	}
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
	// Issue bind commands through the console (Xbox defaults)
	cvarManager->executeCommand("bind XboxTypeS_DPad_Up spotify_play_pause");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Right spotify_next");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Left spotify_prev");
	cvarManager->log("SpotifyController: Default binds applied for Xbox D-pad (Up/Right/Left).");
}

void SpotifyControllerPlugin::SetupBindsXbox()
{
	if (!gameWrapper) return;
	cvarManager->executeCommand("bind XboxTypeS_DPad_Up spotify_play_pause");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Right spotify_next");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Left spotify_prev");
	cvarManager->log("SpotifyController: Xbox binds applied.");
}

void SpotifyControllerPlugin::SetupBindsPlayStation()
{
	if (!gameWrapper) return;
	// Common PlayStation mappings (PS4/PS5/DualShock4/DualSense). These may vary based on drivers/Steam Input.
	const std::vector<std::string> psPrefixes = {
		"PS4",
		"PS5",
		"DualShock4",
		"DualSense"
	};
	for (const auto& prefix : psPrefixes)
	{
		cvarManager->executeCommand("bind " + prefix + "_DPad_Up spotify_play_pause");
		cvarManager->executeCommand("bind " + prefix + "_DPad_Right spotify_next");
		cvarManager->executeCommand("bind " + prefix + "_DPad_Left spotify_prev");
	}
	cvarManager->log("SpotifyController: PlayStation binds applied (PS4/PS5/DualShock4/DualSense).");
}

void SpotifyControllerPlugin::SetupBindsNintendo()
{
	if (!gameWrapper) return;
	// Common Nintendo/Switch mappings. Names depend on drivers/Steam Input; we bind several likely variants.
	const std::vector<std::string> nsPrefixes = {
		"SwitchPro",
		"NintendoSwitchPro",
		"NintendoSwitch"
	};
	for (const auto& prefix : nsPrefixes)
	{
		cvarManager->executeCommand("bind " + prefix + "_DPad_Up spotify_play_pause");
		cvarManager->executeCommand("bind " + prefix + "_DPad_Right spotify_next");
		cvarManager->executeCommand("bind " + prefix + "_DPad_Left spotify_prev");
	}
	cvarManager->log("SpotifyController: Nintendo/Switch binds applied (multiple variants).");
}

void SpotifyControllerPlugin::SetupBindsAll()
{
	SetupBindsXbox();
	SetupBindsPlayStation();
	SetupBindsNintendo();
	cvarManager->log("SpotifyController: All controller binds applied (Xbox + PlayStation + Nintendo/Switch).");
}

void SpotifyControllerPlugin::SetListenDpadEnabled(bool enabled)
{
	if (!gameWrapper) return;
	gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
	if (!enabled) return;

	prevDpadUp = prevDpadRight = prevDpadLeft = prevDpadDown = false;
	gameWrapper->HookEventWithCaller<CarWrapper>(
		"Function TAGame.Car_TA.SetVehicleInput",
		[this](CarWrapper /*caller*/, void* params, std::string /*eventName*/) {
			if (params == nullptr) return;
			ControllerInput* input = reinterpret_cast<ControllerInput*>(params);
			if (!input) return;

			bool up = input->DPad_Up;
			bool right = input->DPad_Right;
			bool left = input->DPad_Left;
			bool down = input->DPad_Down;

			if (up && !prevDpadUp) { PlayPause(); }
			if (right && !prevDpadRight) { NextTrack(); }
			if (left && !prevDpadLeft) { PreviousTrack(); }
			// optionally map down to stop if desired
			// if (down && !prevDpadDown) { /* Send stop: VK_MEDIA_STOP */ }

			prevDpadUp = up;
			prevDpadRight = right;
			prevDpadLeft = left;
			prevDpadDown = down;
		});
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