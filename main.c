
//librarys we need
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "dos.h"
#include "cmath.h" // stands for chloe math, its math, but only for me hehehehehe

//constants i need to do m a t h 
#define rad2deg 57.2957795131
#define PI 3.14159265358979323846

//definitions
#define screenwidth  320
#define screenheight  200

//player camera
struct {
    float x;       
    float y;        
    float height;   
    float angle;    
    float horizon;  
    float distance; 
} camera = { 343.1, 963.18, 41, 320/rad2deg, 100, 800 };

struct ScreenCoordinates { // used to do the FUCKING TRIG AHSHAHHS for the worldToScreen
    int x;
    int y;
    int size;
};
struct ScreenCoordinates worldToScreen(float worldX, float worldY, float worldZ,float init_size) {
    // attempts in desmos to solve this problem
    //https://www.desmos.com/calculator/xjy1majyyx
    //https://www.desmos.com/calculator/hksoyyyyf1
    struct ScreenCoordinates screenCoords;

    //we setup debug symbols
    // circle(300,180,15);
    // rectangle(300,180,1,1);
    // setcolor(189);
    // rectangle(300 + (sin(camera.angle) * 15),180 + (cos(camera.angle)* 15),1,1);
    // setcolor(120);
    float point_angle = atan2(worldX - camera.x,worldY - camera.y); // this is the normalized angle from 0-1
    // rectangle(300 + (-sin(point_angle) * 10),180 + (-cos(point_angle)* 10),1,1);
    // setcolor(255);
    // rectangle(300-15,180-20,30,0);
    // setcolor(189);
    float point = sin(point_angle-camera.angle); // get the angle subratcion 
    screenCoords.y = (100 + worldZ) + camera.horizon + (camera.height / PI) - 25;
    float difference = (fmod(camera.angle,PI*2)-fmod(point_angle,PI*2) - PI);
    screenCoords.x = (difference * (screenwidth/2)) + (screenwidth/2);
    float dist = calculateDistance(worldX,worldY,worldZ,camera.x,camera.y,camera.height);
    float size = init_size;
    size = size / map(dist,0,1024,0,128);
    size = size * init_size;


    if(camera.angle < point_angle){
        screenCoords.x = -69;
    }

    return screenCoords;
}



uint8_t palette[ 768 ];
uint8_t* mapcol;
uint8_t* mapalt;
uint8_t* screen;


void render(int mapwidth, int mapheight, uint8_t* screen){
    int mapwidthperiod = mapwidth - 1;
    int mapheightperiod = mapheight - 1;
    int mapshift = 10;

    int cameraoffs = ( ( ((int)camera.y) & mapwidthperiod ) << mapshift ) + ( ((int)camera.x) & mapheightperiod );
    camera.height = mapalt[ cameraoffs ] + 10.0f; // makes player stick to floor

    float sinang = (float)sin( camera.angle );
    float cosang = (float)cos( camera.angle );

    int hiddeny[ screenwidth ];
    for( int i = 0; i < screenwidth; ++i )  hiddeny[ i ] = screenheight;
    float deltaz = 1.0f;

    for( float z = 1.0f; z < camera.distance; z += deltaz ) {
        float plx =  -cosang * z - sinang * z; // thi sis pain in trig
        float ply =   sinang * z - cosang * z; // thi sis pain in trig
        float prx =   cosang * z - sinang * z; // thi sis pain in trig
        float pry =  -sinang * z - cosang * z; // thi sis pain in trig

        float dx = ( prx - plx ) / screenwidth; // get the difference in x
        float dy = ( pry - ply ) / screenwidth; // get in different in y
        plx += camera.x; // get planer x
        ply += camera.y; //  get in planer y
        float invz = 1.0f / z * 100.0f;
        for( int i = 0; i < screenwidth; ++i ) {
            int mapoffset = ( ( ((int)ply) & mapwidthperiod ) << mapshift ) + ( ((int)plx) & mapheightperiod ); // OK OK OK OK  SO EXPLANIATION
            //remember the version i did when i was in grade 9
            //this was the part that was slow
            //and fun fact, shifting the bits actually multiples by n*2
            //and bit shifting only is 1 clock cycle on x86
            //so we get s p e e e d
            // so we get SPEEDDDDDD
            int heightonscreen = (int)( ( camera.height - (mapalt[ mapoffset ]) ) * invz + camera.horizon );
            if( heightonscreen < 0 ) heightonscreen = 0;
            int col = mapcol[ mapoffset ];
            for( int y = heightonscreen; y < hiddeny[ i ]; ++y ) {
                screen[ i + y * screenwidth ] = (uint8_t)col; // we acsess the framebuffer directly
            }
            if( heightonscreen < hiddeny[ i ] )  hiddeny[ i ] = heightonscreen; 
            plx += dx; // update planer values
            ply += dy; // update planer values
        }
        deltaz += 0.005f;
    }
}

void update_player(){
    float speed = 0.05;
    if(keystate(KEY_SHIFT)){
        speed = 0;
    }
    if( keystate( KEY_A ) ) camera.angle += 0.02f;
    if( keystate( KEY_D ) ) camera.angle -= 0.02f;
    if( keystate( KEY_W ) ) {
        camera.x -= (float)sin( camera.angle ) * 1.1 / speed;
        camera.y -= (float)cos( camera.angle ) * 1.1 / speed;
    }
    if( keystate( KEY_S ) ) {
        camera.x += (float)sin( camera.angle ) * 0.75f / speed;
        camera.y += (float)cos( camera.angle ) * 0.75f / speed;
    }
    if( keystate( KEY_RIGHT ) ) camera.height += 0.5f;
    if( keystate( KEY_LEFT ) ) camera.height -= 0.5f;
    if( keystate( KEY_UP ) ) camera.horizon += 1.5f;
    if( keystate( KEY_DOWN ) ) camera.horizon -= 1.5f;
}

void init_and_play_music_main_game(){
    struct sound_t* gamemusic = loadwav( "files/music.wav");
    setsoundmode( soundmode_16bit_mono_44100 );
    playsound(2, gamemusic, 0, 128 ); //setup music
}
void init_and_play_music_final_game(){
    struct sound_t* finalmusic = loadwav( "files/final.wav");
    setsoundmode( soundmode_16bit_mono_44100 );
    playsound(2, finalmusic, 0, 128 ); //setup music
}

void titleScreen(){
        //super basic title screen thingy
    bool title_screen = true;
    camera.x = 301.749786;
    camera.y = 602.356689;
    camera.height = 47.500000;
    camera.angle = 7.625052;
    camera.horizon = 101.500000;
    uint8_t logopalette[ 768 ];
    int logo_pallet_count;
    uint8_t* logo = loadgif( "files/logo.gif", NULL,NULL,&logo_pallet_count,logopalette);    
    for( int i = 0; i < logo_pallet_count; ++i ) {
        setpal(i, logopalette[ 3 * i + 0 ],logopalette[ 3 * i + 1 ], logopalette[ 3 * i + 2 ] ); // load up the pallet with all the colours of the gifs
    }
    while (title_screen)
    {
        waitvbl();
        clearscreen();
        //301.749786 | 602.356689 | 47.500000 ||  7.625052 | 101.500000
        //render(mapwidth,mapheight,screen); //proform the voxel space algorithem
        blit(0,0,logo,320,200,0,0,320,200); // same as blit in pygame :)
        setcolor( 255 );
        outtextxy( 20, 150, "Press enter to start" );
        outtextxy( 20, 140, "WASD to Move" );
        outtextxy( 20, 130, "Arrow Keys to Move Camera" );
        outtextxy( 20, 120, "ESC to exit" );

        screen = swapbuffers();
        if(keystate(KEY_RETURN)){
            title_screen = false;
            //camera = str{ 343.1, 963.18, 41, 320/rad2deg, 100, 800 };
            camera.x = 343.1;
            camera.y = 963.18;
            camera.height = 41;
            camera.angle = 320/rad2deg;
            camera.horizon = 100;
        }
        if(keystate(KEY_ESCAPE)){
        
            exit(0);
        }        
    }
    
}

int main( int argc, char* argv[] ) {

    setvideomode( videomode_320x200 ); 
    int mapwidth, mapheight, palcount;
    mapcol = loadgif( "files/C1.gif", &mapwidth, &mapheight, &palcount, palette );    
    mapalt = loadgif( "files/D.gif", &mapwidth, &mapheight, NULL, NULL );    
    setdoublebuffer( 1 );
    screen = screenbuffer();

    titleScreen();

    //loac colour map info
    for( int i = 0; i < palcount; ++i ) {
        setpal(i, palette[ 3 * i + 0 ],palette[ 3 * i + 1 ], palette[ 3 * i + 2 ] ); // load up the pallet with all the colours of the gifs
    }
    setpal( 0, 36, 36, 56 ); // make the sky b l u e

    bool is_game = true;
    bool temple_map = true;
    bool is_ending = false;

    init_and_play_music_main_game();
    while( !shuttingdown() ) {
        setpal( 0, 36, 36, 56 ); // make the sky b l u e
        if(!temple_map){
            setpal( 0, 12, 12, 12 ); // make the sky b l u e
        }
        waitvbl();
        clearscreen();   

        int star_fly = 0;

        if (!is_ending){
            if(is_game){
                update_player();
                if(!temple_map){
                    srand(3152008);
                    for(int i = 0; i <= 150; i++){
                        circle(rand() % 320,(rand() % 600) + camera.horizon - 600,(i % 2));
                    }
                }
                render(mapwidth,mapheight,screen); //proform the voxel space algorithem
                if(!temple_map){
                    outtextxy(10,180,"When your ready, look up");
                }
                float dist = calculateDistance(fmod(camera.x,1024),fmod(camera.y,1024),camera.height,433.3,168.7,99);
                if(dist <= 100 && temple_map){
                    is_game = false;
                }
                if(camera.horizon >= 340){
                    is_ending = true;
                    is_game = false;
                }
            }else{
                temple_map = false;

                mapcol = loadgif( "files/canyon.gif", &mapwidth, &mapheight, &palcount, palette );    
                mapalt = loadgif( "files/candept.gif", &mapwidth, &mapheight, NULL, NULL );  
                is_game = true;

                //reset pallete
                for( int i = 0; i < palcount; ++i ) {
                    setpal(i, palette[ 3 * i + 0 ],palette[ 3 * i + 1 ], palette[ 3 * i + 2 ] ); // load up the pallet with all the colours of the gifs
                }
                stopsound(2);
                init_and_play_music_final_game();
            }
        }else{
            srand(3152008);
            for(int i = 0; i <= 150; i++){
                circle(rand() % 320,(rand() % 200+star_fly) + star_fly,(i % 2));
            }
            outtextxy(10,20 + 0,"I remember once me and my dad went");
            outtextxy(10,20 + 8,"really north to the tip of quebec");
            outtextxy(10,20 + 8*2,"and I saw the aurora for myself");
            outtextxy(10,20 + 8*3,"in my uncles cottage in the middle");
            outtextxy(10,20 + 8*4,"of nowhere.  once we went down south");
            outtextxy(10,20 + 8*5,"to the grand canyon and saw it in");
            outtextxy(10,20 + 8*6,"all its beauty, and for some reason");
            outtextxy(10,20 + 8*7,"in my dreams/memories i confuse them");
            outtextxy(10,20 + 8*8,"all and i see the northern lights in");
            outtextxy(10,20 + 8*9,"the grand canyon.  ive always wanted");
            outtextxy(10,20 + 8*10,"that momment to actually happen so i");
            outtextxy(10,20 + 8*11,"tried making a game around that idea.");
            outtextxy(10,20 + 8*12,"i know it was a pretty short experence");
            outtextxy(10,20 + 8*13,"but i hope you enjoyed just walking");
            outtextxy(10,20 + 8*14,"with chill music.");
        }

        screen = swapbuffers();

        if( keystate( KEY_ESCAPE ) )  {
            printf("%f | %f | %f ||  %f | %f\n",fmod(camera.x,1024),fmod(camera.y,1024),camera.height,camera.angle,camera.horizon);
            break;
        };
    }

    return 0;
}
