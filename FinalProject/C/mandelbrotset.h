#ifndef __MANDELBROTSET_H__
#define __MANDELBROTSET_H__

#include"defs.h"
#include<math.h>
const double PI=  3.141592653589/180.0;
const int MAX_ITER = 256; 

typedef struct mokhtalet
{
    double R , I ; 
} complex;

complex mul(complex m , complex n){
    complex res ; 
    res.R = m.R * n.R - m.I * n.I ; 
    res.I = m.I * n.R + m.R * n.I ; 
    return res ; 
}
complex sum(complex m , complex n){
    complex res = {m.R + n.R , m.I + n.I} ; 
    return res ; 
}

double tav(complex m){
    return sqrt(m.R * m.R + m.I * m.I) ; 
}

double get_mbs_iter(double x, double y)
{
    int res = 0 ;
    complex z = {0 , 0} ;  
    complex c = {x , y} ;
    while(res <= MAX_ITER && tav(z) <= 2)
    {
        z = sum(mul(z,z) , c ) ; 
        res ++ ; 
    }
    if(res == MAX_ITER + 1) return res ; 
    return (double)res - log(log2(tav(z)));
}

void hsv_to_rgb(float hue, float min, float max, COLORINDEX* p)
{
    float r,g,b;
    float mi = min/100.0;
    float ma = max/100.0;
    float k = mi*ma;
    float jj = k*(1-fabs(fmod(hue/60.0, 2)-1));
    float q = ma-k;
    if(hue >= 0 && hue < 60){
        r = k,g = jj,b = 0;
    }
    else if(hue >= 60 && hue < 120){
        r = jj,g = k,b = 0;
    }
    else if(hue >= 120 && hue < 180){
        r = 0,g = k,b = jj;
    }
    else if(hue >= 180 && hue < 240){
        r = 0,g = jj,b = k;
    }
    else if(hue >= 240 && hue < 300){
        r = jj,g = 0,b = k;
    }
    else{
        r = k,g = 0,b = jj;
    }
    int R = (r+q)*255;
    int G = (g+q)*255;
    int B = (b+q)*255; 
    p->b = R ;
    p->b = B ; 
    p->g = G ; 
}

void UpdateImageData(ImageState* state)
{
    for(int x=0; x<state->width; x++){
        for(int y=0; y<state->height; y++)
        {
            double nx = (double)state->minx + (x / (double)(state->width) ) * (double)(state->maxx - state->minx)
             , ny = (double)state->miny + (y / (double)(state->height) ) * (double)(state->maxx - state->miny) ; 
            double rx = nx * cos(PI * state->angle) + ny * sin(PI * state->angle);
            double ry = -nx * sin(PI * state->angle) + ny * cos(PI * state->angle); 
            double iter = get_mbs_iter(rx , ry); 
            if(iter == MAX_ITER + 1)
            state->bmFileData.bmData[y * state->width + x] = 0;
            else 
            state->bmFileData.bmData[y * state->width + x] = iter;
        }
    }  
    for(int i=0; i<256; i++)
    {
        double hue = (int) ((i / 255)/360);
        hue = (double)(1-i*2)/255.0 * 360.0 ;  
        hsv_to_rgb(hue, 65, 80, &(state->bmFileData.bmHeader.colorIdx[i])); 
    }
    state->bmFileData.bmHeader.bmInfoHeader.biClrImportant = 150;
    state->bmFileData.bmHeader.bmInfoHeader.biClrUsed = 150;
    state->bmFileData.bmHeader.colorIdx[0].r = 255 ; 
    state->bmFileData.bmHeader.colorIdx[0].g = 11 ;
    state->bmFileData.bmHeader.colorIdx[0].b = 97 ;
    state->bmFileData.bmHeader.colorIdx[1].g = 110 ;
    state->bmFileData.bmHeader.colorIdx[1].r = 160 ;
}

void ChangeCenter(ImageState* state, double newcx, double newcy, int steps)
{
    double xst = (newcx - state->cx) / (double)(steps) ; 
    double yst = (newcy - state->cy) / (double)(steps) ; 
    for(int i=0;i<steps; i++)
    {
        state->cx += xst ; 
        state->cy += yst ; 
        state->minx += xst ; 
        state->miny += yst ; 
        state->maxx += xst ; 
        state->maxy += yst; 
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeZoom(ImageState* state, double zoom, int steps)
{
    double zoo = pow(zoom , 1./steps) ;
    for(int i=0; i<steps; i++)
    {
        state->minx = state->minx / zoo ;  
        state->miny = state->miny / zoo ; 
        state->maxx = state->maxx / zoo ; 
        state->maxy = state->maxy / zoo ; 
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeRotation(ImageState* state, double angle, int steps)
{
    double angle_step = angle / steps;
    for(int i=0; i<steps; i++)
    {
        state->angle += angle_step;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void Hold(ImageState* state, int steps)
{
    for(int i=0; i<steps; i++)
    {
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

#endif