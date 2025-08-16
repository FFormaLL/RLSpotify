# RL Spotify D-pad Plugin (BakkesMod)

Control Spotify (or any media player that honors system media keys) using your controller D-pad while playing Rocket League.

- Play/Pause (D-pad Up)
- Next track (D-pad Right)
- Previous track (D-pad Left)

Windows-focused: Uses Windows media key events and an XInput listener by default.

## Build

- Open the solution/project in Visual Studio 2022 (v143 toolset) using this template.
- Ensure BakkesMod SDK paths are set via `BakkesMod.props`.
- Build Release|x64 to produce a DLL in `plugins/`.

## Install

- Copy the built DLL from `plugins/` to `%appdata%/bakkesmod/bakkesmod/plugins/`.
- Load in the BakkesMod console:
  - `plugin load <YourPluginName>`

## Listening modes (Windows)

- XInput listener (default ON): `spotify_listen_xinput 1|0`
- In-game ControllerInput hook (default OFF): `spotify_listen_dpad 1|0`

Both use rising-edge detection to avoid repeats.

## Quick setup (optional binds)

- Xbox: `spotify_setup_binds_xbox` (alias: `spotify_setup_binds`)
- PlayStation: `spotify_setup_binds_ps`
- Nintendo/Switch: `spotify_setup_binds_nintendo`
- All: `spotify_setup_binds_all`

## Troubleshooting

- If XInput doesn’t respond, ensure Windows sees your controller and Steam Input isn’t blocking XInput.
- Disable one listener if you get double triggers (`spotify_listen_xinput 0` or `spotify_listen_dpad 0`).
- Verify Spotify reacts to your keyboard media keys outside the game.

## Notes

- Windows only. Linux would need an MPRIS/`playerctl` backend.
