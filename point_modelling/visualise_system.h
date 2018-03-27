#define NDEBUG
#include "group_driver.h"
#include "TXLib.h"
#include <time.h>

#define SOL 228;

double cur_time()
{
    return (double)clock() / (double)CLOCKS_PER_SEC;
}

struct vid_sys_info
{
    double center_x;
    double center_y;
    double scale;
    double max_x, min_x, max_y, min_y;
};


void draw_point(ph_point* point, vid_sys_info conf)
{
    if(point)
    {
        txSetColor(TX_YELLOW, 1);
        txSetFillColor(TX_BLACK);
        txCircle(conf.center_x + point -> rad_vect. e1 * conf.scale, conf.center_y + point -> rad_vect. e2 * conf.scale, 5);

        txSetColor(TX_GREEN, 2);
        txSetFillColor(TX_CYAN);
        txCircle(conf.center_x + point -> rad_vect. e1 * conf.scale, conf.center_y + point -> rad_vect. e2 * conf.scale, point -> radius * conf.scale + 1);
        if(point -> name)
        {
            txSetColor(TX_WHITE, 2);
            txTextOut(conf.center_x + (point -> rad_vect.e1 + 4 * point -> radius)* conf.scale, conf.center_y + (point -> rad_vect.e2 + 4 * point -> radius) * conf.scale, (const char*) point -> name);
        }
        if(point -> type == 228)
        {
            txSetColor(TX_ORANGE, 2);
            txSetFillColor(TX_YELLOW);

            txLine(conf.center_x + (point -> rad_vect.e1 + 5.65 * point -> radius)* conf.scale, conf.center_y + point -> rad_vect. e2 * conf.scale,
                   conf.center_x + (point -> rad_vect.e1 - 5.65 * point -> radius)* conf.scale, conf.center_y + point -> rad_vect. e2 * conf.scale);

            txLine(conf.center_x + point -> rad_vect.e1 * conf.scale, conf.center_y + (point -> rad_vect.e2 + 5.65 * point -> radius) * conf.scale,
                   conf.center_x + point -> rad_vect.e1 * conf.scale, conf.center_y + (point -> rad_vect.e2 - 5.65 * point -> radius) * conf.scale);

            txLine(conf.center_x + (point -> rad_vect.e1 + 4 * point -> radius)* conf.scale, conf.center_y + (point -> rad_vect.e2 + 4 * point -> radius) * conf.scale,
                   conf.center_x + (point -> rad_vect.e1 - 4 * point -> radius)* conf.scale, conf.center_y + (point -> rad_vect.e2 - 4 * point -> radius) * conf.scale);

            txLine(conf.center_x + (point -> rad_vect.e1 + 4 * point -> radius)* conf.scale, conf.center_y + (point -> rad_vect.e2 - 4 * point -> radius) * conf.scale,
                   conf.center_x + (point -> rad_vect.e1 - 4 * point -> radius)* conf.scale, conf.center_y + (point -> rad_vect.e2 + 4 * point -> radius) * conf.scale);

            txCircle(conf.center_x + point -> rad_vect. e1 * conf.scale, conf.center_y + point -> rad_vect. e2 * conf.scale, 2 * point -> radius * conf.scale + 1);
        }



    }
}



void draw_interact(opt_interact* inter, vid_sys_info conf)
{
    if(inter)
    {
        txSetColor(TX_RED, 1);
        txLine(conf.center_x + inter -> point1 -> rad_vect. e1 * conf.scale, conf.center_y + inter -> point1 -> rad_vect. e2 * conf.scale,
               conf.center_x + inter -> point2 -> rad_vect. e1 * conf.scale, conf.center_y + inter -> point2 -> rad_vect. e2 * conf.scale);
    }
}


void draw_scale_lines(vid_sys_info conf)
{
    POINT size = txGetExtent();

    txSetColor(TX_GREEN, 1);
    txLine(conf.center_x + conf.max_x * conf.scale, 40, conf.center_x + conf.max_x * conf.scale, size.y);
    txLine(conf.center_x + conf.min_x * conf.scale, 40, conf.center_x + conf.min_x * conf.scale, size.y);
    txLine(0, conf.center_y + conf.max_y * conf.scale, size.x, conf.center_y + conf.max_y * conf.scale);
    txLine(0, conf.center_y + conf.min_y * conf.scale, size.x, conf.center_y + conf.min_y * conf.scale);

    char buf[100] = {};

    sprintf(buf, "%lg", conf.max_x);
    txTextOut(conf.center_x + conf.max_x * conf.scale, 40, (const char*) buf);
    sprintf(buf, "%lg", conf.min_x);
    txTextOut(conf.center_x + conf.min_x * conf.scale, 40, (const char*) buf);

    sprintf(buf, "%lg", conf.max_y);
    txTextOut(5, conf.center_y + conf.max_y * conf.scale, (const char*) buf);
    sprintf(buf, "%lg", conf.min_y);
    txTextOut(5, conf.center_y + conf.min_y * conf.scale, (const char*) buf);


    txSetColor(TX_BLUE, 5);
    txLine(1, 40, size.x - 1, 40);
    txLine(1, 40, 1, size.y - 1);
    txLine(size.x - 3, 40, size.x - 3, size.y - 1);
    txLine(0, size.y - 2, size.x, size.y - 2);
}

void redraw_universe(universe* un, vid_sys_info conf)
{
    txSetFillColor(TX_BLACK);
    txClear();
    draw_scale_lines(conf);
    for(int i = 0; i < un -> point_num; i++)
    {
        draw_point(un -> points[i], conf);
    }

    for(int i = 0; i < un -> opt_int_num; i++)
    {
        draw_interact(un -> opt_interacts[i], conf);
    }
}



vid_sys_info set_video_conf(double center_x, double center_y, double scale)
{
    vid_sys_info res;
    res.center_x = center_x;
    res.center_y = center_y;
    res.scale = scale;
    return res;
}

#define min_two(a, b) ((a < b) ? a : b)

vid_sys_info apply_autoscale(universe* un)
{
    double max_x = -1e300;
    double min_x = 1e300;
    double max_y = -1e300;
    double min_y = 1e300;

    for(int i = 0; i < un -> point_num; i++)
    {
        ph_point* point = un -> points[i];
        if(point)
        {
            if(point -> rad_vect.e1 - point -> radius < min_x)
            {
                min_x = point -> rad_vect.e1;
            }

            if(point -> rad_vect.e1 + point -> radius > max_x)
            {
                max_x = point -> rad_vect.e1;
            }

            if(point -> rad_vect.e2 - point -> radius < min_y)
            {
                min_y = point -> rad_vect.e2 - point -> radius;
            }

            if(point -> rad_vect.e2 + point -> radius > max_y)
            {
                max_y = point -> rad_vect.e2 + point -> radius;
            }
        }
    }

    vid_sys_info res;

    res.max_x = max_x;
    res.max_y = max_y;
    res.min_x = min_x;
    res.min_y = min_y;

    POINT size = txGetExtent();

    res.scale = 0.8* min_two(size.x / (max_x - min_x + 1e-15), size.y / (max_y - min_y + 1e-15));


    res. center_x = (size.x - res.scale * (max_x + min_x))/2;
    res. center_y = (size.y - res.scale * (max_y + min_y))/2;



    return res;
}




