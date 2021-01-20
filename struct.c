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
    if (d>0){
        unit_vect->x = vect.x / d;
        unit_vect->y = vect.y / d;
    }else{
        unit_vect->x = 0;
        unit_vect->y = 0;
    }
}


float GetDistance(Coord pos1, Coord pos2){
    return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
}

void ProjVect(Coord* proj_vect, Coord projected_vect, Coord ref_vect){
    float uv = ScalProdVect(projected_vect, ref_vect);
    float v2 = pow(VectResult(ref_vect), 2);

    ScalProdFloat(proj_vect, ref_vect, uv / v2);
}

void PrintVect(char title[], Coord vector){
    printf("%s = (%f, %f)\n", title, vector.x, vector.y);
}

float Degres(float radians){
    return radians * (180.0 / M_PI);
}

float Radians(float degres){
    return degres / (180.0 / M_PI);
}

float Signe(float value){
    if (value < 0.0){
        return -1.0;}
    else{
        return 1.0;}
}
