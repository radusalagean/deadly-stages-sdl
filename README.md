# deadly-stages-sdl
A multi-platform SDL reimplementation of the Deadly Stages Demo

## PSP
- Dev environmnent setup on Ubuntu Linux:
    - Install the required dependencies:
        ```bash
        bash <(curl -s https://raw.githubusercontent.com/pspdev/pspdev/v20240901/prepare.sh)
        ```
        ```bash
        sudo apt install make cmake
        ```
    - Get the [`v20240901`](https://github.com/pspdev/pspdev/releases/tag/v20240901) `pspdev-ubuntu-latest-x86_64.tar.gz` archive from the releases page and extract it
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
        pspsh -e ./DeadlyStagesDemo.prx
        ```