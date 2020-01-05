#pragma once
#include "Game.h"
#include "ShaderStage.h"
#include "Camera.h"
#include "Entity.h"
#include "Font.h"
#include <GLFW/glfw3.h>
#include "ShaderStageBuilder.h"
#include "Lightcamera.h"
#include "Sun.h"

class MSGame :
	public Game{
public:
	MSGame();
	~MSGame() override;
protected:
	GLvoid update(GLfloat delta) override;
	GLvoid render() override;
	GLvoid resize(GLint width, GLint height) override;
private:
	ShaderStage* fontStage;
	ShaderStage* shadowStage;
	ShaderStage* shadowFilterStage;
	ShaderStage* occlusionGenStage;
	ShaderStage* occlusionFilterStage;
	ShaderStage* skyboxStage;
	ShaderStage* terrainStage;
	ShaderStage* grassStage;
	ShaderStage* wallStage;
	ShaderStage* unitStage;
	ShaderStage* bloomStage;
	ShaderStage* bloomCombineStage;
	ShaderStage* lumStage;
	ShaderStage* shadowGrassStage;
	Camera* camera;
	Font debugFont;

	GLuint oldMouseX, oldMouseY;

	Entity terrain;
	Entity walls[8];
	Entity lamps[16];
	glm::vec3 lampColors[16];
	Lightcamera lightCams[8];
	Entity occuLamps[4];
	Entity builds[12];
	Entity skybox;
	Entity screenPlane;
	std::vector<Entity> billboards;
	std::vector<Entity> units;

	GLfloat currentTime;
	GLuint stoneTileTex;
	GLuint grassTileTex;
	GLuint grassBillboardTex;
	GLfloat grassAlphaFilter;
	GLuint skyboxTex;
	GLuint sceneTex;
	GLuint lumTex;
	GLint shadowMapSize;
	GLint bloomTexAddCount;
	GLint bloomSizeStartDivision;
	GLuint* bloomTexs;
	GLuint shadowMap;
	GLuint sceneFB;
	GLuint* bloomFBs;
	GLuint shadowMapFB;
	GLuint lumFB;
	GLint grassCount;
	GLuint wallTopTex;
	GLuint wallSideTex;
	GLfloat maxGrassGenTime;
	GLuint layeredShadowTex;
	glm::vec2 windDir;
	glm::vec2 targetWindDir;
	std::vector<glm::vec3> unitColors;
	glm::vec3 upperHorizon;
	glm::vec3 lowerHorizon;
	GLfloat horizonOffset;
	Sun* sun;
	GLuint glowTex;
	GLfloat dayCycleSpeed;
	GLfloat dayTime;
	GLint shadowMapFilterKernelSize;
	GLvoid initScene();
	GLvoid loadAssets();
	GLvoid loadFramebuffers();
	GLvoid loadShaderStages();
	GLvoid destroyFramebuffers();
};

