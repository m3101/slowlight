#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../src/slowlight.h"
#include "../external/gfx.h"

int main()
{
	slraster *raster=Slraster(300,300,3);
	slvect *pos=Slvect(4.5,0,2),*dir=Slvect(1,0,0);
	slcamera *cam=Slcamera(raster,pos,dir,0,1,20,20,0.9,100);
	slrandray(cam,100);
	sltri** triangles=malloc(sizeof(sltri)*3);
	slvect *a,*b,*c;
    a=Slvect(5,-5,-5);
    b=Slvect(5,5,-5);
    c=Slvect(5,0,5);
	triangles[0]=Sltri(a,b,c);
	triangles[0]->colour[0]=255;
	triangles[0]->colour[1]=0;
	triangles[0]->colour[2]=0;
	a->x=6.000001;a->y=-50;a->z=-50;
	b->x=6.000001;b->y=50;b->z=-50;
	c->x=6.000001;c->y=0;c->z=50;
	triangles[1]=Sltri(a,b,c);
	triangles[1]->colour[0]=0;
	triangles[1]->colour[1]=0;
	triangles[1]->colour[2]=255;
	
	triangles[2]=NULL;
	int x,y;
	int ysize = 300;
	int xsize = 300;
	/*//printf("execute({\"Vector((%.2lf,%.2lf,%.2lf),(%.2lf+(%.2lf),%.2lf+(%.2lf),%.2lf+(%.2lf)))\",",cam->pos.x,cam->pos.y,cam->pos.z,cam->pos.x,cam->dir.x,cam->pos.y,cam->dir.y,cam->pos.z,cam->dir.z);
	printf("execute({");
	for(y=0;y<2;y++)
		for(x=0;x<2;x++)
			printf("\"Vector((%.2lf,%.2lf,%.2lf),(%.2lf+(%.2lf),%.2lf+(%.2lf),%.2lf+(%.2lf)))\",",cam->rays[x+2*y]->pos.x,cam->rays[x+2*y]->pos.y,cam->rays[x+2*y]->pos.z,cam->rays[x+2*y]->pos.x,cam->rays[x+2*y]->dir.x,cam->rays[x+2*y]->pos.y,cam->rays[x+2*y]->dir.y,cam->rays[x+2*y]->pos.z,cam->rays[x+2*y]->dir.z);
	printf("\"f=1\"})\n");*/

	char h;

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"Slowlight raster rendering test");

	int cy=0,cyc=1500;
	struct timespec tim, tim2;
	tim.tv_sec = 0;
   	tim.tv_nsec = 1;
	while(cy<cyc) {
		slstep(cam,(const sltri**)triangles);
		for(x=0;x<xsize;x++)
		{
			for(y=0;y<ysize;y++)
			{
				gfx_color(raster->data[(x+y*xsize)*3],raster->data[(x+y*xsize)*3+1],raster->data[(x+y*xsize)*3+2]);
				gfx_point(x,y);
			}
		}
		cam->roll+=0.009*((cyc-(double)cy)/cyc);
		cam->raystep+=0.01;
		//cam->pos.x+=.01;
		slupdatecamera(cam);
		nanosleep(&tim,&tim2);
		cy++;
	}

	freeslcamera(&cam);
	freeslraster(&raster);
	free(pos);free(dir);
	free(triangles[0]);
	free(triangles[1]);
	free(triangles);
	printf("Ok.");
	return 0;
}
