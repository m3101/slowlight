/*
A simple example of using the gfx library.
CSE 20211
9/7/2011
by Prof. Thain
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../src/slowlight.h"
#include "../external/gfx.h"

int main()
{
	slraster *raster=Slraster(200,200,3);
	slvect *pos=Slvect(0,0,2),*dir=Slvect(1,0,0);
	slcamera *cam=Slcamera(raster,pos,dir,0,0.5,2,2,0.10);
	sltri** triangles=malloc(sizeof(sltri)*2);
	slvect *a,*b,*c;
    a=Slvect(5,-5,-5);
    b=Slvect(5,5,-5);
    c=Slvect(5,0,5);
	triangles[0]=Sltri(a,b,c);
	triangles[0]->colour[0]=255;
	triangles[0]->colour[1]=0;
	triangles[0]->colour[2]=0;
	
	triangles[1]=NULL;
	int x,y;
	int ysize = 200;
	int xsize = 200;
	/*//printf("execute({\"Vector((%.2lf,%.2lf,%.2lf),(%.2lf+(%.2lf),%.2lf+(%.2lf),%.2lf+(%.2lf)))\",",cam->pos.x,cam->pos.y,cam->pos.z,cam->pos.x,cam->dir.x,cam->pos.y,cam->dir.y,cam->pos.z,cam->dir.z);
	printf("execute({");
	for(y=0;y<3;y++)
		for(x=0;x<3;x++)
			printf("\"Vector((%.2lf,%.2lf,%.2lf),(%.2lf+(%.2lf),%.2lf+(%.2lf),%.2lf+(%.2lf)))\",",cam->rays[x+3*y]->pos.x,cam->rays[x+3*y]->pos.y,cam->rays[x+3*y]->pos.z,cam->rays[x+3*y]->pos.x,cam->rays[x+3*y]->dir.x,cam->rays[x+3*y]->pos.y,cam->rays[x+3*y]->dir.y,cam->rays[x+3*y]->pos.z,cam->rays[x+3*y]->dir.z);
	printf("\"f=1\"})\n");*/

	char h;

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"Slowlight raster rendering test");

	// Set the current drawing color to green.
	gfx_color(0,200,100);

	// Draw a triangle on the screen.
	gfx_line(100,100,200,100);
	gfx_line(200,100,150,150);
	gfx_line(150,150,100,100);

	int cy=0;
	struct timespec tim, tim2;
	tim.tv_sec = 0;
   	tim.tv_nsec = 5;
	while(cy<9999999) {
		slstep(cam,(const sltri**)triangles);
		for(x=0;x<xsize;x++)
		{
			for(y=0;y<ysize;y++)
			{
				gfx_color(raster->data[x+y*xsize],raster->data[x+y*xsize+1],raster->data[x+y*xsize+2]);
				gfx_point(x,y);
			}
		}
		printf("Cycle.\n");
		// Wait for the user to press a character.
		//h = gfx_wait();
		
		nanosleep(&tim,&tim2);
		cy++;
		// Quit if it is the letter q.
		if(h=='q') break;
	}

	freeslcamera(&cam);
	freeslraster(&raster);
	free(pos);free(dir);
	free(triangles[0]);
	free(triangles);
	return 0;
}
