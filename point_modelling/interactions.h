#include "ph_point.h"

//PHYSICAL CONSTS

#define GRAV_CONST 6.67e-11

#define QULON_CONST 9e9

//

void grav_interact(ph_point* p1, ph_point* p2)
{
    if(p1 && p2)
    {
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        ph_vector f;
        double rabs = vector_abs(r);
        if(rabs == 0) return;

        if(rabs > p1 -> radius + p2 -> radius)
            f = mul_to_num(r, GRAV_CONST * (p1 -> mass) * (p2 -> mass)/(pow(rabs, 3)), TYPE_FORCE);
        else
            f = new_vector(0, 0, 0, TYPE_FORCE);

        p2 -> force_vect = vector_add(p2 -> force_vect, f);
        p1 -> force_vect = vector_sub(p1 -> force_vect, f);
    }
}

void common_interact(ph_point* p1, ph_point* p2)
{
    if(p1 && p2)
    {
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        ph_vector f;
        double rabs = vector_abs(r);
        if(rabs == 0) return;

        if(rabs > p1 -> radius + p2 -> radius)
        {
            f = mul_to_num(r, (QULON_CONST * (p1 -> charge) * (p2 -> charge) - GRAV_CONST * (p1 -> mass) * (p2 -> mass))
                           /(pow(rabs, 3)), TYPE_FORCE);
            p2 -> force_vect = vector_sub(p2 -> force_vect, f);
            p1 -> force_vect = vector_add(p1 -> force_vect, f);
        }
    }
}

double common_energy(ph_point* p1, ph_point* p2)
{
    double energy = 0;
    if(p1 && p2)
    {
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        double rabs = vector_abs(r);
        if(rabs == 0) return 0;

        if(rabs > p1 -> radius + p2 -> radius)
        {
            energy = (-QULON_CONST * (p1 -> charge) * (p2 -> charge) - GRAV_CONST * (p1 -> mass) * (p2 -> mass)) / rabs;
        }
    }
    return energy;
}

void qulon_interact(ph_point* p1, ph_point* p2)
{
    if(p1 && p2)
    {
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        ph_vector f;
        double rabs = vector_abs(r);
        if(rabs == 0) return;

        if(rabs > p1 -> radius + p2 -> radius)
            f = mul_to_num(r, QULON_CONST * (p1 -> charge) * (p2 -> charge)/(pow(rabs, 3)), TYPE_FORCE);
        else
            f = new_vector(0, 0, 0, TYPE_FORCE);

        p2 -> force_vect = vector_sub(p2 -> force_vect, f);
        p1 -> force_vect = vector_add(p1 -> force_vect, f);
    }
}


void elastic_interact(ph_point* p1, ph_point* p2, double r_targ, double k)
{
    if(p1 && p2)
    {
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        ph_vector f;
        double rabs = vector_abs(r);
        if(rabs == 0) return;

        f = mul_to_num(r, (r_targ - vector_abs(r)) * k / rabs, TYPE_FORCE);

        p2 -> force_vect = vector_sub(p2 -> force_vect, f);
        p1 -> force_vect = vector_add(p1 -> force_vect, f);
    }
}

void viscous_interact(ph_point* p1, ph_point* p2, double k)
{
    if(p1 && p2)
    {
        ph_vector v = vector_sub(p1 -> vel_vect, p2 -> vel_vect);
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        ph_vector f;

        double rabs = vector_abs(r);
        if(rabs == 0) return;

        f = mul_to_num(r, scalar_mul(r, v)*k / pow(rabs, 2), TYPE_FORCE);

        p2 -> force_vect = vector_add(p2 -> force_vect, f);
        p1 -> force_vect = vector_sub(p1 -> force_vect, f);
    }
}

void lnk_interact(ph_point* p1, ph_point* p2, double r_targ, double k, double v_k)
{
    if(p1 && p2)
    {
        ph_vector v = vector_sub(p1 -> vel_vect, p2 -> vel_vect);
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        ph_vector f;
        double rabs = vector_abs(r);
        if(rabs == 0) return;
        f = mul_to_num(r, (scalar_mul(r, v) * v_k - (r_targ - rabs) * k * vector_abs(r))  / pow(rabs, 2), TYPE_FORCE);

        p2 -> force_vect = vector_add(p2 -> force_vect, f);
        p1 -> force_vect = vector_sub(p1 -> force_vect, f);
    }
}

double lnk_energy(ph_point* p1, ph_point* p2, double r_targ, double k, double v_k)
{
    double energy = 0;
    if(p1 && p2)
    {
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        double rabs = vector_abs(r);
        if(rabs == 0) return 0;
        energy = pow(r_targ - rabs, 2) * k * 0.5;
    }
    return energy;
}

#ifndef LEN_JOHN_MAX_DIST
#define LEN_JOHN_MAX_DIST 3
#endif

void len_john_interact(ph_point* p1, ph_point* p2, double k)
{
    if(p1 && p2 && !(p1 == p2))
    {
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        ph_vector f;
        double rabs = vector_abs(r);
        if(rabs == 0) return;
        double rmin = (p1 -> radius) + (p2 -> radius);
        double rel = rmin/rabs;
        if(rabs < LEN_JOHN_MAX_DIST * rmin)
        {
            f = mul_to_num(r, k * (pow(rel, 8) - pow(rel, 14)), TYPE_FORCE);

            p2 -> force_vect = vector_add(p2 -> force_vect, f);
            p1 -> force_vect = vector_sub(p1 -> force_vect, f);
        }
    }
}
double len_john_energy(ph_point* p1, ph_point* p2, double k)
{
    if(p1 && p2 && !(p1 == p2))
    {
        ph_vector r = vector_sub(p1 -> rad_vect, p2 -> rad_vect);
        double energy = 0;
        double rabs = vector_abs(r);
        if(rabs == 0) return 0;
        double rmin = (p1 -> radius) + (p2 -> radius);
        double rel = rmin/rabs;
        if(rabs < LEN_JOHN_MAX_DIST * rmin)
        {
            energy = k * (pow(rel, 6)/-6 - pow(rel, 12)/-12);
        }
        return energy;
    }
    return 0;
}


