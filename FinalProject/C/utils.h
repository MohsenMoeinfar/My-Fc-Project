#ifndef __UTILS_H__
#define __UTILS_H__
#include"defs.h"
#include"image_utils.h"
#include<stdlib.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<string.h>
#include<stdbool.h>

inline double to_degrees(double radians) {
    return radians * (180.0 / M_PI);
}

inline double to_radian(double degree) {
    return degree * (M_PI/180.0);
}

void ProcessArgs(int argc, char** argv, Config* outcfg, ImageState* state)
{
    FILE* fp = fopen(argv[1], "r");
    fscanf(fp, "%d%*c%d ", &(outcfg->width), &(outcfg->height));
    printf("config file image resolution: %d * %d", outcfg->width, outcfg->height);
    fscanf(fp, "%lf%*c%lf%*c%lf%*c%lf", 
        &(state->minx),
        &(state->maxx),
        &(state->miny),
        &(state->maxy)
    );
    state->cx = (state->minx + state->maxx)/2.0 ; 
    state->cy = (state->miny + state->maxy)/2.0 ;
    state->height = outcfg->height ; 
    state->width = outcfg->width ; 
    Cmd Commands[100];
    int cmdno = 0;
    while(true)
    {
        char cmdname[10];
        int ret = 0 ; 
        ret = fscanf(fp , "%s ", cmdname); 
        if (ret <= 0)
            break;
        if ( strcmp(cmdname, "zoom") == 0 )
        {
            Commands[cmdno].cmd = ct_Zoom;
            fscanf(fp , "%lf%*c%d ", &(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "rotate") == 0)
        {
            Commands[cmdno].cmd = ct_Rotate;
            fscanf(fp , "%lf%*c%d ", &(Commands[cmdno].angle), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "center") == 0)
        {
            Commands[cmdno].cmd = ct_Center;
            fscanf(fp , "%lf%*c%lf%*c%d ", &(Commands[cmdno].cx),  &(Commands[cmdno].cy), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "hold") == 0)
        {
            Commands[cmdno].cmd = ct_Hold;
            fscanf(fp , "%d ", &(Commands[cmdno].steps));
        }
        cmdno++;
    }
    outcfg->CmdCount = cmdno;
    outcfg->Commands = (Cmd*) malloc(sizeof(Cmd) * cmdno);
    for(int i=0; i<cmdno; i++)
    {
        outcfg->Commands[i] = Commands[i]; 
    }   
    InitImage(& state->bmFileData, state->height, state->width);
}
#endif