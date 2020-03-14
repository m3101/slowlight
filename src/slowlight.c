#include "./slowlight.h"
#include <stdlib.h>
#include <stdio.h>
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
The following vectors are general-purpose vectors used under many different names by the functions below.
This makes it so they don't have to spend cycles allocating and freeing memory for temporary vectors.
*/
slvect _slgp0={0,0,0};
slvect _slgp1={0,0,0};
slvect _slgp2={0,0,0};
slvect _slgp3={0,0,0};
slvect _slgp4={0,0,0};

slvect _slgpm0={0,0,0};
slvect _slgpm1={0,0,0};
slvect _slgpm2={0,0,0};
slvect _slgpm3={0,0,0};

slvect slx={1,0,0};
slvect sly={0,1,0};
slvect slz={0,0,1};

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
void freeslraster(slraster** obj)
{
    if(!obj||!(*obj))return;
    if((*obj)->data)free((*obj)->data);
    free(*obj);
    *obj=NULL;
}

slvect* Slvect(double x, double y, double z)
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
    ret->c=0;
    ret->depth=100;
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

slcamera *Slcamera(slraster *image,const slvect *pos,const slvect *dir,const double roll,const double fl,const double w,const double h,const double raystep,const double depth)
{
    if(!image||!pos||!dir)return NULL;
    slcamera *ret=malloc(sizeof(slcamera));
    slray *ray;
    slvect *rpos=&_slgpm0,*rdir=&_slgpm1,*rotaxis=&_slgpm2,*focalpoint=&_slgpm3;
    int length=image->h*image->w;
    int i,x,y;
    ret->pos=*pos;
    ret->dir=*dir;
    slvectnormalize(&ret->dir);
    ret->image=image;
    ret->fl=fl;
    ret->w=w;
    ret->h=h;
    ret->roll=roll;
    ret->raystep=raystep;
    ret->depth=depth;

    /*We first define the focal point of the camera*/
    slvectscale(dir,fl,rdir);
    slvectsub(pos,rdir,focalpoint);

    ret->rays=malloc(length*sizeof(slray));
    for(i=0;i<length;i++)
    {
        x=i%image->w;
        y=floor(i/image->w);
        ret->rays[i]=Slray(image,x,y,rpos,rdir);
    }
    slupdatecamera(ret,0);
    return ret;
}

void slupdatecamera(slcamera *camera,int rnd)
{
    if(!camera)return;
    slray *ray;
    slvect *rpos=&_slgpm0,*rdir=&_slgpm1,*rotaxis=&_slgpm2,*focalpoint=&_slgpm3;
    int length=camera->image->h*camera->image->w;
    int i,x,y;

    /*We first define the focal point of the camera*/
    slvectscale(&camera->dir,camera->fl,rdir);
    slvectsub(&camera->pos,rdir,focalpoint);

    for(i=0;i<length;i++)
    {
        if(camera->rays[i]->c)continue;
        if(rnd)camera->rays[i]->c=rand()%rnd;
        x=i%camera->image->w;
        y=floor(i/camera->image->w);
        /*First we place the vector as if the camera was pointing upwards, that is, (0,0,1) at (0,0,0)*/
        rpos->x=(-(camera->w/2))+((camera->w/(camera->image->w-1))*x);
        rpos->y=(-(camera->h/2))+((camera->h/(camera->image->h-1))*y);
        rpos->z=0;
        /*Then we rotate them so they align with the camera's normal vector*/
        slvectproduct(&camera->dir,&slz,rotaxis);
        slvectrotateaxis(rpos,rotaxis,acos(slscproduct(&camera->dir,&slz)));
        /*Then we roll them to the specified roll*/
        slvectrotateaxis(rpos,&camera->dir,camera->roll);
        /*Then displace them to the camera position*/
        slvectsum(rpos,&camera->pos,rpos);
        /*Now we have positioned the ray, let's find its direction.*/
        slvectsub(rpos,focalpoint,rdir);
        slvectnormalize(rdir);
        slvectscale(rdir,camera->raystep,rdir);
        camera->rays[i]->dir=*rdir;
        camera->rays[i]->pos=*rpos;
        camera->rays[i]->depth=camera->depth;
    }
}

void freeslcamera(slcamera **camera)
{
    if(!camera||!(*camera))return;
    int i,s;
    s=(*camera)->image->h*(*camera)->image->w*(*camera)->image->s;
    for(i=0;i<s;i++)free((*camera)->rays[i]);
    free(*camera);
    *camera=NULL;
}

/*
Vector functions
*/
double slscproduct(const slvect *a,const slvect *b)
{
    if(!a||!b)return -1;
    return a->x*b->x+a->y*b->y+a->z*b->z;
}

void slvectproduct(const slvect *a,const slvect *b,slvect *c)
{
    if(!a||!b||!c)return;
    slvect *temp=&_slgp4;
    temp->x=a->y*b->z-a->z*b->y;
    temp->y=a->z*b->x-a->x*b->z;
    temp->z=a->x*b->y-a->y*b->x;
    c->x=temp->x;
    c->y=temp->y;
    c->z=temp->z;
}

void slvectscale(const slvect *a,double s,slvect *b)
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

void slvectnormalize(slvect *a)
{
    double sf=sqrt(slscproduct(a,a));
    a->x=a->x/sf;
    a->y=a->y/sf;
    a->z=a->z/sf;
}

/*See https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions - General Rotations*/
void slvectrotate(slvect *a,double rx,double ry,double rz)
{
    double srx=sin(rx),sry=sin(ry),srz=sin(rz),crx=cos(rx),cry=cos(ry),crz=cos(rz);
    slvect* tmp=&_slgp0;
    tmp->x=(a->x*(crz*cry))+(a->y*(crz*sry*srx-srz*crx))+(a->z*(crz*sry*crx+srz*srx));
    tmp->y=(a->x*(srz*cry))+(a->y*(srz*sry*srx+crz*crx))+(a->z*(srz*sry*crx-crz*srx));
    tmp->z=(a->x*(-sry))+(a->y*(cry*srx))+(a->z*(cry*crx));
    a->x=tmp->x;
    a->y=tmp->y;
    a->z=tmp->z;
}

/*See https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula*/
void slvectrotateaxis(slvect *a,const slvect *r,double rv)
{
    slvect* res=&_slgp0,*temp=&_slgp1;
    slvectscale(a,cos(rv),res);
    slvectproduct(r,a,temp);
    slvectscale(temp,sin(rv),temp);
    slvectsum(res,temp,res);
    slvectscale(r,slscproduct(r,a)*(1-cos(rv)),temp);
    slvectsum(res,temp,a);
}

/*
Vector-Triangle functions
*/

/*See https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection - Algebraic Form*/
double slvectintersect(const slvect *pos,const slvect *dir,const sltri *t)
{
    if(!pos||!dir||!t)return 0;
    slvect *result=&_slgp0;
    slvect *normal=&_slgp1;
    slvect *edge1=&_slgp2;
    slvect *edge2=&_slgp3;
    double s;
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
    /*A normal vector. We'll use it for checking if the other vectors point roughly towards the same direction*/
    slvect *normal=&_slgp2;
    /*Result vector*/
    slvect *result=&_slgp3;
    double reference;/*Scalar product of the first vector and the normal vector*/
    /*AB edge*/
    slvectsub(&t->a,&t->b,edge);

    /*We'll use this opportunity to calculate the normal vector, too*/
    slvectsub(&t->a,&t->c,vp);
    slvectproduct(vp,edge,normal);

    slvectsub(a,&t->a,vp);
    slvectproduct(vp,edge,result);
    reference=slscproduct(result,normal);
    /*BC edge*/
    slvectsub(&t->b,&t->c,edge);
    slvectsub(a,&t->b,vp);
    slvectproduct(vp,edge,result);
    if(slscproduct(result,normal)*reference<0)return 0;
    /*CA edge*/
    slvectsub(&t->c,&t->a,edge);
    slvectsub(a,&t->c,vp);
    slvectproduct(vp,edge,result);
    if(slscproduct(result,normal)*reference<0)return 0;
    return 1;
}

/*Raycasting functions*/


void slrandray(slcamera *camera,int maxc)
{
    if(!camera)return;
    int i,s;
    s=camera->image->h*camera->image->w;
    for(i=0;i<s;i++)camera->rays[i]->c=rand()%maxc;
}

double pmaxs=0;
double maxs=0;

void slcalcray(slray *ray,const sltri**triangles)
{
    int i=0;
    double s;
    double min=__DBL_MAX__;
    slvect *pos=&_slgpm0,*dir=&_slgpm1;
    if(!ray||!triangles||!(triangles[0]))return;
    double dirnorm=sqrt(slscproduct(&ray->dir,&ray->dir));
    double upperlimit=(ray->c+1)*dirnorm;
    double lowerlimit=(ray->c)*dirnorm;
    if(ray->depth<lowerlimit)
    {
        ray->c=0;
        ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s]=0;
        ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s+1]=0;
        ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s+2]=0;
        return;
    }
    ray->c++;
    double S=0.0;
    double bright;
    while(triangles[i]!=NULL)
    {
        *dir=ray->dir;
        slvectnormalize(dir);
        s=slvectintersect(&ray->pos,dir,triangles[i]);
        if(s>=0&&s<min&&s<upperlimit&&s>lowerlimit)
        {
            slvectscale(dir,s,dir);
            slvectsum(&ray->pos,dir,pos);
            if(slvectintri(pos,triangles[i]))
            {
                maxs=maxs>s?maxs:s;
                min=s;
                bright=s<maxs?(s/maxs):1;
                bright=(S+((1-S)*(1-bright)));
                
                ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s]=bright*(unsigned char)255;//(unsigned char)triangles[i]->colour[0]/(1+bright);
                ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s+1]=bright*(unsigned char)255;//(unsigned char)triangles[i]->colour[1]/(1+bright);
                ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s+2]=bright*(unsigned char)255;//(unsigned char)triangles[i]->colour[2]/(1+bright);
                
                ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s]=(unsigned char)triangles[i]->colour[0]*bright;
                ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s+1]=(unsigned char)triangles[i]->colour[1]*bright;
                ray->screen->data[(ray->rx+ray->ry*ray->screen->w)*ray->screen->s+2]=(unsigned char)triangles[i]->colour[2]*bright;
                
                ray->c=0;
            }
        }
        i++;
    }
}
void slstep(slcamera *camera,const sltri**triangles)
{
    if(!camera||!triangles||!triangles[0])return;
    int i,s;
    maxs=(8*pmaxs)/10;
    s=camera->image->h*camera->image->w*camera->image->s;
    for(i=0;i<s;i++)
        slcalcray(camera->rays[i],triangles);
    pmaxs=maxs;
}