/*
A simple example of using the gfx library.
CSE 20211
9/7/2011
by Prof. Thain
*/

#include <stdio.h>
#include <stdlib.h>
#include "../src/slowlight.h"
#include "../external/gfx.h"

int main()
{
	slraster *raster=Slraster(3,3,3);
	slvect *pos=Slvect(0,0,2),*dir=Slvect(1,0,0);
	slcamera *cam=Slcamera(raster,pos,dir,0,3,2,2,1);

	//printf("execute({\"Vector((%.2lf,%.2lf,%.2lf),(%.2lf+(%.2lf),%.2lf+(%.2lf),%.2lf+(%.2lf)))\",",cam->pos.x,cam->pos.y,cam->pos.z,cam->pos.x,cam->dir.x,cam->pos.y,cam->dir.y,cam->pos.z,cam->dir.z);
	printf("execute({");
	int x,y;
	for(y=0;y<3;y++)
		for(x=0;x<3;x++)
			printf("\"Vector((%.2lf,%.2lf,%.2lf),(%.2lf+(%.2lf),%.2lf+(%.2lf),%.2lf+(%.2lf)))\",",cam->rays[x+3*y]->pos.x,cam->rays[x+3*y]->pos.y,cam->rays[x+3*y]->pos.z,cam->rays[x+3*y]->pos.x,cam->rays[x+3*y]->dir.x,cam->rays[x+3*y]->pos.y,cam->rays[x+3*y]->dir.y,cam->rays[x+3*y]->pos.z,cam->rays[x+3*y]->dir.z);
	printf("\"f=1\"})\n");
	int ysize = 300;
	int xsize = 600;

	char c;

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"Slowlight raster rendering test");

	// Set the current drawing color to green.
	gfx_color(0,200,100);

	// Draw a triangle on the screen.
	gfx_line(100,100,200,100);
	gfx_line(200,100,150,150);
	gfx_line(150,150,100,100);

	while(1) {
		// Wait for the user to press a character.
		c = gfx_wait();

		// Quit if it is the letter q.
		if(c=='q') break;
	}

	freeslcamera(&cam);
	freeslraster(&raster);
	free(pos);free(dir);
	return 0;
}
