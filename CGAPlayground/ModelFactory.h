#pragma once
#include "Model.h"

class ModelFactory {
public:
	ModelFactory() = delete;
	~ModelFactory() = delete;
	static Model* createTriangleModel();
	static Model* createPlaneModel();
	static Model* createScreenPlaneModel();
	static Model* createCube();
	static Model* createSkybox();
	static Model* createBox();
	static Model* createWall();
	static Model * createBillboardModel();
};

