#pragma once

void load_minigame_signature(){
    NF_InitTextSys(SCREEN_BOTTUM);
    NF_LoadTextFont("fnt/default", "normal", 256, 256, 0); // Load normal text
    NF_CreateTextLayer(SCREEN_BOTTUM, 0, 0, "normal");
    NF_UpdateTextLayers();
    NF_LoadTiledBg("bg/signature_minigame_bottum", "signature_minigame_bottum", 256, 256);
    NF_LoadTiledBg("bg/blank_background", "signature_minigame_top", 256, 256);

    NF_CreateTiledBg(SCREEN_BOTTUM, 1, "signature_minigame_bottum");
    NF_CreateTiledBg(SCREEN_TOP   , 1, "signature_minigame_top");
    NF_LoadSpriteGfx("sprite/pen_sprite", 0, 8, 8);
    NF_LoadSpritePal("sprite/pen_sprite", 0);
    NF_VramSpriteGfx(SCREEN_BOTTUM, 0, 0,false);
    NF_VramSpritePal(SCREEN_BOTTUM, 0, 0);

}

void unload_minigame_signature(){

    NF_DeleteTiledBg(SCREEN_BOTTUM,1);
    NF_DeleteTiledBg(SCREEN_TOP,1);
    NF_UnloadSpriteGfx(0);
    NF_UnloadSpritePal(0);
    
}

void minigame_main_signature(){

    load_minigame_signature();

    int signature_count = 0;

    touchPosition touch;
    int signature_id_next = 0;
    while(1==1){
        if (signature_id_next >= 128){
            signature_id_next = 0;
            signature_count++;
            for(int i = 0; i <= 127;i++){
                NF_DeleteSprite(SCREEN_BOTTUM,i);
            }
        }
        touchRead(&touch);

        scanKeys(); // Read keypad
        u16 keys = keysHeld(); 
        char str[2];
        sprintf(str, "%d", signature_count);

        NF_WriteText(SCREEN_BOTTUM, 0, 1, 1, str);

        NF_UpdateTextLayers();
        NF_SpriteOamSet(SCREEN_TOP);
        NF_SpriteOamSet(SCREEN_BOTTUM);
        NF_UpdateTextLayers();
        swiWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);

        if(keys && KEY_TOUCH){
            NF_CreateSprite(SCREEN_BOTTUM, signature_id_next, 0, 0, touch.px - 4, touch.py - 4);
            signature_id_next++;
        }

    }

}