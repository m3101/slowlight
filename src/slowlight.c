#include "./slowlight.h"
#include <stdlib.h>
#include <math.h>
/*
Copyright (c) 2020 Amélia O. F. da S.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
Instantiators
*/

slraster* Slraster(int w, int h, int s)
{
    slraster *ret=malloc(sizeof(slraster));
    ret->data=malloc(w*h*s);
    ret->w=w;
    ret->h=h;
    ret->s=s;
    return ret;
}
void freeSlraster(slraster** obj)
{
    if(!obj||!(*obj))return;
    if((*obj)->data)free((*obj)->data);
    free(*obj);
    *obj=NULL;
}

slvect* Slvect(float x, float y, float z)
{
    slvect* ret=malloc(sizeof(slvect));
    ret->x=x;
    ret->y=y;
    ret->z=z;
    return ret;
}

slray* Slray(slraster* screen, int rx, int ry, const slvect *pos, const slvect *dir)
{
    slray* ret=malloc(sizeof(slray));
    ret->screen=screen;
    ret->rx=rx;
    ret->ry=ry;
    ret->pos=*pos;
    ret->dir=*dir;
    return ret;
}

sltri* Sltri(const slvect *a,const slvect *b,const slvect *c)
{
    sltri* ret=malloc(sizeof(sltri));
    ret->a=*a;
    ret->b=*b;
    ret->c=*c;
    return ret;
}

/*
Vector functions
*/
float slscproduct(const slvect *a,const slvect *b)
{
    if(!a||!b)return -1;
    return a->x*b->x+a->y*b->y+a->z*b->z;
}

void slvectproduct(const slvect *a,const slvect *b,slvect *c)
{
    if(!a||!b||!c)return;
    c->x=a->y*b->z-a->z*b->y;
    c->y=a->z*b->x-a->x*b->z;
    c->z=a->x*b->y-a->y*b->x;
}

void slvectscale(const slvect *a,float s,slvect *b)
{
    if(!a||!b)return;
    b->x=a->x*s;
    b->y=a->y*s;
    b->z=a->z*s;
}

void slvectsum(const slvect *a,const slvect *b, slvect *c)
{
    if(!a||!b||!c)return;
    c->x=a->x+b->x;
    c->y=a->y+b->y;
    c->z=a->z+b->z;
}

void slvectsub(const slvect *a,const slvect *b, slvect *c)
{
    if(!a||!b||!c)return;
    c->x=a->x-b->x;
    c->y=a->y-b->y;
    c->z=a->z-b->z;
}

/*
Vector-Triangle functions
*/

/*
The following vectors are general-purpose vectors used under many different names by the functions below.
This makes it so they don't have to spend cycles allocating and freeing memory for temporary vectors.
*/
slvect _slgp0={0,0,0};
slvect _slgp1={0,0,0};
slvect _slgp2={0,0,0};
slvect _slgp3={0,0,0};

/*See https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection - Algebraic Form*/
float slvectintersect(const slvect *pos,const slvect *dir,const sltri *t)
{
    if(!pos||!dir||!t)return 0;
    slvect *result=&_slgp0;
    slvect *normal=&_slgp1;
    slvect *edge1=&_slgp2;
    slvect *edge2=&_slgp3;
    float s;
    /*First we calculate the vectors corresponding to the edges of the triangle*/
    slvectsub(&t->a,&t->b,edge1);
    slvectsub(&t->a,&t->c,edge2);
    /*Then the normal*/
    slvectproduct(edge1,edge2,normal);
    s=slscproduct(dir,normal);
    /*If the vector is parallel to the plane*/
    if(s==0)return INFINITY;
    slvectsub(&t->a,pos,result);
    s=slscproduct(result,normal)/s;
    return s;
}
char slvectintri(const slvect *a,const sltri *t)
{
    if(!a||!t)return -1;
    /*A vector representing the edge we are currently analysing*/
    slvect *edge=&_slgp0;
    /*A vector starting on the first vertex of the edge and ending on our point*/
    slvect *vp=&_slgp1;
    /*A vector for storing the first cross product. We'll use it for checking if the other vectors point roughly towards the same direction*/
    slvect *ref=&_slgp2;
    /*A vector for the other cross products*/
    slvect *result=&_slgp3;
    /*AB edge*/
    slvectsub(&t->a,&t->b,edge);
    slvectsub(a,&t->a,vp);
    slvectproduct(edge,vp,ref);
    /*BC edge*/
    slvectsub(&t->b,&t->c,edge);
    slvectsub(a,&t->b,vp);
    slvectproduct(edge,vp,result);
    if(slscproduct(result,ref)<0)return 0;
    /*CA edge*/
    slvectsub(&t->c,&t->a,edge);
    slvectsub(a,&t->c,vp);
    slvectproduct(edge,vp,result);
    if(slscproduct(result,ref)<0)return 0;
    return 1;
}