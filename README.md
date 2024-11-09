# Deadly Stages Remix

A multi-platform remake of the original 2015 Deadly Stages Demo from @XtremePrime and @popica23

## Clone
```
git clone --recurse-submodules <repository-url>
```

## Dependencies (verified versions - known to work)
- SDL2: `2.30.9`
- SDL_image: `2.8.2`
- SDL_mixer: `2.8.0`
- SDL_ttf: `2.22.0`

## Development environment
- Visual Studio Code
    - Plugins:
        - C/C++ Extension Pack (`ms-vscode.cpptools-extension-pack`)
        - CodeLLDB (`vadimcn.vscode-lldb`)
- Open the project: Open Workspace from File: `src/Platform/{PLATFORM}/.vscode/DeadlyStages.code-workspace`

## Build instructions

### Linux target (from Linux environment)
- Install dependencies using apt:
    ```bash
    sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
    ```

### Windows target (from Linux environment)
- Install compiler:
    ```bash
    sudo apt install g++-mingw-w64-x86-64
    ```
- Get the mingw devel and the runtime win64 SDL dependencies for Windows from GitHub Release pages: 
    - https://github.com/libsdl-org/SDL/releases
    - https://github.com/libsdl-org/SDL_image/releases
    - https://github.com/libsdl-org/SDL_mixer/releases
    - https://github.com/libsdl-org/SDL_ttf/releases
- Extract them in `~/sdl2-devel-mingw/`

### macOS target (from macOS environment)
- Install dependencies using brew:
    ```bash
    brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
    ```

### PSP target (from Linux environment)
- Dev environmnent setup on Ubuntu Linux:
    - Install the required dependencies:
        ```bash
        bash <(curl -s https://raw.githubusercontent.com/pspdev/pspdev/v20241101/prepare.sh)
        ```
        ```bash
        sudo apt install make cmake genisoimage
        ```
    - Get the [`v20241101`](https://github.com/pspdev/pspdev/releases/tag/v20241101) `pspdev-ubuntu-latest-x86_64.tar.gz` archive from the releases page and extract it
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
- For creating a PSP Release ISO, you need to have the `PARAM.SFO` file. As of the current `v20241101` PSP SDK version, there is a post-build Cmake command that removes `PARAM.SFO` after the build is complete. To avoid this, before creating a release build, you can remove the following lines from the `$PSPDEV/psp/share/CreatePBP.cmake` file:
    ```cmake
    add_custom_command(
      TARGET ${ARG_TARGET} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E remove
      ${ARG_OUTPUT_DIR}/PARAM.SFO
      COMMENT "Cleaning up PARAM.SFO for target ${ARG_TARGET}"
    )
    ```
