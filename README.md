# RL Spotify D-pad Plugin (BakkesMod)

Control Spotify (or any media player that honors system media keys) using your controller D-pad while playing Rocket League.

- Play/Pause
- Next track
- Previous track

This plugin is primarily for Windows. It can listen to controller D-pad via Windows XInput (recommended) or via the in-game `ControllerInput` hook.

## Build

1. Install Visual Studio 2019/2022 (MSVC, Desktop development with C++).
2. Install the BakkesMod Plugin SDK template:
   - Recommended: Use the official template (`bakkesmod/bakkesmod-plugin-template`).
3. Add these two files to your plugin's source folder:
   - `SpotifyControllerPlugin.h`
   - `SpotifyControllerPlugin.cpp`
4. Ensure the include path contains the BakkesMod SDK headers and you link against the BakkesMod libraries as per the template.
5. Build the project to produce a DLL.

## Install

1. Copy the built DLL into your BakkesMod `plugins` folder, e.g.:
   - `%appdata%/bakkesmod/bakkesmod/plugins/SpotifyController.dll`
2. Add the plugin to your BakkesMod config or load it manually in the BakkesMod console:
   - `plugin load SpotifyController` (replace with your actual DLL name if different)

## Listening modes

- Windows XInput listener (default ON):
  - Cvar: `spotify_listen_xinput 1|0`
  - Reads D-pad directly from OS (works regardless of controller naming inside RL/Steam Input)
- In-game ControllerInput hook (default OFF):
  - Cvar: `spotify_listen_dpad 1|0`
  - Hooks `Function TAGame.Car_TA.SetVehicleInput` and reads `ControllerInput` D-pad

Both modes use rising-edge detection to avoid repeats while held.

## D-pad actions

- Up: Play/Pause
- Right: Next track
- Left: Previous track
- Down: (unused by default)

## Optional: Bind commands manually

You can also bind to explicit commands if you prefer not to use listeners:

- All controllers (apply all three presets): `spotify_setup_binds_all`
- Xbox: `spotify_setup_binds_xbox` or `spotify_setup_binds`
- PlayStation: `spotify_setup_binds_ps`
- Nintendo/Switch: `spotify_setup_binds_nintendo`

Manual examples are included for each controller family earlier.

## Provided commands and cvars

- `spotify_play_pause`, `spotify_next`, `spotify_prev`
- `spotify_setup_binds`, `spotify_setup_binds_xbox`, `spotify_setup_binds_ps`, `spotify_setup_binds_nintendo`, `spotify_setup_binds_all`
- `spotify_listen_xinput` (Windows XInput listener)
- `spotify_listen_dpad` (in-game ControllerInput listener)

## How it works

- Sends Windows media key events (`VK_MEDIA_PLAY_PAUSE`, `VK_MEDIA_NEXT_TRACK`, `VK_MEDIA_PREV_TRACK`) so Spotify responds while RL is focused.
- On Windows, XInput polling runs on a small background thread (polls 4 pads, ~8 ms cadence) and triggers on rising edges.

## Troubleshooting

- If XInput mode doesn't respond, ensure your controller is detected by Windows (Game Controllers) and not restricted by Steam Input.
- If you get double triggers, disable one mode: set `spotify_listen_xinput 0` or `spotify_listen_dpad 0`.
- If media keys do nothing, confirm Spotify responds to your keyboard's media keys outside the game.
- If using binds, controller button names may vary depending on drivers/Steam Input. Use the keybind UI or console key logging to find exact names.

## Platform note

- Windows only for media control and XInput listener. Linux support would require an alternative backend (e.g., MPRIS/`playerctl`).