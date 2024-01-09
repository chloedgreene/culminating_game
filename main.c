
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "dos.h"

#include "cmath.h"

#define rad2deg 57.2957795131
#define PI 3.14159265358979323846

struct {
    float x;       
    float y;        
    float height;   
    float angle;    
    float horizon;  
    float distance; 
} camera = { 0, -100, 50, 180/rad2deg, 100, 800 };

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
    screenCoords.x = (difference * (320/2)) + (320/2);
    float dist = calculateDistance(worldX,worldY,worldZ,camera.x,camera.y,camera.height);
    float size = init_size;
    size = size / map(dist,0,1024,0,128);
    size = size * init_size;


    if(camera.angle < point_angle){
        screenCoords.x = -69;
    }

    return screenCoords;
}

uint8_t* mapcol;
uint8_t* mapalt;


void render(int mapwidth, int mapheight, uint8_t* screen){
    int mapwidthperiod = mapwidth - 1;
    int mapheightperiod = mapheight - 1;
    int mapshift = 10;

    int cameraoffs = ( ( ((int)camera.y) & mapwidthperiod ) << mapshift ) + ( ((int)camera.x) & mapheightperiod );
    camera.height = mapalt[ cameraoffs ] + 10.0f; // makes player stick to floor

    int screenwidth = 320;
    int screenheight = 200;
    float sinang = (float)sin( camera.angle );
    float cosang = (float)cos( camera.angle );

    int hiddeny[ 320 ];
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
                screen[ i + y * 320 ] = (uint8_t)col; // we acsess the framebuffer directly
            }
            if( heightonscreen < hiddeny[ i ] )  hiddeny[ i ] = heightonscreen; 
            plx += dx; // update planer values
            ply += dy; // update planer values
        }
        deltaz += 0.005f;
    }
}

int main( int argc, char* argv[] ) {

    setvideomode( videomode_320x200 ); 

    uint8_t palette[ 768 ];
    int mapwidth, mapheight, palcount;
    mapcol = loadgif( "files/C1.gif", &mapwidth, &mapheight, &palcount, palette );    
    mapalt = loadgif( "files/D1.gif", &mapwidth, &mapheight, NULL, NULL );    

    for( int i = 0; i < palcount; ++i ) {
        setpal(i, palette[ 3 * i + 0 ],palette[ 3 * i + 1 ], palette[ 3 * i + 2 ] );
    }
    setpal( 0, 36, 36, 56 );

    
    setdoublebuffer( 1 );
    uint8_t* screen = screenbuffer();

    while( !shuttingdown() ) {
        waitvbl();
        clearscreen();        

        float speed = 2.5;

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

        render(mapwidth,mapheight,screen);

        struct ScreenCoordinates screenCoords = worldToScreen(434,164,-110,10);
        float dist = calculateDistance(434,164,0,camera.x,camera.y,camera.height);
        float size = 10;
        size = size / map(dist,0,1024,0,128);
        size = size * 10;
        rectangle(screenCoords.x-size,screenCoords.y-size,size,size);
        screen = swapbuffers();

        if( keystate( KEY_ESCAPE ) )  break;
    }

    return 0;
}
