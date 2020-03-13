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
One should use the freeSlraster to free its memory (as it allocates memory for storing the image data)
*/
slraster* Slraster(int w, int h, int s);
/*Frees a slraster object*/
void freeSlraster(slraster** obj);

/*
A vector object.
*/
typedef struct _slvect {
    float x;/*The point's x coordinate*/
    float y;/*The point's y coordinate*/
    float z;/*The point's z coordinate*/ 
} slvect;
/*Instantiates a slvect object*/
slvect* Slvect(float x, float y, float z);

/*
A ray object.
*/
typedef struct _slray {
    slraster* screen;/*The raster image that'll receive the ray's value when it hits something*/
    int rx;/*The x coordinate of the pixel that'll be updated*/
    int ry;/*The y coordinate of the pixel that'll be updated*/
    slvect pos;/*A vector that marks the ray's position*/
    slvect dir;/*A direction vector*/
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

/*Vector functions*/

/*
Scalar product (takes two vector as input and outputs their internal product (sum of products of coordinates))
*/
float slscproduct(const slvect *a,const slvect *b);
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
void slvectscale(const slvect *a,float s,slvect *b);
/*
Sum two vectors. C=A+B
*/
void slvectsum(const slvect *a,const slvect *b, slvect *c);
/*
Subtract two vectors. C=A-B
*/
void slvectsub(const slvect *a,const slvect *b, slvect *c);

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
float slvectintersect(const slvect *pos,const slvect *dir,const sltri *t);
/*
Calculates whether a point (vector) lies within the subspace defined by a triangle sliding along its normal axis.
(E.g. if the point and the triangle are at the same plane, calculates whether the point is inside the triangle).
Returns 1 when the point lies within the subspace and 0 otherwise (or -1 for errors).
*/
char slvectintri(const slvect *a,const sltri *t);

/*Raycasting functions*/

#endif