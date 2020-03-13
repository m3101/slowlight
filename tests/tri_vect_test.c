#include "../src/slowlight.h"
#include <stdio.h>
#include <stdlib.h>

/*
See docs/interpreting_results.png for a visualisation of this test
*/

int main()
{
    slvect *a,*b,*c,*vpos,*vdir;
    sltri *tri;
    float s;
    a=Slvect(5,-5,-5);
    b=Slvect(5,5,-5);
    c=Slvect(5,0,5);
    vpos=Slvect(0,0,-4);
    vdir=Slvect(1,0,0);
    tri=Sltri(a,b,c);

    /*TEST 1*/
    s=slvectintersect(vpos,vdir,tri);
    slvectscale(vdir,s,a);
    slvectsum(vpos,a,a);
    printf("Vector interesects triangle plane at (%.2lf,%.2lf,%.2lf)\n",a->x,a->y,a->z);

    /*The following test was visualized in said image*/
    /*TEST 2*/
    vdir->z=0.5;
    vdir->y=0.25;

    s=slvectintersect(vpos,vdir,tri);
    slvectscale(vdir,s,a);
    slvectsum(vpos,a,a);
    printf("Vector interesects triangle plane at (%.2lf,%.2lf,%.2lf)\n",a->x,a->y,a->z);
    printf("Vector lies %s triangle.\n",slvectintri(a,tri)?"in":"outside");

    /*TEST 3*/
    vdir->z=1;
    vdir->y=0.5;

    s=slvectintersect(vpos,vdir,tri);
    slvectscale(vdir,s,a);
    slvectsum(vpos,a,a);
    printf("Vector interesects triangle plane at (%.2lf,%.2lf,%.2lf)\n",a->x,a->y,a->z);
    printf("Vector lies %s triangle.\n",slvectintri(a,tri)?"in":"outside");

    free(a);free(b);free(c);
    free(vpos);free(vdir);
    return 0;
}