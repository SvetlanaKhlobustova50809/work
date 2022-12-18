#ifndef DIAMOND_H
#define DIAMOND_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <assert.h>
#include <climits>
#include <landscape.h>

void square_step(int x, int y, int step);
void diamond_step(int x, int y, int step);
void normalization();
void median(int radius);
void averaging(int radius);
void avg_neighbor(int from_x, int from_y, unsigned int size);
float median_filter(int from_x, int from_y, unsigned int size);


void diamond_square(const int size){

    assert(terrain_size > std::min(m_rows, m_cols));
    if (isRandom)
        srand(time(NULL));
    map[0][0] = 1;
    map[0][terrain_size-1] = -5;
    map[terrain_size-1][0] = 20;
    map[terrain_size-1][terrain_size-1] = 10;

    int step_size = size - 1;

    while(step_size > 1) {

        for(unsigned int x = 0; x < size-1; x += step_size)
            for(unsigned int y = 0; y < size-1; y += step_size)
                square_step(x, y, step_size);

        for(unsigned int x = step_size/2; x < size-1; x += step_size)
            for(unsigned int y = step_size/2; y < size-1; y += step_size)
                diamond_step(x, y, step_size);

        step_size /= 2;
    }
    normalization();
    // averaging(1);
    if (isMedian)
        median(1);
}

void square_step(int x, int y, int step) {

    float r = roughness;
    int half = step / 2;

    float a = map[x][y];
    float b = map[x+step][y];
    float c = map[x][y+step];
    float d = map[x+step][y+step];

    float average = (a + b + c + d) / 4;
    float random_range = -r * half + rand() % int(2*r*half + 1);

    map[x + half][y + half] = average + random_range;
}

void diamond_step(int x, int y, int step) {

    float r = roughness;
    int half = step / 2;
    float a,b,c;
    float random_range = -r * half + rand() % int(2*r*half + 1);
    float average;
    
    a = map[x][y];

    b = map[x-half][y-half];
    c = map[x-half][y+half];
    average = (a + b + c) / 3;
    map[x-half][y] = average + random_range;

    c = map[x+half][y-half];
    average = (a + b + c) / 3;
    random_range = -r * half + rand() % int(2*r*half + 1);
    map[x][y-half] = average + random_range;
    
    b = map[x+half][y+half];
    average = (a + b + c) / 3;
    random_range = -r * half + rand() % int(2*r*half + 1);
    map[x+half][y] = average + random_range;

    c = map[x-half][y+half];
    average = (a + b + c) / 3;
    random_range = -r * half + rand() % int(2*r*half + 1);
    map[x][y+half] = average + random_range;
}

void normalization(){

    float min = std::numeric_limits<float>::max();
    float max = -min;
    for(unsigned int x = 0; x < terrain_size; ++x)
        for(unsigned int y = 0; y < terrain_size; ++y){
            min = map[x][y] < min ? map[x][y] : min;
            max = map[x][y] > max ? map[x][y] : max;
        }
    
    for(unsigned int x = 0; x < terrain_size; ++x)
        for(unsigned int y = 0; y < terrain_size; ++y){
            map[x][y] = (map[x][y] - min) / (max-min);
            map[x][y] = map[x][y]*map[x][y] - 0.2f;
        }

    for(unsigned int x = 0; x < terrain_size; ++x)
        for(unsigned int y = 0; y < terrain_size; ++y){
            if (isMedian && map[x][y] < 0.0f)
                map[x][y] = 0.0f;
            else 
            map[x][y] = map[x][y]*map_height;
        }
}

void averaging(int radius){
    unsigned int size = 2*radius+1;
    for(unsigned int x = radius; x < terrain_size-radius; ++x)
        for(unsigned int y = radius; y < terrain_size-radius; ++y){
            avg_neighbor(x-radius,y-radius,size);
        }
}


void avg_neighbor(int from_x, int from_y, unsigned int size){
    
    unsigned int to_x = from_x+size;
    unsigned int to_y = from_y+size;
    float sum2 = 0.0f;
    for(unsigned int x = from_x; x < to_x; ++x)
        for(unsigned int y = from_y; y < to_y; ++y){
            sum2 += map[x][y]; 
        }
    if (sum2 < 0.001f)
        return;
    float sum = sqrt(sum2);
    for(unsigned int x = from_x; x < to_x; ++x)
        for(unsigned int y = from_y; y < to_y; ++y){
            map[x][y] = map[x][y] / sum;
        }
}

void median(int radius){
    unsigned int size = 2*radius+1;
    for(unsigned int x = radius; x < terrain_size-radius; ++x)
        for(unsigned int y = radius; y < terrain_size-radius; ++y){
            map[x][y] = median_filter(x-radius,y-radius,size);
        }
}

float median_filter(int from_x, int from_y, unsigned int size){
    
    int hist[map_height];
    unsigned int to_x = from_x+size;
    unsigned int to_y = from_y+size;
    for (unsigned int i = 0; i < map_height; ++i) hist[i] = 0;
    
    for(unsigned int x = from_x; x < to_x; ++x)
        for(unsigned int y = from_y; y < to_y; ++y){
            int val = map[x][y]; 
            hist[val]++;
        }
    int sum = 0; int r = 0;
    for(unsigned int i = 0; i < map_height; ++i){
        sum += hist[i];
        if (sum > size*size / 2){ //found median
            r = i;
            break;
        }
    }
    return r;
}

#endif
