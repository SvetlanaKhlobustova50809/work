#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <string>

/**** лучше не трогать ****/
using std::cerr;
using std::endl;
const int m_rows = 200;
const int m_cols = 200;
const int terrain_size = 2*2*2*2*2*2*2*2+1; // terrain_size > min(rows,cols)  
float map[terrain_size][terrain_size];
float Sea[terrain_size][terrain_size];

/***** можно менять *****/
const float roughness = 0.3; // шероховатость 
const int map_height = 85; // максимальная высота ландшафта
const std::string pick_sky = "4";  // 1-4 выбор skybox'a
bool isRandom = true; // генерировать рандомный ландшафт?
bool isMedian = false; // использовать медианный фильтр на ландшафте?
const int frame = 3; // перерисовывать волны каждый 'frame' кадр



#endif
