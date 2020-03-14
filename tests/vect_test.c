#include "../src/slowlight.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    slvect *a,*b,*c,*z;
    a=Slvect(1,0,0);
    b=Slvect(0,1,0);
    c=Slvect(0,0,0);
    z=Slvect(0,0,1);
    slvectproduct(a,b,c);
    printf("a=(%.2lf,%.2lf,%.2lf),b=(%.2lf,%.2lf,%.2lf)\n",a->x,a->y,a->z,b->x,b->y,b->z);
    printf("Scalar product: %.2lf\n",slscproduct(a,b));
    printf("Cross product: c=(%.2lf,%.2lf,%.2lf)\n",c->x,c->y,c->z);
    a->x=1;a->y=1;a->z=1;
    b->x=1;b->y=-1;b->z=1;
    slvectproduct(a,b,c);
    printf("a=(%.2lf,%.2lf,%.2lf),b=(%.2lf,%.2lf,%.2lf)\n",a->x,a->y,a->z,b->x,b->y,b->z);
    printf("Scalar product: %.2lf\n",slscproduct(a,b));
    printf("Cross product: c=(%.2lf,%.2lf,%.2lf)\n",c->x,c->y,c->z);

    a->x=1;a->y=0;a->z=0;
    slvectrotate(a,0,0,SL_PI/2);
    printf("a=(%.2lf,%.2lf,%.2lf),b=(%.2lf,%.2lf,%.2lf)\n",a->x,a->y,a->z,b->x,b->y,b->z);

    a->x=1;a->y=1;a->z=0;
    b->x=0;b->y=1;b->z=0;
    slvectnormalize(b);
    slvectproduct(b,z,c);
    slvectrotateaxis(a,c,acos(slscproduct(b,z)));
    printf("a=(%.2lf,%.2lf,%.2lf),b=(%.2lf,%.2lf,%.2lf),c=(%.2lf,%.2lf,%.2lf)\n",a->x,a->y,a->z,b->x,b->y,b->z,c->x,c->y,c->z);

    free(a);free(b);free(c);free(z);
    return 0;
}