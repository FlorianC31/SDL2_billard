#include "struct.h"

float VectResult(Coord vector){
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));

}

float ScalProdVect(Coord vect1, Coord vect2){
    return vect1.x * vect2.x + vect1.y * vect2.y;
}

void ScalProdFloat(Coord* dest_vect, Coord src_vect, float a){
    dest_vect->x = src_vect.x * a;
    dest_vect->y = src_vect.y * a;
}

void UnitVect(Coord* unit_vect, Coord vect){
    float d = VectResult(vect);
    unit_vect->x = vect.x / d;
    unit_vect->y = vect.y / d;
}


void ProjVect(Coord* proj_vect, Coord projected_vect, Coord ref_vect){
    float uv = ScalProdVect(projected_vect, ref_vect);
    float v2 = pow(VectResult(ref_vect), 2);

    ScalProdFloat(proj_vect, ref_vect, uv / v2);
}

void PrintVect(char title[], Coord vector){
    printf("%s = (%f, %f)\n", title, vector.x, vector.y);
}
