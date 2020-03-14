#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "../src/slowlight.h"
#include "../src/slt.h"
#include "../external/gfx.h"

int main()
{
    int s=150;
	slraster *raster=Slraster(s,s,3);
	slvect *pos=Slvect(-4,4,4),*dir=Slvect(1,-1,0);
	slcamera *cam=Slcamera(raster,pos,dir,-SL_PI/4,2,5,5,1.9,100);
    slrandray(cam,100);
	sltri** triangles=sltToSltriList("build/polgono.slt");
    if(!triangles)
        printf("polgono.slt not found!\n");
    else printf("polgono.slt imported. Length: %d\n",sllen((void**)triangles));
	int x,y;
	int ysize = s;
	int xsize = s;
	printf("execute({");
	for(y=0;y<7;y++)
		printf("\"Polygon({(%.2lf,%.2lf,%.2lf),(%.2lf,%.2lf,%.2lf),(%.2lf,%.2lf,%.2lf)})\",",triangles[y]->a.x,triangles[y]->a.y,triangles[y]->a.z,triangles[y]->b.x,triangles[y]->b.y,triangles[y]->b.z,triangles[y]->c.x,triangles[y]->c.y,triangles[y]->c.z);
	printf("\"f=1\"})\n");
    printf("Colours:\n");
	for(y=0;y<7;y++)
		printf("(%X,%X,%X)\n",triangles[y]->colour[0],triangles[y]->colour[1],triangles[y]->colour[2]);

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
        //cam->pos.z+=0.01;
        cam->roll+=cy<200?0.01:0;
		//cam->fl=1+1*sin((double)cy/1000);
		slupdatecamera(cam);
		nanosleep(&tim,&tim2);
		cy++;
	}

	freeslcamera(&cam);
	freeslraster(&raster);
	free(pos);free(dir);
	freesltriarray(&triangles);
	printf("Ok.");
	return 0;
}
