#include "pch.h"
#include "$projectname$.h"

#ifdef _WIN32
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")
#endif

BAKKESMOD_PLUGIN($projectname$, "Control Spotify with controller D-pad via media keys", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void $projectname$::onLoad()
{
	_globalCvarManager = cvarManager;

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

	// In-game ControllerInput hook toggle (default off; prefer Windows XInput listener)
	auto listenCvar = cvarManager->registerCvar("spotify_listen_dpad", "0", "Listen to in-game ControllerInput D-pad (1=on,0=off)");
	listenCvar.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		SetListenDpadEnabled(cvar.getBoolValue());
	});

#ifdef _WIN32
	// Windows XInput listener toggle (default on)
	auto xinputCvar = cvarManager->registerCvar("spotify_listen_xinput", "1", "Listen to Windows XInput D-pad (1=on,0=off)");
	xinputCvar.addOnValueChanged([this](std::string, CVarWrapper cvar) {
		bool enabled = cvar.getBoolValue();
		if (enabled) StartXInputListener(); else StopXInputListener();
	});
	if (xinputCvar.getBoolValue()) StartXInputListener();
#endif

	cvarManager->log("SpotifyController loaded. Use spotify_listen_xinput (Windows) or spotify_listen_dpad (in-game) to choose listening mode.");
}

void $projectname$::onUnload()
{
	if (gameWrapper)
	{
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
	}
#ifdef _WIN32
	StopXInputListener();
#endif
}

void $projectname$::PlayPause()
{
#ifdef _WIN32
	SendMediaKey(VK_MEDIA_PLAY_PAUSE);
#else
	cvarManager->log("spotify_play_pause: Unsupported platform. This plugin requires Windows.");
#endif
}

void $projectname$::NextTrack()
{
#ifdef _WIN32
	SendMediaKey(VK_MEDIA_NEXT_TRACK);
#else
	cvarManager->log("spotify_next: Unsupported platform. This plugin requires Windows.");
#endif
}

void $projectname$::PreviousTrack()
{
#ifdef _WIN32
	SendMediaKey(VK_MEDIA_PREV_TRACK);
#else
	cvarManager->log("spotify_prev: Unsupported platform. This plugin requires Windows.");
#endif
}

void $projectname$::SetupDefaultBinds()
{
	if (!gameWrapper) return;
	cvarManager->executeCommand("bind XboxTypeS_DPad_Up spotify_play_pause");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Right spotify_next");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Left spotify_prev");
	cvarManager->log("SpotifyController: Default binds applied for Xbox D-pad (Up/Right/Left).");
}

void $projectname$::SetupBindsXbox()
{
	if (!gameWrapper) return;
	cvarManager->executeCommand("bind XboxTypeS_DPad_Up spotify_play_pause");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Right spotify_next");
	cvarManager->executeCommand("bind XboxTypeS_DPad_Left spotify_prev");
	cvarManager->log("SpotifyController: Xbox binds applied.");
}

void $projectname$::SetupBindsPlayStation()
{
	if (!gameWrapper) return;
	const std::vector<std::string> psPrefixes = { "PS4", "PS5", "DualShock4", "DualSense" };
	for (const auto& prefix : psPrefixes)
	{
		cvarManager->executeCommand("bind " + prefix + "_DPad_Up spotify_play_pause");
		cvarManager->executeCommand("bind " + prefix + "_DPad_Right spotify_next");
		cvarManager->executeCommand("bind " + prefix + "_DPad_Left spotify_prev");
	}
	cvarManager->log("SpotifyController: PlayStation binds applied (PS4/PS5/DualShock4/DualSense).");
}

void $projectname$::SetupBindsNintendo()
{
	if (!gameWrapper) return;
	const std::vector<std::string> nsPrefixes = { "SwitchPro", "NintendoSwitchPro", "NintendoSwitch" };
	for (const auto& prefix : nsPrefixes)
	{
		cvarManager->executeCommand("bind " + prefix + "_DPad_Up spotify_play_pause");
		cvarManager->executeCommand("bind " + prefix + "_DPad_Right spotify_next");
		cvarManager->executeCommand("bind " + prefix + "_DPad_Left spotify_prev");
	}
	cvarManager->log("SpotifyController: Nintendo/Switch binds applied (multiple variants).");
}

void $projectname$::SetupBindsAll()
{
	SetupBindsXbox();
	SetupBindsPlayStation();
	SetupBindsNintendo();
	cvarManager->log("SpotifyController: All controller binds applied (Xbox + PlayStation + Nintendo/Switch).");
}

void $projectname$::SetListenDpadEnabled(bool enabled)
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
			// if (down && !prevDpadDown) { /* optional stop */ }

			prevDpadUp = up;
			prevDpadRight = right;
			prevDpadLeft = left;
			prevDpadDown = down;
		});
}

#ifdef _WIN32
void $projectname$::StartXInputListener()
{
	StopXInputListener();
	xinputStopFlag = false;
	xinputThread = std::thread([this]() {
		while (!xinputStopFlag)
		{
			for (DWORD i = 0; i < 4; ++i)
			{
				XINPUT_STATE state{};
				DWORD res = XInputGetState(i, &state);
				if (res == ERROR_SUCCESS)
				{
					WORD mask = state.Gamepad.wButtons;
					const WORD upMask = XINPUT_GAMEPAD_DPAD_UP;
					const WORD rightMask = XINPUT_GAMEPAD_DPAD_RIGHT;
					const WORD leftMask = XINPUT_GAMEPAD_DPAD_LEFT;
					const WORD downMask = XINPUT_GAMEPAD_DPAD_DOWN;

					bool up = (mask & upMask) != 0;
					bool right = (mask & rightMask) != 0;
					bool left = (mask & leftMask) != 0;
					bool down = (mask & downMask) != 0;

					WORD prev = prevXInputDpadMask[i];
					bool prevUp = (prev & upMask) != 0;
					bool prevRight = (prev & rightMask) != 0;
					bool prevLeft = (prev & leftMask) != 0;
					bool prevDown = (prev & downMask) != 0;

					if (up && !prevUp) { PlayPause(); }
					if (right && !prevRight) { NextTrack(); }
					if (left && !prevLeft) { PreviousTrack(); }
					// if (down && !prevDown) { /* optional stop */ }

					prevXInputDpadMask[i] = static_cast<unsigned short>(mask & (upMask | rightMask | leftMask | downMask));
				}
				else
				{
					prevXInputDpadMask[i] = 0;
				}
			}
			Sleep(8);
		}
	});
}

void $projectname$::StopXInputListener()
{
	if (xinputThread.joinable())
	{
		xinputStopFlag = true;
		xinputThread.join();
	}
	xinputStopFlag = false;
	for (int i = 0; i < 4; ++i) prevXInputDpadMask[i] = 0;
}
#endif

#ifdef _WIN32
void $projectname$::SendMediaKey(WORD vk)
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
