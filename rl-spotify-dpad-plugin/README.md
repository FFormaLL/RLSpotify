# RL Spotify D-pad Plugin (BakkesMod)

Control Spotify (or any media player that honors system media keys) using your controller D-pad while playing Rocket League.

- Play/Pause
- Next track
- Previous track

Note: This plugin uses Windows media key events. It requires running Rocket League + BakkesMod on Windows.

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

## Bind your D-pad

Use the one-shot helper, or bind manually.

- Helper:
  - `spotify_setup_binds`
- Manual:
```
bind XboxTypeS_DPad_Up spotify_play_pause
bind XboxTypeS_DPad_Right spotify_next
bind XboxTypeS_DPad_Left spotify_prev
```

You can bind to any key or button; the above are just examples for Xbox-style controllers.

## Provided commands

- `spotify_play_pause`: Toggle play/pause
- `spotify_next`: Next track
- `spotify_prev`: Previous track
- `spotify_setup_binds`: Apply default Xbox D-pad binds (Up/Right/Left)

## How it works

- The plugin registers the commands above as notifiers.
- Triggering a notifier sends the corresponding Windows media key event (`VK_MEDIA_PLAY_PAUSE`, `VK_MEDIA_NEXT_TRACK`, `VK_MEDIA_PREV_TRACK`).
- Spotify (and most media players) respond to these events even when Rocket League is focused.

## Troubleshooting

- If binds do not trigger, verify your key name. Use BakkesMod's keybind helper or try binding to a keyboard key first to validate.
- If media keys do nothing, ensure Spotify is running and that system media keys work outside the game (e.g., from your keyboard).
- Steam Input remaps can override D-pad behavior. If needed, disable Steam Input for Rocket League or adjust your Steam controller layout.

## Platform note

- Windows only. If you need Linux support, you would need to adapt the code to call an external controller (e.g., `playerctl` via DBus/MPRIS) or run a small companion process. If you want that, let me know.