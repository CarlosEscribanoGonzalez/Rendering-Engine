#include "GlobalVariables.h"

//Managers:
ShaderManager shaderManager;
VAOManager vaoManager;
MaterialManager materialManager;
MeshManager meshManager;
SceneManager sceneManager;
Renderer renderer;
//General:
float aspectRatio = 1.0f;
bool pause = false;
float objMovementSpeed = 0.05f;
float camMovementSpeed = 1.0f;
float camRotationSpeed = 0.1f;
float lightMovementSpeed = 0.1f;
float lightIntensityVariationSpeed = 0.2f;
bool enablePostProcessing = true;
//Motion blur:
bool enableBlending = true;
GLclampf motionBlurBrightness = 0.5f;
GLclampf motionBlurAlpha = 0.5f;
float motionBlurVariationSpeed = 0.01f;
//Dof:
float focalDistance = -25;
float maxDofDistance = 0.2f;
float dofVariationSpeed = 0.1;