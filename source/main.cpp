
extern "C"{
    #include <stdio.h>
    #include <nds.h>
    #include <filesystem.h>
    #include <nf_lib.h>
}
#include "const.hpp"

#include "signatureminigame.hpp"


void init_nitro_fs(){

    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();

    // Prepare a NitroFS initialization screen

    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

}
void init_nf(){
    // Initialize 2D engine in both screens and use mode 0
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    // Initialize tiled backgrounds system
    NF_InitTiledBgBuffers();    // Initialize storage buffers
    NF_InitTiledBgSys(0);       // Top screen
    NF_InitTiledBgSys(1);       // Bottom screen

        // Initialize sprite system
    NF_InitSpriteBuffers();     // Initialize storage buffers
    NF_InitSpriteSys(0);        // Top screen
    NF_InitSpriteSys(1);        // Bottom screen
}

bool dot_and_box(int rectX, int rectY, int rectWidth, int rectHeight, int pointX, int pointY) {
    bool xCollision = (pointX >= rectX) && (pointX <= rectX + rectWidth);
    bool yCollision = (pointY >= rectY) && (pointY <= rectY + rectHeight);
    return xCollision && yCollision;
}

void load_title_screen(){
    NF_LoadTiledBg("bg/title_screen_top", "title_top", 256, 256);
    NF_LoadTiledBg("bg/title_screen_bottum", "title_bottum", 256, 256);
}
void unload_title_screen(){
    NF_DeleteTiledBg(SCREEN_TOP,0);
    NF_DeleteTiledBg(SCREEN_BOTTUM,0);
}
void main_title(){
    NF_CreateTiledBg(SCREEN_TOP, 0, "title_top");
    NF_CreateTiledBg(SCREEN_BOTTUM, 0, "title_bottum");

    // NF_LoadSpriteGfx("sprite/debug_16", 0, 64, 64);
    // NF_LoadSpritePal("sprite/debug_16", 0);
    // NF_VramSpriteGfx(SCREEN_BOTTUM, 0, 0); // Ball: Keep all frames in VRAM
    // NF_VramSpritePal(SCREEN_BOTTUM, 0, 0);
    // NF_CreateSprite(SCREEN_BOTTUM, 0, 0, 0, 50, 50);

    touchPosition touch;
    while (1)
    {
        scanKeys(); // Read keypad
        u16 keys = keysDown(); // Keys currently pressed

        touchRead(&touch);
        if(keys && KEY_START){
            unload_title_screen();
            minigame_main_signature();
            load_title_screen();
        }else if (keys & KEY_TOUCH)
        {
            if (dot_and_box(60,43,250,250,touch.px,touch.py)){
                unload_title_screen();
                minigame_main_signature();
                load_title_screen();

            }
        }
        
        
        //NF_SpriteOamSet(0);
        //NF_SpriteOamSet(1);
        swiWaitForVBlank();
        //oamUpdate(&oamMain);
        //oamUpdate(&oamSub);
    }
}


int main(int argc, char **argv)
{

    init_nitro_fs();
    init_nf();

    load_title_screen();
    main_title();

    return 0;
}
