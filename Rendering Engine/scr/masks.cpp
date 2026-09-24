#include "masks.h"

// ---------------- 3x3 ----------------
float factor3x3 = 1.0f / 14.0f;
glm::vec2 texIdx3x3[size3x3] = {
    {-1.0f,  1.0f}, { 0.0f,  1.0f}, { 1.0f,  1.0f},
    {-1.0f,  0.0f}, { 0.0f,  0.0f}, { 1.0f,  0.0f},
    {-1.0f, -1.0f}, { 0.0f, -1.0f}, { 1.0f, -1.0f}
};
float gauss3x3[size3x3] = {
    1.0f * factor3x3, 2.0f * factor3x3, 1.0f * factor3x3,
    2.0f * factor3x3, 2.0f * factor3x3, 2.0f * factor3x3,
    1.0f * factor3x3, 2.0f * factor3x3, 1.0f * factor3x3
};
float sobelX[size3x3] = {
    -1, 0, 1,
   -2, 0, 2,
   -1, 0, 1
};
float sobelY[size3x3] = {
   -1, -2, -1,
    0,  0,  0,
    1,  2,  1
};
float maskFactorEdge = 1.0f / 9.0f;
float edgeDetection[size3x3] = {
     0, -1, 0,
    -1,  4,-1,
     0, -1, 0
};

// ---------------- 5x5 ----------------
float factor5x5 = 1.0f / 65.0f;
glm::vec2 texIdx5x5[size5x5] = {
    {-2.0f,  2.0f}, {-1.0f,  2.0f}, { 0.0f,  2.0f}, { 1.0f,  2.0f}, { 2.0f,  2.0f},
    {-2.0f,  1.0f}, {-1.0f,  1.0f}, { 0.0f,  1.0f}, { 1.0f,  1.0f}, { 2.0f,  1.0f},
    {-2.0f,  0.0f}, {-1.0f,  0.0f}, { 0.0f,  0.0f}, { 1.0f,  0.0f}, { 2.0f,  0.0f},
    {-2.0f, -1.0f}, {-1.0f, -1.0f}, { 0.0f, -1.0f}, { 1.0f, -1.0f}, { 2.0f, -1.0f},
    {-2.0f, -2.0f}, {-1.0f, -2.0f}, { 0.0f, -2.0f}, { 1.0f, -2.0f}, { 2.0f, -2.0f}
};
float gauss5x5[size5x5] = {
    1 * factor5x5,2 * factor5x5,3 * factor5x5,2 * factor5x5,1 * factor5x5,
    2 * factor5x5,3 * factor5x5,4 * factor5x5,3 * factor5x5,2 * factor5x5,
    3 * factor5x5,4 * factor5x5,5 * factor5x5,4 * factor5x5,3 * factor5x5,
    2 * factor5x5,3 * factor5x5,4 * factor5x5,3 * factor5x5,2 * factor5x5,
    1 * factor5x5,2 * factor5x5,3 * factor5x5,2 * factor5x5,1 * factor5x5
};

// ---------------- 7x7 ----------------
float factor7x7 = 1.0f / 175.0f;
glm::vec2 texIdx7x7[size7x7] = {
    {-3,3},{-2,3},{-1,3},{0,3},{1,3},{2,3},{3,3},
    {-3,2},{-2,2},{-1,2},{0,2},{1,2},{2,2},{3,2},
    {-3,1},{-2,1},{-1,1},{0,1},{1,1},{2,1},{3,1},
    {-3,0},{-2,0},{-1,0},{0,0},{1,0},{2,0},{3,0},
    {-3,-1},{-2,-1},{-1,-1},{0,-1},{1,-1},{2,-1},{3,-1},
    {-3,-2},{-2,-2},{-1,-2},{0,-2},{1,-2},{2,-2},{3,-2},
    {-3,-3},{-2,-3},{-1,-3},{0,-3},{1,-3},{2,-3},{3,-3}
};
float gauss7x7[size7x7] = {
    1 * factor7x7,2 * factor7x7,3 * factor7x7,4 * factor7x7,3 * factor7x7,2 * factor7x7,1 * factor7x7,
    2 * factor7x7,3 * factor7x7,4 * factor7x7,5 * factor7x7,4 * factor7x7,3 * factor7x7,2 * factor7x7,
    3 * factor7x7,4 * factor7x7,5 * factor7x7,6 * factor7x7,5 * factor7x7,4 * factor7x7,3 * factor7x7,
    4 * factor7x7,5 * factor7x7,6 * factor7x7,7 * factor7x7,6 * factor7x7,5 * factor7x7,4 * factor7x7,
    3 * factor7x7,4 * factor7x7,5 * factor7x7,6 * factor7x7,5 * factor7x7,4 * factor7x7,3 * factor7x7,
    2 * factor7x7,3 * factor7x7,4 * factor7x7,5 * factor7x7,4 * factor7x7,3 * factor7x7,2 * factor7x7,
    1 * factor7x7,2 * factor7x7,3 * factor7x7,4 * factor7x7,3 * factor7x7,2 * factor7x7,1 * factor7x7
};

// ---------------- 9x9 ----------------
float factor9x9 = 1.0f / 369.0f;
glm::vec2 texIdx9x9[size9x9] = {
    {-4, 4},{-3, 4},{-2, 4},{-1, 4},{0, 4},{1, 4},{2, 4},{3, 4},{4, 4},
    {-4, 3},{-3, 3},{-2, 3},{-1, 3},{0, 3},{1, 3},{2, 3},{3, 3},{4, 3},
    {-4, 2},{-3, 2},{-2, 2},{-1, 2},{0, 2},{1, 2},{2, 2},{3, 2},{4, 2},
    {-4, 1},{-3, 1},{-2, 1},{-1, 1},{0, 1},{1, 1},{2, 1},{3, 1},{4, 1},
    {-4, 0},{-3, 0},{-2, 0},{-1, 0},{0, 0},{1, 0},{2, 0},{3, 0},{4, 0},
    {-4,-1},{-3,-1},{-2,-1},{-1,-1},{0,-1},{1,-1},{2,-1},{3,-1},{4,-1},
    {-4,-2},{-3,-2},{-2,-2},{-1,-2},{0,-2},{1,-2},{2,-2},{3,-2},{4,-2},
    {-4,-3},{-3,-3},{-2,-3},{-1,-3},{0,-3},{1,-3},{2,-3},{3,-3},{4,-3},
    {-4,-4},{-3,-4},{-2,-4},{-1,-4},{0,-4},{1,-4},{2,-4},{3,-4},{4,-4}
};
float gauss9x9[size9x9] = {
    1 * factor9x9,2 * factor9x9,3 * factor9x9,4 * factor9x9,5 * factor9x9,4 * factor9x9,3 * factor9x9,2 * factor9x9,1 * factor9x9,
    2 * factor9x9,3 * factor9x9,4 * factor9x9,5 * factor9x9,6 * factor9x9,5 * factor9x9,4 * factor9x9,3 * factor9x9,2 * factor9x9,
    3 * factor9x9,4 * factor9x9,5 * factor9x9,6 * factor9x9,7 * factor9x9,6 * factor9x9,5 * factor9x9,4 * factor9x9,3 * factor9x9,
    4 * factor9x9,5 * factor9x9,6 * factor9x9,7 * factor9x9,8 * factor9x9,7 * factor9x9,6 * factor9x9,5 * factor9x9,4 * factor9x9,
    5 * factor9x9,6 * factor9x9,7 * factor9x9,8 * factor9x9,9 * factor9x9,8 * factor9x9,7 * factor9x9,6 * factor9x9,5 * factor9x9,
    4 * factor9x9,5 * factor9x9,6 * factor9x9,7 * factor9x9,8 * factor9x9,7 * factor9x9,6 * factor9x9,5 * factor9x9,4 * factor9x9,
    3 * factor9x9,4 * factor9x9,5 * factor9x9,6 * factor9x9,7 * factor9x9,6 * factor9x9,5 * factor9x9,4 * factor9x9,3 * factor9x9,
    2 * factor9x9,3 * factor9x9,4 * factor9x9,5 * factor9x9,6 * factor9x9,5 * factor9x9,4 * factor9x9,3 * factor9x9,2 * factor9x9,
    1 * factor9x9,2 * factor9x9,3 * factor9x9,4 * factor9x9,5 * factor9x9,4 * factor9x9,3 * factor9x9,2 * factor9x9,1 * factor9x9
};