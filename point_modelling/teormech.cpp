#define LEN_JOHN_MAX_DIST 1
#include "visualise_system.h"

int main()
{
    universe* un = make_new_universe(4, 3);
    un -> do_common = 0;
    un -> do_len_joh = 1;
    un -> ln_j_k = 10;

    // опорная точка 1
    un -> points[0] = new_point();
    un -> points[0] -> radius = 0.03;
    un -> points[0] -> mass = 1e10;
    un -> points[0] -> rad_vect = new_vector(0, 0, 0, TYPE_RAD);
    un -> points[0] -> vel_vect = new_vector(0, 0, 0, TYPE_VEL);

    // опорная точка 2
    un -> points[1] = new_point();
    un -> points[1] -> radius = 0.03;
    un -> points[1] -> mass = 1e10;
    un -> points[1] -> rad_vect = new_vector(3, 0, 0, TYPE_RAD);
    un -> points[1] -> vel_vect = new_vector(0, 0, 0, TYPE_VEL);

    //груз 1
    un -> points[2] = new_point();
    un -> points[2] -> radius = 0.1;
    un -> points[2] -> mass = 1;
    un -> points[2] -> rad_vect = new_vector(1, 0, 0, TYPE_RAD);
    un -> points[2] -> vel_vect = new_vector(0, 0, 0, TYPE_VEL);

    //груз 2
    un -> points[3] = new_point();
    un -> points[3] -> radius = 0.1;
    un -> points[3] -> mass = 1;
    un -> points[3] -> rad_vect = new_vector(2, 0, 0, TYPE_RAD);
    un -> points[3] -> vel_vect = new_vector(0, 0, 0, TYPE_VEL);

    // пружина 1
    un -> opt_interacts[0] = new_interact(0, 1, 0);
    un -> opt_interacts[0] -> point1 = un -> points[0];
    un -> opt_interacts[0] -> point2 = un -> points[2];

    // пружина 2
    un -> opt_interacts[1] = new_interact(0, 1, 0);
    un -> opt_interacts[1] -> point1 = un -> points[2];
    un -> opt_interacts[1] -> point2 = un -> points[3];

    // пружина 3
    un -> opt_interacts[2] = new_interact(0, 1, 0);
    un -> opt_interacts[2] -> point1 = un -> points[3];
    un -> opt_interacts[2] -> point2 = un -> points[1];


    double A = 0.1;
    double omega = 1.1;

    _txWindowStyle &= ~WS_CAPTION; // FullScreen: окно без заголовка, размером с экран
    txCreateWindow (GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));

    //txCreateWindow(1300, 650);
    //printf("%lg\n", txQueryPerformance());

    txSetColor(TX_GREEN, 2);
    txSetFillColor(TX_BLACK);
    txSelectFont ("Courier New", 20);


    double dt = 0.000001;
    calc_group_energy(un);
    double energy0 = un -> mec_energy;
    char timestr[100];
    vid_sys_info conf = apply_autoscale(un);//set_video_conf(400, 300, 10);
    double time = 0, pretime = 0, autoscale_time = 0;

    while(time <= 20000)
    {
        time = cur_time();

        // колеблем координаты опорных точек
        un -> points[0] -> rad_vect = new_vector(A * sin(omega * (un -> time)) , 0, 0, TYPE_RAD);
        un -> points[1] -> rad_vect = new_vector(3 + A * sin(omega * (un -> time)) , 0, 0, TYPE_RAD);

        calc_group_interactions(un);
        make_timestep(un, dt);

        if(time - autoscale_time > 5)
        {
            //conf = apply_autoscale(un);

            autoscale_time = time;
            //printf("autoscale\n");
        }

        if(time - pretime > 0.02)
        {
            txBegin();
            redraw_universe(un, conf);
            calc_group_energy(un);
            sprintf(timestr, "время в системе: %f секунд, в нашей %f, общая энергия %f\n", un -> time, time, un -> mec_energy);
            txSetColor(TX_ORANGE, 1);
            txTextOut(0, 0, (const char*) timestr);
            txEnd();
            pretime = time;
        }
    }
    return 0;
}
