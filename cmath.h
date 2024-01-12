// place to hold all the custom math functions because
// who know that implimmenting a 3d renderer in scratch would requre ACTUALLY MATH!?!?

#include <math.h>

#define rad2deg 57.2957795131 // ive never had a more useful varuable in my life



float calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

float map(float value, 
                              float istart, 
                              float istop, 
                              float ostart, 
                              float ostop) {
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}
