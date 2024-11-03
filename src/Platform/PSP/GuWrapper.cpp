#include "GuWrapper.hpp"

#include <pspgu.h>
#include <pspdisplay.h>

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

namespace GuWrapper
{
    static unsigned int __attribute__((aligned(16))) list[1024];

    void render()
    {
        sceGuStart(GU_DIRECT,list);
        sceGuClearColor(0xff554433);
        sceGuClearDepth(0);
        sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
        sceGuFinish();
        sceGuSync(0,0);
    }

    void waitForVblank()
    {
        sceDisplayWaitVblankStart();
    }

    void swapBuffers()
    {
        sceGuSwapBuffers();
    }
};
