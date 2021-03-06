# sm64ex practice mod
Fork of [sm64pc/sm64ex](https://github.com/sm64pc/sm64ex) with additional speedrun practicing features.

Made possible by the existing patches DynOS and time-trials, huge thank you to it's creators.

Feel free to report bugs and contribute, but remember, there must be **no upload of any copyrighted asset**. 
Run `./extract_assets.py --clean && make clean` or `make distclean` to remove ROM-originated content.

## Pratice mod features
 * Timer for all courses
 * Leaderboard of best times for each star
 * Real-time ghost replay of best time for each star
 * Instantly restart level by pressing L+R
 * Warp to level
 * Warp to castle
 * Always spawn Mips if you have >= 15 stars
 * Always make cap unlock boxes pressable

## To-do list
 * Make all options effecting gameplay configurable
 * When warping directly to a subarea, the timer gets stuck
 * Cannot restart a specific subarea
 * Mario cannot fly in the ending cutscene
 * Restart castle movement (return to last level exit)

## sm64ex features
 * Options menu with various settings, including button remapping.
 * Optional external data loading (so far only textures and assembled soundbanks), providing support for custom texture packs.
 * Optional analog camera and mouse look (using [Puppycam](https://github.com/FazanaJ/puppycam)).
 * Optional OpenGL1.3-based renderer for older machines, as well as the original GL2.1, D3D11 and D3D12 renderers from Emill's [n64-fast3d-engine](https://github.com/Emill/n64-fast3d-engine/).
 * Option to disable drawing distances.
 * Optional model and texture fixes (e.g. the smoke texture).
 * Skip introductory Peach & Lakitu cutscenes with the `--skip-intro` CLI option
 * Cheats menu in Options (activate with `--cheats` or by pressing L thrice in the pause menu).
 * Support for both little-endian and big-endian save files (meaning you can use save files from both sm64-port and most emulators), as well as an optional text-based save format.

Recent changes in Nightly have moved the save and configuration file path to `%HOMEPATH%\AppData\Roaming\sm64ex` on Windows and `$HOME/.local/share/sm64ex` on Linux. This behaviour can be changed with the `--savepath` CLI option.
For example `--savepath .` will read saves from the current directory (which not always matches the exe directory, but most of the time it does);
   `--savepath '!'` will read saves from the executable directory.

## Building
For building instructions, please refer to the [wiki](https://github.com/sm64pc/sm64ex/wiki).

**Make sure you have MXE first before attempting to compile for Windows on Linux and WSL. Follow the guide on the wiki.**
