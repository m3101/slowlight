#include "slt.h"
#include "slowlight.h"
#include <stdio.h>
#include <stdlib.h>
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
sltri** sltToSltriList(const char* fname)
{
    sltri **ret=NULL;
    slvect *aa=Slvect(0,0,0),*bb=Slvect(0,0,0),*cc=Slvect(0,0,0);
    FILE* f=fopen(fname,"r");
    if(!f)return NULL;
    char r,g,b;
    int i=0,len,_r,_g,_b;
    while(!feof(f))
    {
        if(fgetc(f)=='#'){fscanf(f,"%*[^\n]");fscanf(f,"%*[\n\r]");}
        else{
            fseek(f,-1,SEEK_CUR);
            if(ret)
            {
                fscanf(f,"%d",&_r);
                r=(char)_r;
                fscanf(f,"%d",&_g);
                g=(char)_g;
                fscanf(f,"%d",&_b);
                b=(char)_b;
                fscanf(f,"%lf",&aa->x);
                fscanf(f,"%lf",&aa->y);
                fscanf(f,"%lf",&aa->z);
                fscanf(f,"%lf",&bb->x);
                fscanf(f,"%lf",&bb->y);
                fscanf(f,"%lf",&bb->z);
                fscanf(f,"%lf",&cc->x);
                fscanf(f,"%lf",&cc->y);
                fscanf(f,"%lf",&cc->z);
                ret[i]=Sltri(aa,bb,cc);
                ret[i]->colour[0]=r;
                ret[i]->colour[1]=g;
                ret[i++]->colour[2]=b;
                fscanf(f,"%*[\n\r]");
            }
            else{
                fscanf(f,"%d",&len);
                ret=malloc(sizeof(sltri*)*len+1);
                ret[len]=NULL;
                fscanf(f,"%*[^\n]");fscanf(f,"%*[\n\r]");
            }
        }
    }
    fclose(f);
    return ret;
}

void freesltriarray(sltri*** array)
{
    if(!array||!(*array))return;
    int i=0;
    while((*array)[i]!=NULL)free((*array)[i++]);
    free(*array);
    *array=NULL;
}

int sllen(void** array)
{
    if(!array)return 0;
    int i=0;
    while(array[i++]!=NULL){}
    return i-1;
}