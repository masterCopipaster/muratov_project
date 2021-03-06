/**
this is vector structure and functions
*/

#ifndef _VECT_H
#define _VECT_H

#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum vector_types
{
    TYPE_ALL = 0,
    TYPE_RAD,
    TYPE_VEL,
    TYPE_ACCEL,
    TYPE_IMP,
    TYPE_FORCE,
    TYPE_M_FORCE,
    TYPE_M_IMP
};

struct ph_vector
{
    double e1;
    double e2;
    double e3;
    int type;
};

ph_vector new_vector(double e1, double e2, double e3, int type)
{
    ph_vector res;
    res.e1 = e1;
    res.e2 = e2;
    res.e3 = e3;
    res.type = type;
    return res;
}

ph_vector vector_add(ph_vector l, ph_vector r)
{
    assert(l.type == r.type);
    ph_vector res;
    res.e1 = l.e1 + r.e1;
    res.e2 = l.e2 + r.e2;
    res.e3 = l.e3 + r.e3;
    res.type = l.type;
    return res;
}

ph_vector vector_sub(ph_vector l, ph_vector r)
{
    assert(l.type == r.type);
    ph_vector res;
    res.e1 = l.e1 - r.e1;
    res.e2 = l.e2 - r.e2;
    res.e3 = l.e3 - r.e3;
    res.type = l.type;
    return res;
}
ph_vector negative(ph_vector vec)
{
    ph_vector res;
    res.e1 = - vec.e1;
    res.e2 = - vec.e2;
    res.e3 = - vec.e3;
    res.type = vec.type;
    return res;
}

ph_vector mul_to_num(ph_vector vect, double num, int type)
{
    ph_vector res;
    res.e1 = vect.e1 * num;
    res.e2 = vect.e2 * num;
    res.e3 = vect.e3 * num;
    res.type = type;
    return res;
}



double scalar_mul(ph_vector l, ph_vector r)
{
    return (l.e1 * r.e1) + (l.e2 * r.e2) + (l.e3 * r.e3);
}

double vector_abs(ph_vector vect)
{
    return sqrt(scalar_mul(vect, vect));
}

char* print_type(ph_vector vect)
{
    if(vect.type == TYPE_ALL) return "ALL";
    else if(vect.type == TYPE_RAD) return "RADIUS";
    else if(vect.type == TYPE_VEL) return "VELOCITY";
    else if(vect.type == TYPE_ACCEL) return "ACCELERATION";
    else if(vect.type == TYPE_IMP) return "IMPULSE";
    else if(vect.type == TYPE_IMP) return "IMPULSE";
    else if(vect.type == TYPE_FORCE) return "FORCE";
    else if(vect.type == TYPE_M_IMP) return "IMPULSE MOMENT";
    else if(vect.type == TYPE_M_FORCE) return "FORCE MOMENT";
    else return "UNKNOWN TYPE";
}

char* str_output(ph_vector vect)
{
    char buf[100] = {};
    sprintf(buf, "(%lg, %lg, %lg) abs: %lg, type: %s", vect.e1, vect.e2, vect.e3, vector_abs(vect), print_type(vect));
    char* res = (char*) calloc(strlen(buf) + 1, sizeof(char));
    strcpy(res, buf);
    return res;
}

#endif









