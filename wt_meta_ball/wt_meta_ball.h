
#ifndef _WT_META_BALL_H_
#define _WT_META_BALL_H_


#include <stdio.h>
#include <math.h>
static GLubyte meta_map[256][256][4];
static int map_width = 256;
static int world_width = 0;


static float clamp(float a, float min, float max)
{
    return min(max(a, min), max);
}

static float meta_falloff(float distance, float maxDistance, float scalingFactor)
{
    if (distance <= maxDistance / 3) {
        return scalingFactor * (1 - 3 * distance * distance / (maxDistance * maxDistance));
    }
    else if (distance <= maxDistance) {
        float x = 1 - distance / maxDistance;
        return (3.0f / 2.0f) * scalingFactor * x * x;
    }
    else
        return 0;
}
static void wt_meta_map_init(int width)
{
    memset(meta_map,0,sizeof(meta_map));
    world_width = width;
    //printf("meta_map[10][10] %d\n",meta_map[10][10][0]);
}

static void wt_meta_map_add_cir(int posx,int posy,int r)
{
    float mapx = (float)posx/(float)world_width * (float)map_width;
    float mapy = (float)posy/(float)world_width * (float)map_width;
    float mapr = (float)r / (float)world_width * (float)map_width;
    //printf("%d %d %d \n",posx,posy,r );
    
    int minx = (int)(mapx - mapr) >= 0 ? (int)(mapx - mapr) : 0;
    int maxx = (int)(mapx + mapr) <= map_width ? (int)(mapx + mapr) : 0;
    int miny = (int)(mapy - mapr) >= 0 ? (int)(mapy - mapr) : 0;
    int maxy = (int)(mapy + mapr) <= map_width ? (int)(mapy + mapr) : 0;

    //printf("%d %d %d %d\n",minx,maxx,miny,maxy );
    //printf("%f %f %f  \n",mapx,mapy,mapr);
    for(int x = minx ; x < maxx ; x++) {
        for(int y = miny ; y < maxy ; y++) {
            float alpha = meta_falloff(sqrt((mapx-x)*(mapx-x) + (mapy-y)*(mapy-y)), mapr, 1);
            //meta_map[x][y][0] = 255;
            //meta_map[x][y][1] = 255;
            //meta_map[x][y][2] = 255;
           // int temp = (int)meta_map[y][x][3] + (int)wt_rclamp(alpha * 256 + 0.5f, 0, 255);
            //printf("temp : %d\n", temp);

            if(meta_map[y][x][3] + (GLubyte)clamp(alpha * 256 + 0.5f, 0, 255) > 255) {
                meta_map[y][x][3] = 255;
                //printf("> 255\n");
            }else{
                meta_map[y][x][3] += (GLubyte)clamp(alpha * 256 + 0.5f, 0, 255);
            }

            //meta_map[y][x][3] = temp > 254 ? 254 : (GLubyte)temp;
            // if(meta_map[x][y][3] >= 190 && meta_map[x][y][3] <= 220){
            //     meta_map[x][y][0] = 0;
            //     meta_map[x][y][1] = 255;
            //     meta_map[x][y][2] = 255;
            // }
        }
    }

}

static void wt_meta_map_set_color()
{
    for(int x = 0 ; x < map_width ; x++){
        for(int y = 0 ; y < map_width ; y++){
            if( meta_map[x][y][3] < 190){
                meta_map[x][y][0] = 255;
                meta_map[x][y][1] = 0;
                meta_map[x][y][2] = 0;
            }else{
                meta_map[x][y][0] = 255;
                meta_map[x][y][1] = 255;
                meta_map[x][y][2] = 255;
            }
        }
    }
}

// int wt_meta_map_add_cir2() //自己绘制纹理
// {
//     int len = map_width;
//     //memset(meta_map, 0, sizeof(meta_map));
//     for (int x = 0 ; x < len ; x++)
//     {
//         for (int y = 0 ; y < len ; y++)
//         {

//             meta_map[x][y][0] = 255;
//             meta_map[x][y][1] = 255;
//             meta_map[x][y][2] = 255;
//             float alpha = Falloff(sqrt((x - len / 2) * (x - len / 2) + (y - len / 2) * (y - len / 2)), len / 2, 1);
//             meta_map[x][y][3] = wt_rclamp(alpha * 256 + 0.5f, 0, 255);
//             if(meta_map[x][y][3] >= 190 && meta_map[x][y][3] <= 200){
//                 meta_map[x][y][0] = 0;
//                 meta_map[x][y][1] = 255;
//                 meta_map[x][y][2] = 255;

//             }
//         }
//     }
// }


#endif