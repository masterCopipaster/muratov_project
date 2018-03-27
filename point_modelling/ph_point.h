#include "ph_vector.h"

struct ph_point
{
    double mass;
    double radius;
    double charge;
    ph_vector rad_vect;
    ph_vector vel_vect;
    ph_vector force_vect;
    ph_vector accel_vect;
    int type;
    char* name;
};

ph_point* new_point()
{
    return (ph_point*) calloc(1, sizeof(ph_point));
}

void calc_movement(ph_point* point, double dt)
{
    if(point)
    {
        point -> accel_vect = mul_to_num(point -> force_vect, 1 / point -> mass, TYPE_ACCEL);
        point -> vel_vect = vector_add(point -> vel_vect, mul_to_num(point -> accel_vect, dt, TYPE_VEL));
        point -> rad_vect = vector_add(point -> rad_vect, mul_to_num(point -> vel_vect, dt, TYPE_RAD));
    }
}
// call this func 1 time in main cycle
void point_init(ph_point* point)
{
    if(point) point -> force_vect = new_vector(0, 0, 0, TYPE_FORCE);
}

char* point_str_printout(ph_point* point)
{
    assert(point);
    if(point)
    {
        char buf[1000] = {};
        sprintf(buf, "type: %d\nmass: %lg\nradius: %lg\ncharge: %lg\nradius vector: %s\nvelocity: %s\nacceleration: %s\nforce: %s", point -> type, point -> mass,
            point -> radius, point -> charge, str_output(point -> rad_vect), str_output(point -> vel_vect),
                 str_output(point -> accel_vect), str_output(point -> force_vect));


        char* res = (char*) calloc(strlen(buf) + 1, sizeof(char));
        strcpy(res, buf);
        return res;
    }
}







