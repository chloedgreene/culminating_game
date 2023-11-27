// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023

#include <stdio.h>
#include <time.h>

#include <nds.h>
#include <NEMain.h>
#include <maxmod9.h>

#include "common.h"

#include "graphics.h"

int main(int argc, char **argv)
{
    consoleDemoInit();

    while (1)
    {
        swiWaitForVBlank();
        scanKeys();

        uint16_t keys = keysHeld();
        uint16_t keys_down = keysDown();


    }

    return 0;
}
