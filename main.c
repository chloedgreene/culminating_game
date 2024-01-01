
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "dos.h"

typedef struct{
    float posx;
    float posy;
    float angle
} player;

int main( int argc, char* argv[] ) {

    player p = {
        0,0,0
    };

    setvideomode( videomode_320x200 ); 
    

    while( !shuttingdown() ) {
        waitvbl();
        clearscreen();        

        if(keystate(KEY_W)){
            p.posx += sin(p.angle);
            p.posy += cos(p.angle);
        }
        else if (keystate(KEY_S))
        {
            p.posx -= sin(p.angle);
            p.posy -= cos(p.angle);
        }

        if(keystate(KEY_A)){
            p.angle += 0.05;
        }
        else if (keystate(KEY_D))
        {
            p.angle -= 0.05;
        }


        
        circle(p.posx,p.posy,5);
        line(p.posx,p.posy,p.posx + sin(p.angle) * 5, p.posy + cos(p.angle) * 5);
        

        if( keystate( KEY_ESCAPE ) )  break;
    }

    return 0;
}