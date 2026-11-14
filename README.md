# InCarInventory

InCarInventory is a DayZ PC mod that lets you use your personal inventory and the inventory of the vehicle you occupy without leaving your seat.

## Features

- Open your inventory from a driver or passenger seat using the normal inventory key.
- View the occupied vehicle's cargo and attachment categories in the vicinity panel when the vehicle has them.
- Move items between your inventory and the vehicle when DayZ allows the transfer. Server-side transfer checks and script locks that hide vehicle inventory still apply.

## Requirements and compatibility

Requires DayZ on PC. The addon depends on DayZ's built-in `DZ_Data` and loads scripts in the World and Mission modules. It has no declared third-party mod dependencies.

Version 1.2 was tested on DayZ 1.30. The test confirmed item transfers while seated. Vehicles with custom inventory handling and mods that also change inventory visibility or vicinity refresh may behave differently.

## Installation

1. Subscribe to [InCarInventory on Steam Workshop](https://steamcommunity.com/sharedfiles/filedetails/?id=3693164249) if it is available to you.
2. For a dedicated server, place the packaged mod in its own folder, such as `@InCarInventory`. Copy the included public `.bikey` to the server's `keys` directory when signature verification is enabled.
3. Add `@InCarInventory` to the server's existing `-mod` list and restart the server. Clients must load the matching mod version before joining.

The GitHub repository contains source files. To build locally with DayZ Tools, pack `addons/InCarInventory` using the `InCarInventory` PBO prefix, then put the PBO in the packaged mod's `addons` directory alongside the package's root metadata. Sign your local build with your own key if your server requires signatures.

## Usage

Enter a supported vehicle and open your inventory with **Tab** on the default PC controls. Find the vehicle in the vicinity panel and expand its cargo or attachment categories. Use the normal inventory controls to move items.

## Source layout

- `addons/InCarInventory/config.cpp` declares the addon and its script modules.
- `scripts/4_World/PlayerBase.c` handles inventory access when entering a vehicle and logs entry and exit state.
- `scripts/5_Mission/VicinityItemManager.c` adds the occupied vehicle to the vicinity list.
- `scripts/5_Mission/ICI_VehicleVisibility.c` adapts inventory containers to display the occupied vehicle's cargo and categories.
- `scripts/5_Mission/MissionGameplay.c` logs inventory opening and vehicle visibility for troubleshooting.

The inventory UI includes adaptations from [Bohemia Interactive's DayZ-Script-Diff](https://github.com/BohemiaInteractive/DayZ-Script-Diff). DayZ and the original game scripts are by Bohemia Interactive.
