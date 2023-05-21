#include "stdafx.h"
#include "DebugNormalsObject.h"

DebugNormalsObject::DebugNormalsObject(std::vector<Vertex>& vertices) {

	Model* model = new Model();
	model->LoadNormalModel(vertices);
	_model = model;

	SetName("normals");
}
DebugNormalsObject::~DebugNormalsObject() {

	if (_model)
		delete _model;
}