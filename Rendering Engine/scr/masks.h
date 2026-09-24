#pragma once
#include <glm/glm.hpp>

// 3x3
constexpr int size3x3 = 9;
extern float factor3x3;
extern glm::vec2 texIdx3x3[size3x3];
extern float gauss3x3[size3x3];
extern float sobelX[size3x3];
extern float sobelY[size3x3];
extern float maskFactorEdge;
extern float edgeDetection[size3x3];

// 5x5
constexpr int size5x5 = 25;
extern float factor5x5;
extern glm::vec2 texIdx5x5[size5x5];
extern float gauss5x5[size5x5];

// 7x7
constexpr int size7x7 = 49;
extern float factor7x7;
extern glm::vec2 texIdx7x7[size7x7];
extern float gauss7x7[size7x7];

// 9x9
constexpr int size9x9 = 81;
extern float factor9x9;
extern glm::vec2 texIdx9x9[size9x9];
extern float gauss9x9[size9x9];