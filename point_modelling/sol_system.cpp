#include "visualise_system.h"

int main()
{
    universe* un = make_new_universe(10, 0);
    un -> do_common = 1;
    un -> points[0] = new_point();
    un -> points[0] -> name = "СОЛНЫШКО^^ :3";
    un -> points[0] -> type = SOL;
    un -> points[0] -> radius = 0.696e9;
    un -> points[0] -> mass = 1.9885e30;
    un -> points[0] -> charge = 0;
    un -> points[0] -> rad_vect = new_vector(0, 0, 0, TYPE_RAD);
    un -> points[0] -> vel_vect = new_vector(1e4, 0, 0, TYPE_VEL);


    un -> points[1] = new_point();
    un -> points[1] -> name = "МЕРКУРИЙ";
    un -> points[1] -> radius = 2.4e6;
    un -> points[1] -> mass = 3.33e23;
    un -> points[1] -> charge = 0;
    un -> points[1] -> rad_vect = new_vector(-0.5791e11, 0, 0, TYPE_RAD);
    un -> points[1] -> vel_vect = new_vector(0, -47360, 0, TYPE_VEL);

    un -> points[2] = new_point();
    un -> points[2] -> name = "ВЕНЕРА";
    un -> points[2] -> radius = 6.052e6;
    un -> points[2] -> mass = 4.8675e24;
    un -> points[2] -> charge = 0;
    un -> points[2] -> rad_vect = new_vector(1.082e11, 0, 0, TYPE_RAD);
    un -> points[2] -> vel_vect = new_vector(0, 35020, 0, TYPE_VEL);

    un -> points[3] = new_point();
    un -> points[3] -> name = "ЗЕМЕЛЯ";
    //un -> points[3] -> type = 228;
    un -> points[3] -> radius = 6.4e6;
    un -> points[3] -> mass = 5.97e24;
    un -> points[3] -> charge = 0;
    un -> points[3] -> rad_vect = new_vector(-1.496e11, 0, 0, TYPE_RAD);
    un -> points[3] -> vel_vect = new_vector(-0, -29500, 0, TYPE_VEL);

    un -> points[4] = new_point();
    un -> points[4] -> name = "МАРС";
    un -> points[4] -> radius = 3.3e6;
    un -> points[4] -> mass = 6.4e23;
    un -> points[4] -> charge = 0;
    un -> points[4] -> rad_vect = new_vector(2.2794382e11, 0, 0, TYPE_RAD);
    un -> points[4] -> vel_vect = new_vector(0, 24130, 0, TYPE_VEL);

    un -> points[5] = new_point();
    un -> points[5] -> name = "ЮПИТЕР";
    un -> points[5] -> radius = 69.911e6;
    un -> points[5] -> mass = 1.8986e27;
    un -> points[5] -> charge = 0;
    un -> points[5] -> rad_vect = new_vector(-7.405736e11, 0, 0, TYPE_RAD);
    un -> points[5] -> vel_vect = new_vector(0, -13070, 0, TYPE_VEL);

    un -> points[6] = new_point();
    un -> points[6] -> name = "САТУРН";
    un -> points[6] -> radius = 58e6;
    un -> points[6] -> mass = 5.6846e26;
    un -> points[6] -> charge = 0;
    un -> points[6] -> rad_vect = new_vector(1.429e12, 0, 0, TYPE_RAD);
    un -> points[6] -> vel_vect = new_vector(0, 9690, 0, TYPE_VEL);

    un -> points[7] = new_point();
    un -> points[7] -> name = "УРАН";
    un -> points[7] -> radius = 25e6;
    un -> points[7] -> mass = 8.68e25;
    un -> points[7] -> charge = 0;
    un -> points[7] -> rad_vect = new_vector(-2.871e12, 0, 0, TYPE_RAD);
    un -> points[7] -> vel_vect = new_vector(0, -6810, 0, TYPE_VEL);

    un -> points[8] = new_point();
    un -> points[8] -> name = "НЕПТУН";
    un -> points[8] -> radius = 24.622e6;
    un -> points[8] -> mass = 1.0243e26;
    un -> points[8] -> charge = 0;
    un -> points[8] -> rad_vect = new_vector(4.498e12, 0, 0, TYPE_RAD);
    un -> points[8] -> vel_vect = new_vector(0, 5439, 0, TYPE_VEL);

    un -> points[9] = new_point();
    un -> points[9] -> name = "ПЛУТОН";
    un -> points[9] -> radius = 1.1e6;
    un -> points[9] -> mass = 6.4e23;
    un -> points[9] -> charge = 0;
    un -> points[9] -> rad_vect = new_vector(-7.5e12, 0, 0, TYPE_RAD);
    un -> points[9] -> vel_vect = new_vector(-0, -4669, 0, TYPE_VEL);




    _txWindowStyle &= ~WS_CAPTION; // FullScreen: окно без заголовка, размером с экран
    txCreateWindow (GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));

    txSetColor(TX_GREEN, 2);
    txSetFillColor(TX_BLACK);
    txSelectFont ("Courier New", 20);


    double dt = 100;
    char timestr[100];
    vid_sys_info conf = apply_autoscale(un);//set_video_conf(400, 300, 10);
    double time = 0, pretime = 0, autoscale_time = 0;
    conf = apply_autoscale(un);

    while(time <= 20000)
    {
        time = cur_time();
        //printf("no error\n");
        calc_group_interactions(un);
        //printf("no error\n");
        make_timestep(un, dt);

        if(time - autoscale_time > 5)
        {
            conf = apply_autoscale(un);
            autoscale_time = time;
            //printf("autoscale\n");
        }

        if(time - pretime > 0.02)
        {   txBegin();
            redraw_universe(un, conf);
            sprintf(timestr, "время в этой мать ее вселенной : %f гребаных секунд или %f лет а в нашей %f\n", un -> time,  un -> time / 31415965, time);
            txSetColor(TX_ORANGE, 1);
            txTextOut(0, 0, (const char*) timestr);
            txEnd();
            pretime = time;
        }
    }
    return 0;
}
