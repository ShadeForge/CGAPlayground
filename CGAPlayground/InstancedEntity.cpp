#include "InstancedEntity.h"



InstancedEntity::InstancedEntity() {
}

InstancedEntity::InstancedEntity(InstancedModel * model, GLint index) {
	this->model = model;
	this->index = index;
}


InstancedEntity::~InstancedEntity() {
}

GLvoid InstancedEntity::render() {
	model->render();
}

GLvoid InstancedEntity::update(float delta) {
	model->loadUpData(modelMatrix, index);
}
