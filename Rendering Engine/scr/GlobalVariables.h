#pragma once

#define NEAR_DIST 0.1f
#define FAR_DIST 50.0f
#define SCREEN_SIZE 500, 500
#define PI 3.141592f
#define RAND_SEED 31415926

#include "ShaderManager.h"
#include "VAOManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "SceneManager.h"
#include "Renderer.h"

//Managers:
extern ShaderManager shaderManager;
extern VAOManager vaoManager;
extern MaterialManager materialManager;
extern MeshManager meshManager;
extern SceneManager sceneManager; 
extern Renderer renderer;
//General:
extern float aspectRatio;
extern bool pause;
extern float objMovementSpeed;
extern float camMovementSpeed;
extern float camRotationSpeed;
extern float lightMovementSpeed;
extern float lightIntensityVariationSpeed;
extern bool enablePostProcessing;
//Motion blur:
extern bool enableBlending;
extern GLclampf motionBlurBrightness;
extern GLclampf motionBlurAlpha;
extern float motionBlurVariationSpeed;
//Dof:
extern float focalDistance;
extern float maxDofDistance;
extern float dofVariationSpeed;