#ifndef SPRING_H
#define SPRING_H

#include "point.h"

#define RIGID_STIFFNESS 20.0
#define SOFT_STIFFNESS 20.0
#define RIGID_DAMPING 3.0
#define SOFT_DAMPING 0.0


spring** create_springs(point** points,int rows,int columns,int* nb_springs);

void update_spring(spring* s);

void delete_springs(spring** springs,int nb_springs);

int get_nb_sticks(int rows,int columns);


#endif