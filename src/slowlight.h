/*
Slowlight raycaster-like renderer

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

#ifndef SLOWLIGHT
#define SLOWLIGHT
#define SL_PI 3.14159265358979323846
/*Data structures and allocation functions*/

/*
A raster image.
*/
typedef struct _slraster {
    char* data;/*Raster data. 0-th byte belongs to the top left pixel. (w*s)-th byte belongs to the leftmost pixel of the second line.*/
    int w;/*Width, in pixels*/
    int h;/*Height, in pixels*/
    int s;/*size of each pixel (e.g. 3 for 0-255 RGB pixels)*/
}slraster;
/*
Instantiates a slraster object.
One should use the freeslraster to free its memory (as it allocates memory for storing the image data)
*/
slraster* Slraster(int w, int h, int s);
/*Frees a slraster object*/
void freeslraster(slraster** obj);

/*
A vector object.
*/
typedef struct _slvect {
    double x;/*The point's x coordinate*/
    double y;/*The point's y coordinate*/
    double z;/*The point's z coordinate*/ 
} slvect;
/*Instantiates a slvect object*/
slvect* Slvect(double x, double y, double z);

/*
A ray object.
*/
typedef struct _slray {
    slraster* screen;/*The raster image that'll receive the ray's value when it hits something*/
    int rx;/*The x coordinate of the pixel that'll be updated*/
    int ry;/*The y coordinate of the pixel that'll be updated*/
    int c;/*Number of cycles passed since last collision*/
    slvect pos;/*A vector that marks the ray's position*/
    slvect dir;/*A direction vector*/
    double depth;/*Depth at which the ray respawns*/
}slray;
/*Instantiates a slray object. Vectors will be copied to the ray, not passed by reference.*/
slray* Slray(slraster* screen, int rx, int ry, const slvect *pos, const slvect *dir);

/*
A triangle object
*/
typedef struct _sltri {
    slvect a;/*First vertext*/
    slvect b;/*Second vertex*/
    slvect c;/*Third vertex*/
    char colour[3];/*Triangle colour*/
}sltri;
/*Instantiates a sltri object. Vectors will be copied to the triangle, not passed by reference.*/
sltri* Sltri(const slvect *a,const slvect *b,const slvect *c);

/*
A camera object.
One should free its memory with freeslcamera, as it allocates many ray objects which might not be freed automatically.
*/
typedef struct _slcamera{
    slraster *image;/*Image object*/
    slray** rays;/*slray objects associated with this camera*/
    slvect pos;/*Camera position*/
    slvect dir;/*Camera orientation*/
    double roll;/*Camera roll angle (extra orientation component)*/
    double fl;/*"Focal length" of the camera. Affects the ray angles*/
    double w;/*Camera width in "real-world" units (same units as the vectors)*/
    double h;/*Camera height in "real-world" units (same units as the vectors)*/
    double raystep;/*Camera raystep*/
    double depth;/*Depth at which rays respawn*/
} slcamera;
/*
Generates a slcamera object from a slraster object and parameters.
Most parameters come from slcamera properties.
Vectors will be copied, not stored by reference, so you can reuse them without affecting the camera.
raystep dictates how long each ray will move for each step
*/
slcamera *Slcamera(slraster *image,const slvect *pos,const slvect *dir,const double roll,const double fl,const double w,const double h,const double raystep,const double depth);
/*Updates a camera's rays to its current parameters and position*/
void slupdatecamera(slcamera *camera);
/*Frees a slcamera object*/
void freeslcamera(slcamera **camera);

/*Vector functions*/

/*
Scalar product (takes two vector as input and outputs their internal product (sum of products of coordinates))
*/
double slscproduct(const slvect *a,const slvect *b);
/*
3d-Vectorial product. Outputs a vector perpendicular to both input vectors at c.
C = A x B
slvects A,B,C;
slscproduct(&A,&B,&C);
A=(a,b,c), b=(d,e,f)
# # | i  j  k |
p = | a  b  c | = (bf-ce,cd-af,ae-bd)
# # | d  e  f |
*/
void slvectproduct(const slvect *a,const slvect *b,slvect *c);
/*
Multiply a vector by a scalar. B=s*A
*/
void slvectscale(const slvect *a,double s,slvect *b);
/*
Sum two vectors. C=A+B
*/
void slvectsum(const slvect *a,const slvect *b, slvect *c);
/*
Subtract two vectors. C=A-B
*/
void slvectsub(const slvect *a,const slvect *b, slvect *c);

/*Normalizes a vector (so that its length is 1)*/
void slvectnormalize(slvect *a);

/*Rotates a vector around the x,y and z axes by rx,ry and rz radians.*/
void slvectrotate(slvect *a,double rx,double ry,double rz);

/*Rotates a vector around the v axis by rv radians.*/
void slvectrotateaxis(slvect *a,const slvect *r,double rv);

/*Vector-Triangle functions*/

/*
Calculates the factor by which a vector should be scaled to interesect a plane defined by a triangle t.
Said vector is defined by two vectors, one indicating its position and one for the orientation
Vector A,B; Triangle T
s=slvectintersect(a,t)
If s is 0:
    Vector is already on plane.
If s is inf:
    Vector is parallel to the plane.
Else:
    Point p= A.s
    p lies on the plane on which T lies.
*/
double slvectintersect(const slvect *pos,const slvect *dir,const sltri *t);
/*
Calculates whether a point (vector) lies within the subspace defined by a triangle sliding along its normal axis.
(E.g. if the point and the triangle are at the same plane, calculates whether the point is inside the triangle).
Returns 1 when the point lies within the subspace and 0 otherwise (or -1 for errors).
*/
char slvectintri(const slvect *a,const sltri *t);

/*Raycasting functions*/

/*Randomizes the initial positions of the rays*/
void slrandray(slcamera *camera,int maxc);
/*Calculates one cycle of a ray*/
void slcalcray(slray *ray,const sltri**triangles);
/*Cycles all the camera's rays*/
void slstep(slcamera *camera,const sltri**triangles);

/*Constants*/
/*(1,0,0)*/
slvect slx;
/*(0,1,0)*/
slvect sly;
/*(0,0,1)*/
slvect slz;

#endif