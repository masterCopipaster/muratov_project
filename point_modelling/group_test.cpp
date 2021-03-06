#define LEN_JOHN_MAX_DIST 1
#include "visualise_system.h"

int main()
{
    universe* un = make_new_universe(76, 40);
    un -> do_common = 0;
    un -> do_len_joh = 1;
    un -> ln_j_k = 10;
    srand(clock());
    for(int i = 0; i < 36; i++)
    {
        un -> points[i] = new_point();
        un -> points[i] -> radius = 0.55;
        un -> points[i] -> mass = 1;
        un -> points[i] -> charge = 2e-7 * ((i % 2) ? 1 : -1);
        un -> points[i] -> rad_vect = new_vector(-1.5 + 0.6 * (double)(i / 6), -1.5 + 0.6 * (double)(i % 6), 0, TYPE_RAD);
        un -> points[i] -> vel_vect = new_vector(0, 0, 0, TYPE_VEL);
        //un -> points[i] -> vel_vect = new_vector((double)(rand() - 16200)/1000, (double)(rand() - 16200)/1000, 0, TYPE_VEL);
    }
    un -> points[10] -> type = SOL;
    un -> points[10] -> mass = 1;

    for(int i = 0; i < 20; i++)
    {
        un -> points[i + 36] = new_point();
        un -> points[i + 36] -> radius = 0.5;
        un -> points[i + 36] -> mass = 100;
        un -> points[i + 36] -> charge = 0;//2e-7 * ((i % 2) ? 1 : -1);
        un -> points[i + 36] -> rad_vect = new_vector(-4.3 + 0.5 * (double)abs(i - 10), -0.5 * (double)i + 5, 0, TYPE_RAD);
        un -> points[i + 36] -> vel_vect = new_vector(0, 0, 0, TYPE_VEL);

        un -> points[75 - i] = new_point();
        un -> points[75 - i] -> radius = 0.5;
        un -> points[75 - i] -> mass = 100;
        un -> points[75 - i] -> charge = 0;//2e-7 * ((i % 2) ? 1 : -1);
        un -> points[75 - i] -> rad_vect = new_vector(6 - 0.5 * (double)abs(i - 9), -0.5 * (double)i + 5, 0, TYPE_RAD);
        un -> points[75 - i] -> vel_vect = new_vector(0, 0, 0, TYPE_VEL);


    }

    for(int i = 0; i < 40; i++)
    {
        un -> opt_interacts[i] = new_interact(0, 2000, 10);
        un -> opt_interacts[i] -> point1 = un -> points[i + 35];
        un -> opt_interacts[i] -> point2 = un -> points[i + 36];
    }
    un -> opt_interacts[0] -> point1 = un -> points[75];
    un -> opt_interacts[0] -> elast_targ = 0;
    //un -> opt_interacts[30] -> point1 = un -> points[95];




    _txWindowStyle &= ~WS_CAPTION; // FullScreen: ���� ��� ���������, �������� � �����
    txCreateWindow (GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));

    //txCreateWindow(1300, 650);
    //printf("%lg\n", txQueryPerformance());

    txSetColor(TX_GREEN, 2);
    txSetFillColor(TX_BLACK);
    txSelectFont ("Courier New", 20);


    double dt = 0.0001;
    calc_group_energy(un);
    double energy0 = un -> mec_energy;
    char timestr[100];
    vid_sys_info conf = apply_autoscale(un);//set_video_conf(400, 300, 10);
    double time = 0, pretime = 0, autoscale_time = 0;

    while(time <= 20000)
    {
        time = cur_time();
        //printf("no error\n");
        calc_group_interactions(un);
        //printf("no error\n");
        make_timestep(un, dt);

        if(time - autoscale_time > 5)
        {
            //conf = apply_autoscale(un);

            autoscale_time = time;
            //printf("autoscale\n");
        }

        if(time - pretime > 0.04)
        {
            txBegin();
            redraw_universe(un, conf);
            calc_group_energy(un);
            sprintf(timestr, "����� � �������: %f ������, � ����� %f, ����� ������� %f\n", un -> time, time, un -> mec_energy);
            txSetColor(TX_ORANGE, 1);
            txTextOut(0, 0, (const char*) timestr);
            txEnd();
            txSleep(0.5);
            pretime = time;
        }
    }
    return 0;
}
