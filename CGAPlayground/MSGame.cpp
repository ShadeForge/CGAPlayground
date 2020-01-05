#include "MSGame.h"
#define PI 3.14159265359

#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/vector_angle.hpp>
#include <iostream>

#include "ShaderStageBuilder.h"
#include "FramebufferBuilder.h"
#include "TextureBuilder.h"
#include <SOIL.h>
#include "Assets.h"
#include "ModelFactory.h"
#include "Sun.h"


MSGame::MSGame() : Game("CGAPlayground", 800, 600, 2, 3) {
	setClearColor(0, 0, 0, 0);

	bloomSizeStartDivision = 8;
	bloomTexAddCount = 4;
	grassCount = 500;
	maxGrassGenTime = 3.0f;
	grassAlphaFilter = 200.0f / 255.0f;
	shadowMapSize = 4096;
	shadowMapFilterKernelSize = 5;
	dayCycleSpeed = 0.1f;
	windDir = glm::vec2(1, 0);
	targetWindDir = glm::vec2(1, 0);
	currentTime = glfwGetTime();
	bloomTexs = new GLuint[bloomTexAddCount];
	bloomFBs = new GLuint[bloomTexAddCount];

	loadAssets();
	loadFramebuffers();
	loadShaderStages();

	initScene();
	camera = new Camera(glm::vec3(3, 3, 3), glm::vec3(-3, -3, -3), glm::vec3(0, 1, 0), getWindowAspect());
	sun = new Sun();
	oldMouseX = getMouseX();
	oldMouseY = getMouseY();

	glm::mat4 mat = camera->getProjection() * camera->getView();
}

MSGame::~MSGame() {
	delete fontStage;
	delete camera;
}

GLvoid MSGame::update(GLfloat delta) {
	GLfloat moveSpeed = 20.0f * delta;

	if (isKeyDown(GLFW_KEY_UP) || isKeyDown(GLFW_KEY_W)) {
		camera->translate(glm::rotate(glm::vec3(0, 0, moveSpeed), camera->getRotationXZ(), glm::vec3(0, 1, 0)));
	}
	if(isKeyDown(GLFW_KEY_LEFT) || isKeyDown(GLFW_KEY_A)) {
		camera->translate(glm::rotate(glm::vec3(moveSpeed, 0, 0), camera->getRotationXZ(), glm::vec3(0, 1, 0)));
	}
	if (isKeyDown(GLFW_KEY_DOWN) || isKeyDown(GLFW_KEY_S)) {
		camera->translate(glm::rotate(glm::vec3(0, 0, -moveSpeed), camera->getRotationXZ(), glm::vec3(0, 1, 0)));
	}
	if (isKeyDown(GLFW_KEY_RIGHT) || isKeyDown(GLFW_KEY_D)) {
		camera->translate(glm::rotate(glm::vec3(-moveSpeed, 0, 0), camera->getRotationXZ(), glm::vec3(0, 1, 0)));
	}
	if (isKeyDown(GLFW_KEY_ESCAPE))
		this->exit();
	GLint curMX = getMouseX();
	GLint curMY = getMouseY();
	camera->rotateLookDir((GLfloat)(curMX - (GLint)oldMouseX), (GLfloat)((GLint)oldMouseY - curMY));

	oldMouseX = curMX;
	oldMouseY = curMY;

	camera->update();

	if (isKeyDown(GLFW_KEY_R))
		loadShaderStages();
	if (isKeyPressed(GLFW_KEY_1))
		dayCycleSpeed -= 0.1f;
	if (isKeyPressed(GLFW_KEY_2))
		dayCycleSpeed += 0.1f;
	currentTime += delta;
	dayTime += delta * dayCycleSpeed;
	GLfloat dayCycleTime = dayTime;
	skybox.setRotation(glm::vec3(dayCycleTime, 0, 0));
	skybox.update(delta);
	GLfloat dayCycleFactor = (cos(dayCycleTime) + 1) / 2;
	GLfloat riseFactor = 1 -(cos(dayCycleTime * 2.0f + PI / 2.0f + PI / 4.0) + 1) / 4;
	glm::vec3 nightUpperColor = glm::vec3(0.0f, 0.0f, 0.0f) * (1.0f - dayCycleFactor);
	glm::vec3 dayUpperColor = glm::vec3(0.5f, 0.5f, 1) * dayCycleFactor;
	glm::vec3 riseLowerColor = glm::vec3(1.0f, 0.5f, 0.0f) * dayCycleFactor;
	glm::vec3 riseUpperColor = dayUpperColor * riseFactor + nightUpperColor;
	lowerHorizon = riseLowerColor;
	upperHorizon = riseUpperColor;
	horizonOffset = riseFactor - 0.3f;
	
	glm::vec2 rot = glm::rotate(glm::vec2(0, 100), -dayCycleTime);
	sun->setPosition(glm::vec3(0, rot.y, rot.x));
	sun->setView(glm::lookAt(sun->getPosition(), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));
	GLfloat size = 60.0f;
	sun->setProjection(glm::ortho(-size, size, -size, size, 0.1f, 200.0f));

	GLfloat rotationSpeed = 0.1f;

	windDir = glm::rotate(glm::vec2(((sin(currentTime) + 1) / 2.0f) * 0.5f + 0.25f, 0), currentTime * rotationSpeed);
}

GLvoid MSGame::render() {

	GLfloat renderTime = 0;

	glViewport(0, 0, shadowMapSize, shadowMapSize);

	shadowStage->begin();

	glUniformMatrix4fv(shadowStage->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(sun->getProjection()));
	glUniformMatrix4fv(shadowStage->getUniform("view"), 1, GL_FALSE, glm::value_ptr(sun->getView()));

	for (GLint i = 0; i < units.size(); i++) {
		glUniformMatrix4fv(shadowStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(units[i].getModelMatrix()));
		units[i].render();
	}

	for (GLint i = 0; i < 16; i++) {
		glUniformMatrix4fv(shadowStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(lamps[i].getModelMatrix()));
		lamps[i].render();
	}

	for (GLint i = 0; i < 8; i++) {
		glUniformMatrix4fv(shadowStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(walls[i].getModelMatrix()));
		walls[i].render();
	}

	glUniformMatrix4fv(shadowStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(terrain.getModelMatrix()));
	terrain.render();

	shadowStage->end();

	shadowGrassStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassBillboardTex);

	glUniform1f(shadowGrassStage->getUniform("time"), currentTime);
	glUniform2f(shadowGrassStage->getUniform("windDir"), windDir.x, windDir.y);
	glUniformMatrix4fv(shadowGrassStage->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(sun->getProjection()));
	glUniformMatrix4fv(shadowGrassStage->getUniform("view"), 1, GL_FALSE, glm::value_ptr(sun->getView()));

	for each(Entity grass in billboards) {
		glUniformMatrix4fv(shadowGrassStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(grass.getModelMatrix()));
		grass.render();
	}

	shadowGrassStage->end();

	/*shadowFilterStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glUniform1i(shadowFilterStage->getUniform("kernelSize"), shadowMapFilterKernelSize);
	glUniform1i(shadowFilterStage->getUniform("shadowMap"), 0);

	screenPlane.render();

	shadowFilterStage->end();*/

	glViewport(0, 0, getWindowWidth(), getWindowHeight());

	skyboxStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	glUniform1i(skyboxStage->getUniform("skybox"), 0);
	glUniform1f(skyboxStage->getUniform("horizonOffset"), horizonOffset);
	glUniform3f(skyboxStage->getUniform("lowerHorizon"), lowerHorizon.x, lowerHorizon.y, lowerHorizon.z);
	glUniform3f(skyboxStage->getUniform("upperHorizon"), upperHorizon.x, upperHorizon.y, upperHorizon.z);
	glUniformMatrix4fv(skyboxStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(skybox.getModelMatrix()));
	glUniformMatrix4fv(skyboxStage->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
	glUniformMatrix4fv(skyboxStage->getUniform("view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera->getView()))));
	skybox.render();

	skyboxStage->end();

	terrainStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTileTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, stoneTileTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glUniform1i(terrainStage->getUniform("depthMap"), 2);
	glUniform1i(terrainStage->getUniform("grass"), 0);
	glUniform1i(terrainStage->getUniform("stone"), 1);
	glUniform1f(terrainStage->getUniform("grassRange"), 0.25f);
	glUniform1f(terrainStage->getUniform("overlapRange"), 0.3f);
	glUniform2f(terrainStage->getUniform("repeater"), 25, 25);
	glUniformMatrix4fv(terrainStage->getUniform("lightProjection"), 1, GL_FALSE, glm::value_ptr(sun->getProjection()));
	glUniformMatrix4fv(terrainStage->getUniform("lightView"), 1, GL_FALSE, glm::value_ptr(sun->getView()));
	glUniform3f(terrainStage->getUniform("lightPos"), sun->getPosition().x, sun->getPosition().y, sun->getPosition().z);
	glUniformMatrix4fv(terrainStage->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
	glUniformMatrix4fv(terrainStage->getUniform("view"), 1, GL_FALSE, glm::value_ptr(camera->getView()));
	glUniformMatrix4fv(terrainStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(terrain.getModelMatrix()));
	
	terrain.render();

	terrainStage->end();

	wallStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTopTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, wallSideTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glUniform1i(wallStage->getUniform("depthMap"), 2);
	glUniform1i(wallStage->getUniform("top"), 0);
	glUniform1i(wallStage->getUniform("side"), 1);
	glUniform3f(wallStage->getUniform("repeater"), 5, 1, 5);
	glUniformMatrix4fv(wallStage->getUniform("lightProjection"), 1, GL_FALSE, glm::value_ptr(sun->getProjection()));
	glUniformMatrix4fv(wallStage->getUniform("lightView"), 1, GL_FALSE, glm::value_ptr(sun->getView()));
	glUniform3f(wallStage->getUniform("lightPos"), sun->getPosition().x, sun->getPosition().y, sun->getPosition().z);
	glUniformMatrix4fv(wallStage->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
	glUniformMatrix4fv(wallStage->getUniform("view"), 1, GL_FALSE, glm::value_ptr(camera->getView()));

	for (GLint i = 0; i < 4; i++) {
		glUniformMatrix4fv(wallStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(walls[i].getModelMatrix()));
		walls[i].render();
	}

	glUniform3f(wallStage->getUniform("repeater"), 5, 1, 16);

	for (GLint i = 4; i < 8; i++) {
		glUniformMatrix4fv(wallStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(walls[i].getModelMatrix()));
		walls[i].render();
	}

	wallStage->end();

	unitStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	const GLenum attachments[2]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glUniform1i(unitStage->getUniform("depthMap"), 0);
	glUniformMatrix4fv(unitStage->getUniform("lightProjection"), 1, GL_FALSE, glm::value_ptr(sun->getProjection()));
	glUniformMatrix4fv(unitStage->getUniform("lightView"), 1, GL_FALSE, glm::value_ptr(sun->getView()));
	glUniform3f(unitStage->getUniform("lightPos"), sun->getPosition().x, sun->getPosition().y, sun->getPosition().z);
	glUniformMatrix4fv(unitStage->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
	glUniformMatrix4fv(unitStage->getUniform("view"), 1, GL_FALSE, glm::value_ptr(camera->getView()));

	for(GLint i = 0; i < 16; i++) {
		glUniform3f(unitStage->getUniform("unitColor"), lampColors[i].x, lampColors[i].y, lampColors[i].z);
		glUniformMatrix4fv(unitStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(lamps[i].getModelMatrix()));
		glUniform1i(unitStage->getUniform("glowing"), i%2);

		lamps[i].render();
	}

	for(GLint i = 0; i < units.size(); i++) {
		glUniform3f(unitStage->getUniform("unitColor"), unitColors[i].x, unitColors[i].y, unitColors[i].z);
		glUniformMatrix4fv(unitStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(units[i].getModelMatrix()));
		glUniform1i(unitStage->getUniform("glowing"), 1);
		
		units[i].render();
	}
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	
	unitStage->end();

	grassStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassBillboardTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glUniform1i(grassStage->getUniform("tex"), 0);
	glUniform1i(grassStage->getUniform("depthMap"), 1);
	glUniform1f(grassStage->getUniform("time"), currentTime);
	glUniform2f(grassStage->getUniform("windDir"), windDir.x, windDir.y);
	glUniformMatrix4fv(grassStage->getUniform("lightProjection"), 1, GL_FALSE, glm::value_ptr(sun->getProjection()));
	glUniformMatrix4fv(grassStage->getUniform("lightView"), 1, GL_FALSE, glm::value_ptr(sun->getView()));
	glUniform3f(grassStage->getUniform("lightPos"), sun->getPosition().x, sun->getPosition().y, sun->getPosition().z);
	glUniformMatrix4fv(grassStage->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
	glUniformMatrix4fv(grassStage->getUniform("view"), 1, GL_FALSE, glm::value_ptr(camera->getView()));

	for each(Entity grass in billboards) {
		glUniformMatrix4fv(grassStage->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(grass.getModelMatrix()));
		grass.render();
	}

	grassStage->end();

	lumStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, glowTex);

	glUniform1i(lumStage->getUniform("scene"), 0);
	glUniform1i(lumStage->getUniform("glow"), 1);
	screenPlane.render();

	lumStage->end();
	
	GLuint tex = lumTex;

	for (int i = 0; i < bloomTexAddCount; i++) {
		GLint width = getWindowWidth() / (bloomSizeStartDivision + 2 * i);
		GLint height = getWindowHeight() / (bloomSizeStartDivision + 2 * i);
		bloomStage->setFrameBuffer(bloomFBs[i]);
		glViewport(0, 0, width, height);
		bloomStage->begin();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform2f(bloomStage->getUniform("targetSize"), width, height);
		glUniform1i(bloomStage->getUniform("lumination"), 0);

		glUniform1i(bloomStage->getUniform("orientation"), 0);
		screenPlane.render();

		bloomStage->end();

		tex = bloomTexs[i];

		bloomStage->begin();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glUniform2f(bloomStage->getUniform("targetSize"), width, height);
		glUniform1i(bloomStage->getUniform("lumination"), 0);

		glUniform1i(bloomStage->getUniform("orientation"), 1);
		screenPlane.render();

		bloomStage->end();

		renderTime += bloomStage->getRenderTime();
	}

	glViewport(0, 0, getWindowWidth(), getWindowHeight());

	bloomCombineStage->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomTexs[bloomTexAddCount - 1]);

	glUniform1i(bloomCombineStage->getUniform("scene"), 0);
	glUniform1i(bloomCombineStage->getUniform("bloom"), 1);

	screenPlane.render();

	bloomCombineStage->end();

	renderTime += fontStage->getRenderTime();
	renderTime += skyboxStage->getRenderTime();
	renderTime += terrainStage->getRenderTime();
	renderTime += grassStage->getRenderTime();
	renderTime += lumStage->getRenderTime();
	renderTime += bloomCombineStage->getRenderTime();
	renderTime += shadowStage->getRenderTime();
	renderTime += shadowGrassStage->getRenderTime();
	renderTime += bloomStage->getRenderTime();

	fontStage->begin();

	glUniform4f(fontStage->getUniform("textColor"), 1, 1, 1, 1);
	glUniformMatrix4fv(fontStage->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(glm::ortho(0.0f, static_cast<GLfloat>(getWindowWidth()),
																								   0.0f, static_cast<GLfloat>(getWindowHeight()))));
	debugFont.renderText("FPS: " + std::to_string(getFPS()), 5, 45, 1);
	debugFont.renderText("Rendertime: " + std::to_string(renderTime * 1000) + "ms", 5, 5, 1);

	fontStage->end();

	skyboxStage->clearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lumStage->clearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFB);
	glDrawBuffers(2, attachments);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shadowStage->clearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLvoid MSGame::resize(GLint width, GLint height) {
	camera->setAspectRatio(getWindowAspect());
	glViewport(0, 0, width, height);
	destroyFramebuffers();
	loadFramebuffers();
	lumStage->setFrameBuffer(lumFB);
	terrainStage->setFrameBuffer(sceneFB);
	skyboxStage->setFrameBuffer(sceneFB);
	grassStage->setFrameBuffer(sceneFB);
	wallStage->setFrameBuffer(sceneFB);
	unitStage->setFrameBuffer(sceneFB);
	shadowStage->setFrameBuffer(shadowMapFB);
	shadowGrassStage->setFrameBuffer(shadowMapFB);
	shadowFilterStage->setFrameBuffer(shadowMapFB);
}

GLvoid MSGame::initScene() {
	debugFont = assetManager.getFont(DEBUG_FONT);
	terrain = Entity(assetManager.getModel(PLANE_MODEL));
	terrain.setScale(glm::vec3(100, 1, 100));
	skybox = Entity(assetManager.getModel(SKYBOX_MODEL));
	screenPlane = Entity(assetManager.getModel(SCREEN_PLANE_MODEL));

	walls[0] = Entity(assetManager.getModel(WALL_MODEL));
	walls[0].setScale(glm::vec3(15, 3, 3));
	walls[0].setTranslation(glm::vec3(-35, 3, 0));

	walls[1] = Entity(assetManager.getModel(WALL_MODEL));
	walls[1].setScale(glm::vec3(15, 3, 3));
	walls[1].setTranslation(glm::vec3(35, 3, 0));
	walls[1].setRotation(glm::vec3(0, PI, 0));

	walls[2] = Entity(assetManager.getModel(WALL_MODEL));
	walls[2].setScale(glm::vec3(15, 3, 3));
	walls[2].setTranslation(glm::vec3(0, 3, 35));
	walls[2].setRotation(glm::vec3(0, PI/2.0f, 0));

	walls[3] = Entity(assetManager.getModel(WALL_MODEL));
	walls[3].setScale(glm::vec3(15, 3, 3));
	walls[3].setTranslation(glm::vec3(0, 3, -35));
	walls[3].setRotation(glm::vec3(0, -PI / 2.0f, 0));

	walls[4] = Entity(assetManager.getModel(WALL_MODEL));
	walls[4].setScale(glm::vec3(53, 3, 3));
	walls[4].setTranslation(glm::vec3(3, 3, -53));
	walls[4].setRotation(glm::vec3(0, 0, 0));

	walls[5] = Entity(assetManager.getModel(WALL_MODEL));
	walls[5].setScale(glm::vec3(53, 3, 3));
	walls[5].setTranslation(glm::vec3(-3, 3, 53));
	walls[5].setRotation(glm::vec3(0, PI, 0));

	walls[6] = Entity(assetManager.getModel(WALL_MODEL));
	walls[6].setScale(glm::vec3(53, 3, 3));
	walls[6].setTranslation(glm::vec3(-53, 3, -3));
	walls[6].setRotation(glm::vec3(0, PI / 2.0f, 0));

	walls[7] = Entity(assetManager.getModel(WALL_MODEL));
	walls[7].setScale(glm::vec3(53, 3, 3));
	walls[7].setTranslation(glm::vec3(53, 3, 3));
	walls[7].setRotation(glm::vec3(0, -PI / 2.0f, 0));

	lamps[0] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[0].setScale(glm::vec3(0.25f, 3, 0.25f));
	lamps[0].setTranslation(glm::vec3(-28, 3.0f, -9));
	lampColors[0] = glm::vec3(0.1f);
	lamps[1] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[1].setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	lamps[1].setTranslation(glm::vec3(-28, 6.0f, -9));
	lampColors[1] = glm::vec3(1.0f, 0, 0);
	lightCams[0] = Lightcamera(lamps[1].getTranslation(), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 1, 90.0f, 0.1f, 100.0f);

	lamps[2] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[2].setScale(glm::vec3(0.25f, 3, 0.25f));
	lamps[2].setTranslation(glm::vec3(-28, 3.0f, 9));
	lampColors[2] = glm::vec3(0.1f);
	lamps[3] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[3].setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	lamps[3].setTranslation(glm::vec3(-28, 6.0f, 9));
	lampColors[3] = glm::vec3(1.0f, 1.0f, 0.0f);
	lightCams[1] = Lightcamera(lamps[3].getTranslation(), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 1, 90.0f, 0.1f, 100.0f);

	lamps[4] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[4].setScale(glm::vec3(0.25f, 3, 0.25f));
	lamps[4].setTranslation(glm::vec3(28, 3.0f, -9));
	lampColors[4] = glm::vec3(0.1f);
	lamps[5] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[5].setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	lamps[5].setTranslation(glm::vec3(28, 6.0f, -9));
	lampColors[5] = glm::vec3(0.0f, 1.0f, 0);
	lightCams[2] = Lightcamera(lamps[5].getTranslation(), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 1, 90.0f, 0.1f, 100.0f);

	lamps[6] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[6].setScale(glm::vec3(0.25f, 3, 0.25f));
	lamps[6].setTranslation(glm::vec3(28, 3.0f, 9));
	lampColors[6] = glm::vec3(0.1f);
	lamps[7] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[7].setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	lamps[7].setTranslation(glm::vec3(28, 6.0f, 9));
	lampColors[7] = glm::vec3(0.0f, 0, 1.0f);
	lightCams[3] = Lightcamera(lamps[7].getTranslation(), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 1, 90.0f, 0.1f, 100.0f);

	lamps[8] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[8].setScale(glm::vec3(0.25f, 3, 0.25f));
	lamps[8].setTranslation(glm::vec3(-9, 3.0f, -28));
	lampColors[8] = glm::vec3(0.1f);
	lamps[9] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[9].setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	lamps[9].setTranslation(glm::vec3(-9, 6.0f, -28));
	lampColors[9] = glm::vec3(1.0f, 0, 0);
	lightCams[4] = Lightcamera(lamps[9].getTranslation(), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 1, 90.0f, 0.1f, 100.0f);

	lamps[10] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[10].setScale(glm::vec3(0.25f, 3, 0.25f));
	lamps[10].setTranslation(glm::vec3(9, 3.0f, -28));
	lampColors[10] = glm::vec3(0.1f);
	lamps[11] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[11].setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	lamps[11].setTranslation(glm::vec3(9, 6.0f, -28));
	lampColors[11] = glm::vec3(0.0f, 1.0f, 0);
	lightCams[5] = Lightcamera(lamps[11].getTranslation(), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 1, 90.0f, 0.1f, 100.0f);

	lamps[12] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[12].setScale(glm::vec3(0.25f, 3, 0.25f));
	lamps[12].setTranslation(glm::vec3(-9, 3.0f, 28));
	lampColors[12] = glm::vec3(0.1f);
	lamps[13] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[13].setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	lamps[13].setTranslation(glm::vec3(-9, 6.0f, 28));
	lampColors[13] = glm::vec3(1.0f, 1.0f, 0.0f);
	lightCams[6] = Lightcamera(lamps[13].getTranslation(), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 1, 90.0f, 0.1f, 100.0f);

	lamps[14] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[14].setScale(glm::vec3(0.25f, 3, 0.25f));
	lamps[14].setTranslation(glm::vec3(9, 3.0f, 28));
	lampColors[14] = glm::vec3(0.1f);
	lamps[15] = Entity(assetManager.getModel(BOX_MODEL));
	lamps[15].setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	lamps[15].setTranslation(glm::vec3(9, 6.0f, 28));
	lampColors[15] = glm::vec3(0.0f, 0, 1.0f);
	lightCams[7] = Lightcamera(lamps[15].getTranslation(), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 1, 90.0f, 0.1f, 100.0f);

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[0].setTranslation(glm::vec3(-40, 5, -40));
	units[0].setScale(glm::vec3(3.5f, 5, 3.5f));
	unitColors.push_back(glm::vec3(1, 0, 0));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[1].setTranslation(glm::vec3(-40, 2, -20));
	units[1].setScale(glm::vec3(2.5f, 2, 7.5f));
	unitColors.push_back(glm::vec3(1, 0, 0));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[2].setTranslation(glm::vec3(-20, 2, -40));
	units[2].setScale(glm::vec3(7.5f, 2, 2.5f));
	unitColors.push_back(glm::vec3(1, 0, 0));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[3].setTranslation(glm::vec3(40, 5, 40));
	units[3].setScale(glm::vec3(3.5f, 5, 3.5f));
	unitColors.push_back(glm::vec3(0, 0, 1));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[4].setTranslation(glm::vec3(40, 2, 20));
	units[4].setScale(glm::vec3(2.5f, 2, 7.5f));
	unitColors.push_back(glm::vec3(0, 0, 1));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[5].setTranslation(glm::vec3(20, 2, 40));
	units[5].setScale(glm::vec3(7.5f, 2, 2.5f));
	unitColors.push_back(glm::vec3(0, 0, 1));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[6].setTranslation(glm::vec3(40, 5, -40));
	units[6].setScale(glm::vec3(3.5f, 5, 3.5f));
	unitColors.push_back(glm::vec3(0, 1, 0));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[7].setTranslation(glm::vec3(40, 2, -20));
	units[7].setScale(glm::vec3(2.5f, 2, 7.5f));
	unitColors.push_back(glm::vec3(0, 1, 0));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[8].setTranslation(glm::vec3(20, 2, -40));
	units[8].setScale(glm::vec3(7.5f, 2, 2.5f));
	unitColors.push_back(glm::vec3(0, 1, 0));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[9].setTranslation(glm::vec3(-40, 5, 40));
	units[9].setScale(glm::vec3(3.5f, 5, 3.5f));
	unitColors.push_back(glm::vec3(1, 1, 0));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[10].setTranslation(glm::vec3(-40, 2, 20));
	units[10].setScale(glm::vec3(2.5f, 2, 7.5f));
	unitColors.push_back(glm::vec3(1, 1, 0));

	units.push_back(Entity(assetManager.getModel(BOX_MODEL)));
	units[11].setTranslation(glm::vec3(-20, 2, 40));
	units[11].setScale(glm::vec3(7.5f, 2, 2.5f));
	unitColors.push_back(glm::vec3(1, 1, 0));

	GLfloat startTime = glfwGetTime();

	for(int i = 0; i < grassCount; i++) {
		Entity* grass = new Entity(assetManager.getModel(BILLBOARD_MODEL));
		GLfloat x;
		GLfloat z;
		GLboolean again = GL_FALSE;

		do {
			x = getRandom(50) - 25;
			z = getRandom(50) - 25;

			for(GLint j = 0; j < i; j++) {
				GLfloat dist = glm::distance(billboards[j].getTranslation(), glm::vec3(x, 1.0f, z));
				if (dist < 0.05f) {
					again = GL_TRUE;
					break;
				}
				if(glfwGetTime() - startTime > maxGrassGenTime) {
					again = GL_FALSE;
					break;
				}
			}
		} while (again);
		grass->setTranslation(glm::vec3(x, 1, z));
		grass->setScale(glm::vec3(1, 1, 1));
		billboards.push_back(*grass);
	}
}

GLvoid MSGame::loadAssets() {
	assetManager.insertFont(DEBUG_FONT);
	assetManager.insertModel(SKYBOX_MODEL, ModelFactory::createSkybox());
	assetManager.insertModel(PLANE_MODEL, ModelFactory::createPlaneModel());
	assetManager.insertModel(SCREEN_PLANE_MODEL, ModelFactory::createScreenPlaneModel());
	assetManager.insertModel(BOX_MODEL, ModelFactory::createBox());
	assetManager.insertModel(WALL_MODEL, ModelFactory::createWall());
	assetManager.insertModel(BILLBOARD_MODEL, ModelFactory::createBillboardModel());

	TextureBuilder tBuilder;

	GLint width, height;
	GLubyte* data = SOIL_load_image(STONE_TILE_TEXTURE, &width, &height, new int, SOIL_LOAD_RGB);
	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setInternalFormat(GL_RGB);
	tBuilder.setDataFormat(GL_RGB);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.setSize(width, height);
	tBuilder.setData(data);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	tBuilder.setMipmapped(GL_TRUE);
	stoneTileTex = tBuilder.create();

	SOIL_free_image_data(data);

	data = SOIL_load_image(GRASS_TILE_TEXTURE, &width, &height, new int, SOIL_LOAD_RGB);
	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setInternalFormat(GL_RGB);
	tBuilder.setDataFormat(GL_RGB);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.setSize(width, height);
	tBuilder.setData(data);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	tBuilder.setMipmapped(GL_TRUE);
	grassTileTex = tBuilder.create();

	SOIL_free_image_data(data);

	data = SOIL_load_image(WALL_TOP_TEX, &width, &height, new int, SOIL_LOAD_RGB);
	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setInternalFormat(GL_RGB);
	tBuilder.setDataFormat(GL_RGB);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.setSize(width, height);
	tBuilder.setData(data);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	tBuilder.setMipmapped(GL_TRUE);
	wallTopTex = tBuilder.create();

	SOIL_free_image_data(data);

	data = SOIL_load_image(WALL_SIDE_TEX, &width, &height, new int, SOIL_LOAD_RGB);
	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setInternalFormat(GL_RGB);
	tBuilder.setDataFormat(GL_RGB);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.setSize(width, height);
	tBuilder.setData(data);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	tBuilder.setMipmapped(GL_TRUE);
	wallSideTex = tBuilder.create();

	SOIL_free_image_data(data);

	data = SOIL_load_image(GRASS_BILLBOARD, &width, &height, new int, SOIL_LOAD_RGBA);
	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setInternalFormat(GL_RGBA);
	tBuilder.setDataFormat(GL_RGBA);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.setSize(width, height);
	tBuilder.setData(data);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	tBuilder.addParameterArray(GL_TEXTURE_BORDER_COLOR, new GLint[4]{ 0, 0, 0, 0 });
	tBuilder.setMipmapped(GL_TRUE);
	grassBillboardTex = tBuilder.create();

	SOIL_free_image_data(data);

	std::vector<GLubyte*> cubeData;

	tBuilder.setTextureType(GL_TEXTURE_CUBE_MAP);
	tBuilder.setInternalFormat(GL_RGBA);
	tBuilder.setDataFormat(GL_RGBA);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);

	cubeData.push_back(SOIL_load_image(SKYBOX_RIGHT_TEXTURE, &width, &height, new GLint, SOIL_LOAD_RGBA));
	tBuilder.setCubeMapData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubeData[0], width, height);
	cubeData.push_back(SOIL_load_image(SKYBOX_LEFT_TEXTURE, &width, &height, new GLint, SOIL_LOAD_RGBA));
	tBuilder.setCubeMapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cubeData[1], width, height);
	cubeData.push_back(SOIL_load_image(SKYBOX_TOP_TEXTURE, &width, &height, new GLint, SOIL_LOAD_RGBA));
	tBuilder.setCubeMapData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cubeData[2], width, height);
	cubeData.push_back(SOIL_load_image(SKYBOX_BOTTOM_TEXTURE, &width, &height, new GLint, SOIL_LOAD_RGBA));
	tBuilder.setCubeMapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cubeData[3], width, height);
	cubeData.push_back(SOIL_load_image(SKYBOX_BACK_TEXTURE, &width, &height, new GLint, SOIL_LOAD_RGBA));
	tBuilder.setCubeMapData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cubeData[4], width, height);
	cubeData.push_back(SOIL_load_image(SKYBOX_FRONT_TEXTURE, &width, &height, new GLint, SOIL_LOAD_RGBA));
	tBuilder.setCubeMapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cubeData[5], width, height);

	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	skyboxTex = tBuilder.create();

	for each (GLubyte* image in cubeData) {
		SOIL_free_image_data(image);
	}
}

GLvoid MSGame::loadFramebuffers() {
	FramebufferBuilder fbBuilder;
	TextureBuilder tBuilder;

	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setSize(getWindowWidth(), getWindowHeight());
	tBuilder.setInternalFormat(GL_RGBA);
	tBuilder.setDataFormat(GL_RGBA);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	sceneTex = tBuilder.create();

	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setSize(shadowMapSize, shadowMapSize);
	tBuilder.setInternalFormat(GL_RGBA);
	tBuilder.setDataFormat(GL_RGBA);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	GLuint sceneTex2 = tBuilder.create();

	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setSize(getWindowWidth(), getWindowHeight());
	tBuilder.setInternalFormat(GL_RGBA);
	tBuilder.setDataFormat(GL_RGBA);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	lumTex = tBuilder.create();

	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setSize(getWindowWidth(), getWindowHeight());
	tBuilder.setInternalFormat(GL_RGBA);
	tBuilder.setDataFormat(GL_RGBA);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tBuilder.addParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glowTex = tBuilder.create();

	for (int i = 0; i < bloomTexAddCount; i++) {
		tBuilder.setTextureType(GL_TEXTURE_2D);
		tBuilder.setSize(getWindowWidth() / (bloomSizeStartDivision + 2 * i), getWindowHeight() / (bloomSizeStartDivision + 2 * i));
		tBuilder.setInternalFormat(GL_RGBA);
		tBuilder.setDataFormat(GL_RGBA);
		tBuilder.setDataType(GL_UNSIGNED_BYTE);
		tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		tBuilder.addParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		bloomTexs[i] = tBuilder.create();
	}

	tBuilder.setTextureType(GL_TEXTURE_2D);
	tBuilder.setInternalFormat(GL_DEPTH_COMPONENT);
	tBuilder.setDataFormat(GL_DEPTH_COMPONENT);
	tBuilder.setDataType(GL_UNSIGNED_BYTE);
	tBuilder.addParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	tBuilder.addParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	tBuilder.addParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	tBuilder.addParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	tBuilder.addParameterArray(GL_TEXTURE_BORDER_COLOR, new GLfloat[4]{ 1, 1, 1, 1 });
	tBuilder.setSize(shadowMapSize, shadowMapSize);
	shadowMap = tBuilder.create();

	fbBuilder.addTexture(sceneTex, GL_COLOR_ATTACHMENT0);
	fbBuilder.addTexture(glowTex, GL_COLOR_ATTACHMENT1);
	fbBuilder.setDepthBuffering(GL_TRUE);
	fbBuilder.setDepthBufferSize(getWindowWidth(), getWindowHeight());
	fbBuilder.setDepthFormat(GL_DEPTH_COMPONENT);
	sceneFB = fbBuilder.create();

	fbBuilder.addTexture(shadowMap, GL_DEPTH_ATTACHMENT);
	fbBuilder.addTexture(sceneTex2, GL_COLOR_ATTACHMENT0);
	shadowMapFB = fbBuilder.create();

	for (int i = 0; i < bloomTexAddCount; i++) {
		fbBuilder.addTexture(bloomTexs[i], GL_COLOR_ATTACHMENT0);
		bloomFBs[i] = fbBuilder.create();
	}

	fbBuilder.addTexture(lumTex, GL_COLOR_ATTACHMENT0);
	lumFB = fbBuilder.create();
}

GLvoid MSGame::loadShaderStages() {
	ShaderStageBuilder ssBuilder;

	ssBuilder.addFragmentShaderFile("shaders/billboard_frag.glsl");
	ssBuilder.addVertexShaderFile("shaders/billboard_vert.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addAttribute("texCoord", 1);
	ssBuilder.addUniform("projection");
	ssBuilder.addUniform("modelMatrix");
	ssBuilder.addUniform("view");
	ssBuilder.addUniform("time");
	ssBuilder.addUniform("windDir");
	ssBuilder.addUniform("lightProjection");
	ssBuilder.addUniform("lightView");
	ssBuilder.addUniform("lightPos");
	ssBuilder.addUniform("tex");
	ssBuilder.addUniform("depthMap");
	grassStage = ssBuilder.create();
	grassStage->setCullFace(GL_FALSE);
	grassStage->setAlphaTest(GL_TRUE);
	grassStage->setAlphaFunc(GL_GREATER, grassAlphaFilter);
	grassStage->setBlending(GL_TRUE);
	grassStage->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	grassStage->setDepthTest(GL_TRUE);
	grassStage->setFrameBuffer(sceneFB);

	ssBuilder.addVertexShaderFile("shaders/skybox_vert.glsl");
	ssBuilder.addFragmentShaderFile("shaders/skybox_frag.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addUniform("projection");
	ssBuilder.addUniform("view");
	ssBuilder.addUniform("skybox");
	ssBuilder.addUniform("modelMatrix");
	ssBuilder.addUniform("lowerHorizon");
	ssBuilder.addUniform("upperHorizon");
	skyboxStage = ssBuilder.create();
	skyboxStage->setDepthMask(GL_FALSE);
	skyboxStage->setCullFaceMode(GL_FRONT);
	skyboxStage->setFrameBuffer(sceneFB);

	ssBuilder.addFragmentShaderFile("shaders/terrain_frag.glsl");
	ssBuilder.addVertexShaderFile("shaders/terrain_vert.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addUniform("projection");
	ssBuilder.addUniform("view");
	ssBuilder.addUniform("modelMatrix");
	ssBuilder.addUniform("repeater");
	ssBuilder.addUniform("grassRange");
	ssBuilder.addUniform("overlapRange");
	ssBuilder.addUniform("stone");
	ssBuilder.addUniform("grass");
	ssBuilder.addUniform("lightProjection");
	ssBuilder.addUniform("lightView");
	ssBuilder.addUniform("lightPos");
	ssBuilder.addUniform("depthMap");
	terrainStage = ssBuilder.create();
	terrainStage->setCullFace(GL_TRUE);
	terrainStage->setCullFaceMode(GL_BACK);
	terrainStage->setDepthTest(GL_TRUE);
	terrainStage->setFrameBuffer(sceneFB);

	ssBuilder.addFragmentShaderFile("shaders/wall_frag.glsl");
	ssBuilder.addVertexShaderFile("shaders/wall_vert.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addAttribute("normal", 1);
	ssBuilder.addUniform("repeater");
	ssBuilder.addUniform("projection");
	ssBuilder.addUniform("view");
	ssBuilder.addUniform("modelMatrix");
	ssBuilder.addUniform("top");
	ssBuilder.addUniform("side");
	ssBuilder.addUniform("lightProjection");
	ssBuilder.addUniform("lightView");
	ssBuilder.addUniform("lightPos");
	ssBuilder.addUniform("depthMap");
	wallStage = ssBuilder.create();
	wallStage->setCullFace(GL_TRUE);
	wallStage->setCullFace(GL_BACK);
	wallStage->setDepthTest(GL_TRUE);
	wallStage->setFrameBuffer(sceneFB);

	ssBuilder.addFragmentShaderFile("shaders/unit_frag.glsl");
	ssBuilder.addVertexShaderFile("shaders/unit_vert.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addAttribute("normal", 0);
	ssBuilder.addUniform("projection");
	ssBuilder.addUniform("view");
	ssBuilder.addUniform("modelMatrix");
	ssBuilder.addUniform("unitColor");
	ssBuilder.addUniform("lightProjection");
	ssBuilder.addUniform("lightView");
	ssBuilder.addUniform("lightPos");
	ssBuilder.addUniform("depthMap");
	ssBuilder.addUniform("glowing");
	ssBuilder.addFragDataLocation("color", 0);
	ssBuilder.addFragDataLocation("glow", 1);
	unitStage = ssBuilder.create();
	unitStage->setCullFace(GL_TRUE);
	unitStage->setCullFace(GL_BACK);
	unitStage->setDepthTest(GL_TRUE);
	unitStage->setFrameBuffer(sceneFB);

	ssBuilder.addFragmentShaderFile("shaders/shadow_frag.glsl");
	ssBuilder.addVertexShaderFile("shaders/shadow_vert.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addUniform("projection");
	ssBuilder.addUniform("view");
	ssBuilder.addUniform("modelMatrix");
	ssBuilder.addUniform("lightProjection");
	ssBuilder.addUniform("lightView");
	shadowStage = ssBuilder.create();
	shadowStage->setDepthTest(GL_TRUE);
	shadowStage->setAlphaTest(GL_TRUE);
	shadowStage->setAlphaFunc(GL_GREATER, grassAlphaFilter);
	shadowStage->setCullFace(GL_TRUE);
	shadowStage->setCullFaceMode(GL_FRONT);
	shadowStage->setFrameBuffer(shadowMapFB);

	ssBuilder.addFragmentShaderFile("shaders/billboard_frag.glsl");
	ssBuilder.addVertexShaderFile("shaders/billboard_vert.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addAttribute("texCoord", 1);
	ssBuilder.addUniform("modelMatrix");
	ssBuilder.addUniform("projection");
	ssBuilder.addUniform("view");
	ssBuilder.addUniform("time");
	ssBuilder.addUniform("windDir");
	shadowGrassStage = ssBuilder.create();
	shadowGrassStage->setCullFace(GL_FALSE);
	shadowGrassStage->setAlphaTest(GL_TRUE);
	shadowGrassStage->setAlphaFunc(GL_GREATER, grassAlphaFilter);
	shadowGrassStage->setBlending(GL_TRUE);
	shadowGrassStage->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shadowGrassStage->setDepthTest(GL_TRUE);
	shadowGrassStage->setFrameBuffer(shadowMapFB);

	ssBuilder.addFragmentShaderFile("shaders/shadow_pcf.glsl");
	ssBuilder.addVertexShaderFile("shaders/pass_vert.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addUniform("shadowMap");
	shadowFilterStage = ssBuilder.create();
	shadowFilterStage->setCullFace(GL_FALSE);
	shadowFilterStage->setFrameBuffer(shadowMapFB);

	ssBuilder.addVertexShaderFile("shaders/pass_vert.glsl");
	ssBuilder.addFragmentShaderFile("shaders/lum_filter_frag.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addUniform("scene");
	ssBuilder.addUniform("glow");
	lumStage = ssBuilder.create();
	lumStage->setCullFace(GL_FALSE);
	lumStage->setFrameBuffer(lumFB);

	ssBuilder.addVertexShaderFile("shaders/pass_vert.glsl");
	ssBuilder.addFragmentShaderFile("shaders/bloom_frag.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addUniform("orientation");
	ssBuilder.addUniform("lumination");
	ssBuilder.addUniform("targetSize");
	bloomStage = ssBuilder.create();
	bloomStage->setCullFace(GL_FALSE);

	ssBuilder.addVertexShaderFile("shaders/pass_vert.glsl");
	ssBuilder.addFragmentShaderFile("shaders/bloom_combine_frag.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addUniform("scene");
	ssBuilder.addUniform("bloom");
	bloomCombineStage = ssBuilder.create();
	bloomCombineStage->setCullFace(GL_FALSE);

	ssBuilder.addFragmentShaderFile("shaders/text_frag.glsl");
	ssBuilder.addVertexShaderFile("shaders/text_vert.glsl");
	ssBuilder.addAttribute("position", 0);
	ssBuilder.addUniform("projection");
	ssBuilder.addUniform("textColor");
	fontStage = ssBuilder.create();
	fontStage->setBlending(GL_TRUE);
	fontStage->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLvoid MSGame::destroyFramebuffers() {
	glDeleteTextures(1, &sceneTex);
	glDeleteTextures(bloomTexAddCount, &bloomTexs[0]);
	glDeleteRenderbuffers(bloomTexAddCount, &bloomFBs[0]);
	glDeleteRenderbuffers(1, &sceneFB);
}
