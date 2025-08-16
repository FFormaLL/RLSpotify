# RL Spotify D-pad Plugin (BakkesMod)

Control Spotify (or any media player that honors system media keys) using your controller D-pad while playing Rocket League.

- Play/Pause
- Next track
- Previous track

Note: This plugin uses Windows media key events. It is intended primarily for Windows (Rocket League + BakkesMod on Windows).

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

## Two ways to control with D-pad

- Binds (simple; works without code hooks): bind your D-pad buttons to commands.
- Direct input listening (advanced; no binds required): the plugin hooks the game's `ControllerInput` and reacts to D-pad presses.

### Option A: Bind your D-pad

Use a helper or bind manually.

- All controllers (apply all three presets):
  - `spotify_setup_binds_all`
- Xbox only:
  - `spotify_setup_binds_xbox` or `spotify_setup_binds` (alias)
- PlayStation only:
  - `spotify_setup_binds_ps`
- Nintendo/Switch only:
  - `spotify_setup_binds_nintendo`

Manual examples:
```
# Xbox
bind XboxTypeS_DPad_Up spotify_play_pause
bind XboxTypeS_DPad_Right spotify_next
bind XboxTypeS_DPad_Left spotify_prev

# PlayStation (variants; depends on drivers/Steam Input)
bind PS4_DPad_Up spotify_play_pause
bind PS4_DPad_Right spotify_next
bind PS4_DPad_Left spotify_prev
bind PS5_DPad_Up spotify_play_pause
bind PS5_DPad_Right spotify_next
bind PS5_DPad_Left spotify_prev
bind DualShock4_DPad_Up spotify_play_pause
bind DualShock4_DPad_Right spotify_next
bind DualShock4_DPad_Left spotify_prev
bind DualSense_DPad_Up spotify_play_pause
bind DualSense_DPad_Right spotify_next
bind DualSense_DPad_Left spotify_prev

# Nintendo/Switch (variants; depends on drivers/Steam Input)
bind SwitchPro_DPad_Up spotify_play_pause
bind SwitchPro_DPad_Right spotify_next
bind SwitchPro_DPad_Left spotify_prev
bind NintendoSwitchPro_DPad_Up spotify_play_pause
bind NintendoSwitchPro_DPad_Right spotify_next
bind NintendoSwitchPro_DPad_Left spotify_prev
bind NintendoSwitch_DPad_Up spotify_play_pause
bind NintendoSwitch_DPad_Right spotify_next
bind NintendoSwitch_DPad_Left spotify_prev
```

### Option B: Direct D-pad listening

- Toggle: `spotify_listen_dpad` (default `1`)
  - When enabled, the plugin listens to `ControllerInput` via `Function TAGame.Car_TA.SetVehicleInput`.
  - It triggers media keys on rising edges:
    - Up: play/pause
    - Right: next
    - Left: previous
    - Down: (unused by default)

This mode avoids manual binds and should work regardless of controller name, as it reads input at the game level.

## Provided commands

- `spotify_play_pause`: Toggle play/pause
- `spotify_next`: Next track
- `spotify_prev`: Previous track
- `spotify_setup_binds`: Apply default Xbox D-pad binds (Up/Right/Left)
- `spotify_setup_binds_xbox`: Apply Xbox binds
- `spotify_setup_binds_ps`: Apply PlayStation binds (multiple variants)
- `spotify_setup_binds_nintendo`: Apply Nintendo/Switch binds (multiple variants)
- `spotify_setup_binds_all`: Apply all of the above
- `spotify_listen_dpad`: Cvar (1/0) to enable/disable direct D-pad listening

## How it works

- The plugin registers the commands above as notifiers.
- It can optionally hook `SetVehicleInput` and read `ControllerInput` for direct D-pad detection.
- Triggering a notifier or a detected D-pad press sends the corresponding Windows media key event (`VK_MEDIA_PLAY_PAUSE`, `VK_MEDIA_NEXT_TRACK`, `VK_MEDIA_PREV_TRACK`).
- Spotify (and most media players) respond to these events even when Rocket League is focused.

## Troubleshooting

- If binds do not trigger, verify your key name. Use BakkesMod's keybind helper or try binding to a keyboard key first to validate.
- If media keys do nothing, ensure Spotify is running and that system media keys work outside the game (e.g., from your keyboard).
- Steam Input remaps can override D-pad behavior. If needed, disable Steam Input for Rocket League or adjust your Steam controller layout.
- Some controller names depend on your drivers and whether Steam Input is translating to XInput. If the helper presets do not work, press buttons while viewing BakkesMod's keybind UI to see the exact names and bind those manually.
- If direct listening seems inconsistent, try turning `spotify_listen_dpad` off and use binds instead.

## Platform note

- Windows only (for now). If you want Linux support, we can add an MPRIS/`playerctl` backend or a small companion process.