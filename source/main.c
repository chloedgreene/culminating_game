#include <nds.h>
#include <stdio.h>
#include "colour.h"
#include "depth.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define MAP_N 1024
#define SCALE_FACTOR 70.0

///////////////////////////////////////////////////////////////////////////////
// Camera struct type declaration
///////////////////////////////////////////////////////////////////////////////
typedef struct {
  float x;         // x position on the map
  float y;         // y position on the map
  float height;    // height of the camera
  float horizon;   // offset of the horizon position (looking up-down)
  float zfar;      // distance of the camera looking forward
  float angle;     // camera angle (radians, clockwise)
} camera_t;

///////////////////////////////////////////////////////////////////////////////
// Camera initialization
///////////////////////////////////////////////////////////////////////////////
camera_t camera = {
  .x       = 512.0,
  .y       = 512.0,
  .height  = 70.0,
  .horizon = 60.0,
  .zfar    = 600.0,
  .angle   = 1.5 * 3.141592 // (= 270 deg)
};

#define WIDTH_ITER 1
#define RENDER_DETAIL 2
#define HEIGHT_SCALE 2

float sin(float angle)
{
	int32 s = sinLerp((int)((angle * DEGREES_IN_CIRCLE) / 360.0));

	return f32tofloat(s);
}
float cos(float angle)
{
	int32 c = cosLerp((int)((angle * DEGREES_IN_CIRCLE) / 360.0));

	return f32tofloat(c);
}
typedef struct {
    float x;
    float y;
} Point;


int main(void)
{
	
    lcdMainOnTop();
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
    videoSetModeSub(MODE_5_2D);
    bgInitSub(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    
    
	while(1)
	{
		for(int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT;i++){
			VRAM_A[i] = RGB15(2,20,31);
		}

 		float sinangle = sin(camera.angle);
    	float cosangle = cos(camera.angle);
		
		// left most rednered point
    	float plx = cosangle * camera.zfar + sinangle * camera.zfar;
    	float ply = sinangle * camera.zfar - cosangle * camera.zfar;

    	// right most
    	float prx = cosangle * camera.zfar - sinangle * camera.zfar;
    	float pry = sinangle * camera.zfar + cosangle * camera.zfar;

    	// Loop 320 rays from left to right
    	for (int i = 0; i < SCREEN_WIDTH; i++) {
    	  float deltax = (plx + (prx - plx) / SCREEN_WIDTH * i) / camera.zfar;
    	  float deltay = (ply + (pry - ply) / SCREEN_WIDTH * i) / camera.zfar;

    	  // Ray (x,y) coords
    	  float rx = camera.x;
    	  float ry = camera.y;

    	  // Store the tallest projected height per-ray
    	  float tallestheight = SCREEN_HEIGHT;

    	  // Loop all depth units until the zfar distance limit
    	  for (int z = 1; z < camera.zfar; z++) {
    	    rx += deltax;
    	    ry += deltay;

    	    // Find the offset that we have to go and fetch values from the heightmap
    	    int mapoffset = ((MAP_N * ((int)(ry) & (MAP_N - 1))) + ((int)(rx) & (MAP_N - 1)));

    	    // Project height values and find the height on-screen
    	    int projheight = (int)((camera.height - imageDataDepth[mapoffset]) / z * SCALE_FACTOR + camera.horizon);

    	    // Only draw pixels if the new projected height is taller than the previous tallest height
    	    if (projheight < tallestheight) {
    	      // Draw pixels from previous max-height until the new projected height
    	      for (int y = projheight; y < tallestheight; y++) {
    	        if (y >= 0) {
    	          VRAM_A[(SCREEN_WIDTH * y) + i] = imageDataColor[mapoffset];
    	        }
    	      }
    	      tallestheight = projheight;
    	    }
    	  }
    	}
		//p, phi, height, horizon, scale_height, distance, screen_width, screen_height
		//Render( Point(0, 0), 0, 50, 120, 120, 300, 800, 600 )

		swiWaitForVBlank();
	}
 
	return 0;
}