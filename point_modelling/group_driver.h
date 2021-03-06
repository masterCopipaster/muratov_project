#include "interactions.h"


struct opt_interact
{
    ph_point* point1 = 0;
    ph_point* point2 = 0;
    double elast_k = 0;
    double elast_targ = 0;
    double visc_k = 0;
};

opt_interact* new_interact(double elast_targ, double elast_k, double visc_k)
{
    opt_interact* res = (opt_interact*) calloc(1, sizeof(opt_interact));
    res -> elast_targ = elast_targ;
    res -> elast_k = elast_k;
    res -> visc_k = visc_k;
    return res;
}

struct universe
{
    ph_point** points = 0;
    int point_num = 0;
    opt_interact** opt_interacts = 0;
    int opt_int_num = 0;
    double time;
    int do_len_joh = 0;
    int do_common = 1;
    double ln_j_k = 0;
    double mec_energy = 0;
};

universe* make_new_universe(int point_num, int opt_int_num)
{
    universe* res = (universe*) calloc(1, sizeof(universe));
    res -> points = (ph_point**) calloc(point_num, sizeof(ph_point*));
//    ph_point* points_ptr = (ph_point*) calloc(point_num, sizeof(ph_point));
    res -> opt_interacts = (opt_interact**) calloc(opt_int_num, sizeof(opt_interact*));
//    opt_interact* int_ptr = (opt_interact*) calloc(opt_int_num, sizeof(opt_interact));
 /*
    for(int i = 0; i < point_num; i++)
    {
        res -> points[i] = points_ptr + i * sizeof(ph_point);
    }

    for(int i = 0; i < opt_int_num; i++)
    {
        res -> opt_interacts[i] = int_ptr + i * sizeof(opt_interact);
    }
 */
    res -> point_num = point_num;
    res -> opt_int_num = opt_int_num;
    res -> time = 0;
    return res;
}

void calc_group_interactions(universe* un)
{
    int i = 0;
    int j = 0;

    for(i = 0; i < un -> point_num; i++)
    {
        point_init(un -> points[i]);
    }

    for(i = 0; i < un -> point_num; i++)
    {
        for(j = i; j < un -> point_num; j++)
        {
            if(un -> do_common)
            common_interact(un -> points[i], un -> points[j]);
            if(un -> do_len_joh)
            len_john_interact(un -> points[i], un -> points[j], un -> ln_j_k);

        }
    }

    for(i = 0; i < un -> opt_int_num; i++)
    {
        if(un -> opt_interacts[i])
        {
            lnk_interact(un -> opt_interacts[i] -> point1, un -> opt_interacts[i] -> point2, un -> opt_interacts[i] -> elast_targ,
            un -> opt_interacts[i] -> elast_k, un -> opt_interacts[i] -> visc_k);

        }
    }
}

void calc_group_energy(universe* un)
{
    int i = 0;
    int j = 0;
    double energy = 0;

    for(i = 0; i < un -> point_num; i++)
    {
        energy += un -> points[i] -> mass * scalar_mul(un -> points[i] -> vel_vect, un -> points[i] -> vel_vect) / 2;
    }

    for(i = 0; i < un -> point_num; i++)
    {
        for(j = i; j < un -> point_num; j++)
        {
            if(un -> do_common)
            energy += common_energy(un -> points[i], un -> points[j]);
            if(un -> do_len_joh)
            energy += len_john_energy(un -> points[i], un -> points[j], un -> ln_j_k);

        }
    }

    for(i = 0; i < un -> opt_int_num; i++)
    {
        if(un -> opt_interacts[i])
        {
            energy += lnk_energy(un -> opt_interacts[i] -> point1, un -> opt_interacts[i] -> point2, un -> opt_interacts[i] -> elast_targ,
            un -> opt_interacts[i] -> elast_k, un -> opt_interacts[i] -> visc_k);

        }
    }

    un -> mec_energy = energy;
}

void make_timestep(universe* un, double dt)
{
    for(int i = 0; i < un -> point_num; i++)
    {
        calc_movement(un -> points[i], dt);
    }
    un -> time += dt;
}









