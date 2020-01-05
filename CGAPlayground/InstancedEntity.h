#pragma once
#include "Entity.h"
#include "InstancedModel.h"

class InstancedEntity : public Entity{
	InstancedModel* model;
	GLint index;
public:
	InstancedEntity();
	InstancedEntity(InstancedModel* model, GLint index);
	~InstancedEntity();
	virtual GLvoid render() override;
	virtual GLvoid update(float delta) override;
};

