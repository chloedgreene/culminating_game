// #include <stdlib.h>
// #include <stdio.h>
// #include "dos.h"

// #define MAX_OBJECTS 1024

// typedef enum{
//     NoneItem,
//     Ore = 25,
//     Mineral = 88,
//     RefinedMaterial = 101,
// }item_type;

// typedef enum{
//     NonePart,
//     Pusher,
// }part_type;

// typedef struct{
//     int x;
//     int y;
//     item_type type;
// }item;

// typedef struct
// {
//     int x;
//     int y;
//     part_type part;
// }part;


// item items[MAX_OBJECTS];
// int items_index = 0;
// part parts[MAX_OBJECTS];
// int parts_index = 0;




// int main(int argc, char const *argv[])
// {

//     for(int i = 0; i < MAX_OBJECTS;i++){
//         items[i] = (item){0,0,NoneItem};
//         parts[i] = (part){0,0,NonePart};
//     }

//     items[items_index] = (item){2,2,Ore};
//     items_index++;
//     items[items_index] = (item){2,3,Mineral};
//     items_index++;
//     items[items_index] = (item){3,2,RefinedMaterial};
//     items_index++;


//     parts[parts_index] = (part){2,2,Pusher};
//     parts_index++;



//     setvideomode(videomode_320x200);
//     setdoublebuffer(1);

//     while(!shuttingdown()){

//         if(keystate(KEY_MBUTTON)){
//             parts[parts_index] = (part){mousex(),mousey(),Pusher};
//             parts_index++;   
//         }

//         clearscreen();
//         for(int i = 0; i < MAX_OBJECTS;i++){
//             if(parts[i].part != NonePart){
//                 setcolor(parts[i].part);
//                 bar(parts[i].x*10,parts[i].y*10,10,10);
//             }
//         }   

//         for(int i = 0; i < MAX_OBJECTS;i++){
//             if(items[i].type != NoneItem){
//                 //these are real items in the list, now we do a lil t o m f u c k e r y
//                 setcolor(items[i].type);
//                 bar(items[i].x*5,items[i].y*5,5,5);
//             }
//         }

//         bar(mousex()+2,mousey()+2,4,4);
        

    
//         swapbuffers();
//     }
//     return 0;
// }



#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "dos.h"

int main( int argc, char* argv[] ) {
    setvideomode( videomode_320x200 ); 

    uint8_t palette[ 768 ];
    int mapwidth, mapheight, palcount;
    uint8_t* mapcol = loadgif( "files/C1W.gif", &mapwidth, &mapheight, &palcount, palette );    
    uint8_t* mapalt = loadgif( "files/D1.gif", &mapwidth, &mapheight, NULL, NULL );    

    for( int i = 0; i < palcount; ++i ) {
        setpal(i, palette[ 3 * i + 0 ],palette[ 3 * i + 1 ], palette[ 3 * i + 2 ] );
    }
    setpal( 0, 36, 36, 56 );

    struct {
        float x;       
        float y;        
        float height;   
        float angle;    
        float horizon;  
        float distance; 
    } camera = { 512, 800, 78, 0, 100, 800 };

    setdoublebuffer( 1 );
    uint8_t* screen = screenbuffer();

    while( !shuttingdown() ) {
        waitvbl();
        clearscreen();        

        if( keystate( KEY_A ) ) camera.angle += 0.02f;
        if( keystate( KEY_D ) ) camera.angle -= 0.02f;
        if( keystate( KEY_W ) ) {
            camera.x -= (float)sin( camera.angle ) * 1.1f;
            camera.y -= (float)cos( camera.angle ) * 1.1f;
        }
        if( keystate( KEY_S ) ) {
            camera.x += (float)sin( camera.angle ) * 0.75f;
            camera.y += (float)cos( camera.angle ) * 0.75f;
        }
        if( keystate( KEY_RIGHT ) ) camera.height += 0.5f;
        if( keystate( KEY_LEFT ) ) camera.height -= 0.5f;
        if( keystate( KEY_DOWN ) ) camera.horizon += 1.5f;
        if( keystate( KEY_UP ) ) camera.horizon -= 1.5f;

        int mapwidthperiod = mapwidth - 1;
        int mapheightperiod = mapheight - 1;
        int mapshift = 10;

        int cameraoffs = ( ( ((int)camera.y) & mapwidthperiod ) << mapshift ) + ( ((int)camera.x) & mapheightperiod );
        if( ( mapalt[ cameraoffs ] + 10.0f ) > camera.height ) camera.height = mapalt[ cameraoffs ] + 10.0f;

        int screenwidth = 320;
        int screenheight = 200;
        float sinang = (float)sin( camera.angle );
        float cosang = (float)cos( camera.angle );

        int hiddeny[ 320 ];
        for( int i = 0; i < screenwidth; ++i )  hiddeny[ i ] = screenheight;
        float deltaz = 1.0f;

        for( float z = 1.0f; z < camera.distance; z += deltaz ) {
            float plx =  -cosang * z - sinang * z;
            float ply =   sinang * z - cosang * z;
            float prx =   cosang * z - sinang * z;
            float pry =  -sinang * z - cosang * z;

            float dx = ( prx - plx ) / screenwidth;
            float dy = ( pry - ply ) / screenwidth;
            plx += camera.x;
            ply += camera.y;
            float invz = 1.0f / z * 100.0f;
            for( int i = 0; i < screenwidth; ++i ) {
                int mapoffset = ( ( ((int)ply) & mapwidthperiod ) << mapshift ) + ( ((int)plx) & mapheightperiod );
                int heightonscreen = (int)( ( camera.height - mapalt[ mapoffset ] ) * invz + camera.horizon );
                if( heightonscreen < 0 ) heightonscreen = 0;
                int col = mapcol[ mapoffset ];
                for( int y = heightonscreen; y < hiddeny[ i ]; ++y ) {
                    screen[ i + y * 320 ] = (uint8_t)col;
                }
                if( heightonscreen < hiddeny[ i ] )  hiddeny[ i ] = heightonscreen; 
                plx += dx;
                ply += dy;
            }
            deltaz += 0.005f;
        }
        

        screen = swapbuffers();

        if( keystate( KEY_ESCAPE ) )  break;
    }

    return 0;
}