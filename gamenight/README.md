# GameNight couch integration

This fork retains Luanti's upstream history and LGPL-2.1-or-later license.
The `gamenight-couch` branch is based on upstream Luanti 5.17.0
(`c0e6812b1a4260bb25a1f606f70f55f4962bb97d`).

## Scope

- Two Windows clients in borderless side-by-side views.
- Independent controller binding, neutral-state arming, and hotplug handling.
- Host-routed controller frames with freshness checks and background camera input.
- Directional D-pad/stick focus for menus and inventory, with A/B actions.
- Shared pause support for the local world and hidden/muted client views.
- Window grouping that yields when the user switches to another application.

Mineclonia and the GameNight adapter are separate components. This repository
contains the engine changes, not a new Mineclonia game fork. GameNight
integration currently lives in
[the maintained adapter](https://github.com/ontola/gamenight-mineclonia).

## Build and test

Run `bash gamenight/build-windows.sh NEW_BUILD_DIRECTORY` on Linux or WSL.
It tests the input routing helpers, uses upstream's pinned Windows buildbot
dependencies and builds a Windows x64 ZIP. CI retains the ZIP and provenance.
The source checkout must be clean when recording a release revision.

## Readiness

The original prototype had two physical controllers confirmed independently.
A real-daemon start/pause/resume test passed, and the user confirmed the lobby
Back transition. This does not establish that every new package has passed
inventory, reconnect, audio, performance, or mod-installation playtests.
Distribution must include matching source and all upstream licenses.

Initial support is Windows x64 and two local players. Text entry still uses a
keyboard; one view supplies shared audio. This uses two rendering processes.
It does not add native single-renderer split-screen or arbitrary AI-generated mods.
