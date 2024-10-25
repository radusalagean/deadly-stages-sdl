# Deadly Stages Remix
A multi-platform SDL reimplementation of the original 2015 Deadly Stages Demo from @XtremePrime and @popica23

## New features

General:
    - Multi-platform (Linux, Windows, macOS, Sony PSP)
    - Controller support
	- Game Pause state
	- The game viewport adapts to any size (4:3 or wider aspect ratio highly recommended)
	- Separate scaling options for both Gameplay and UI
	- Hud improvements (new layout, more stats displayed)

Behavior:
	- Improved collision handling
	- Player has stamina
	- Player can sprint (stamina-dependent)
	- Player can jump and crush enemies (stamina-dependent)
	- Enemies slow down as they get hurt
	- Enemy bounty is raised as you complete waves
	- More realistic weapon behavior
		- Shotguns fire pellets
		- Reload mechanism
		- Each shot requires a re-press of the "fire" action for semi-automatic weapons
	- Player starts receiving damage from an enemy only after being in continuous contact for 300ms

- New visual effects: 
	- Shadows
	- Landing from a jump will shake the camera

## PSP
- Dev environmnent setup on Ubuntu Linux:
    - Install the required dependencies:
        ```bash
        bash <(curl -s https://raw.githubusercontent.com/pspdev/pspdev/v20241001/prepare.sh)
        ```
        ```bash
        sudo apt install make cmake genisoimage
        ```
    - Get the [`v20241001`](https://github.com/pspdev/pspdev/releases/tag/v20241001) `pspdev-ubuntu-latest-x86_64.tar.gz` archive from the releases page and extract it
    - Move the `pspdev` directory to `~/pspdev`
    - Add in `~/.bashrc`:
        ```bash
        export PSPDEV=~/pspdev
        export PATH=$PATH:$PSPDEV/bin
        ```
    - Run `source ~/.bashrc`
- Preparing a PSP with `psplink` & Linux Host os for running debug builds: https://pspdev.github.io/debugging.html
    - PC: Run `usbhostfs_pc`
    - PSP: Open the PSPLink and connect to PC via USB cable
    - PC: Go to the directory where the build is located and run
        ```
        pspsh -e ./DeadlyStagesRemix.prx
        ```