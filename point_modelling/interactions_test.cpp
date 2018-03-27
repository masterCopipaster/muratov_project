#include "visualise_system.h"

int main()
{
    ph_point* sol = new_point();
    sol -> mass = 5.97e24;
    sol -> radius = 6.4e2;
    sol -> rad_vect = new_vector(0, 0, 0, TYPE_RAD);
    sol -> vel_vect = new_vector(0, 0, 0, TYPE_VEL);

    ph_point* planet = new_point();
    planet -> mass = 100;
    planet -> radius = 0.01;
    planet -> rad_vect = new_vector(6.401e6, 0, 0, TYPE_RAD);
    planet -> vel_vect = new_vector(0, 8000, 0, TYPE_VEL);

    char* vec_out = 0;
    double time = 0;
    double dt = 0.01;
    FILE* file = fopen("c:/users/Alex/Documents/orbit.txt", "w");

    while(time < 15000)
    {

        point_init(sol);
        point_init(planet);

        grav_interact(sol, planet);
        //elastic_interact(sol, planet, 0, 100);
        //viscous_interact(sol, planet, 0.1);

        calc_movement(sol, dt);
        calc_movement(planet, dt);

        vec_out = str_output(planet -> rad_vect);
        printf("%s time: %lg\n", vec_out, time);
        time += dt;
        fprintf(file, "%lg %lg\n", planet -> rad_vect. e1, planet -> rad_vect. e2);
        free(vec_out);

    }

    printf("%s\n", point_str_printout(planet));

    return 0;
}


