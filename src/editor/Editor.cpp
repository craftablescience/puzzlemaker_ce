#include "Editor.h"

#include <QMessageBox>
#include <QStyleOption>

Editor::Editor(QWidget* parent)
	: QOpenGLWidget(parent)
	, QOpenGLFunctions_3_2_Core()
	, chamberVertexCount(0)
	, distance(0)
	, fov(30.f) {}

void Editor::initializeGL() {
	if (!this->initializeOpenGLFunctions()) {
		QMessageBox::critical(this, tr("Error"), tr("Unable to initialize OpenGL 3.2 Core context! Please upgrade your computer to preview models."));
		return; // and probably crash right after
	}

	this->shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/chamber.vert");
	this->shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/chamber.frag");
	this->shaderProgram.link();
}

void Editor::resizeGL(int w, int h) {
	this->glViewport(0, 0, w, h);

	const float aspectRatio = static_cast<float>(w) / static_cast<float>(h > 0 ? h : 1);
	const float nearPlane = 0.015f, farPlane = 32768.0f;
	this->projection.setToIdentity();
	this->projection.perspective(this->fov, aspectRatio, nearPlane, farPlane);
}

void Editor::paintGL() {
	QStyleOption opt;
	opt.initFrom(this);

	auto clearColor = opt.palette.color(QPalette::ColorRole::Window);
	this->glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
	this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//this->glEnable(GL_MULTISAMPLE);
	//this->glEnable(GL_DEPTH_TEST);
	//this->glEnable(GL_CULL_FACE);

	// testing
	this->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	this->shaderProgram.bind();

	QMatrix4x4 view;
	QVector3D translation(this->target.x(), this->target.y(), -this->target.z() - this->distance);
	view.translate(translation);
	view.rotate(this->rotation);
	this->shaderProgram.setUniformValue("uMVP", this->projection * view);
	this->shaderProgram.setUniformValue("uMV", view);
	this->shaderProgram.setUniformValue("uNormalMatrix", view.normalMatrix());
	this->shaderProgram.setUniformValue("uEyePosition", translation);
	this->shaderProgram.setUniformValue("uMeshTexture", 0);
	this->shaderProgram.setUniformValue("uMatCapTexture", 1);

	this->chamberVertices.bind();

	int offset = 0;
	int vertexPosLocation = this->shaderProgram.attributeLocation("vPos");
	this->shaderProgram.enableAttributeArray(vertexPosLocation);
	this->shaderProgram.setAttributeBuffer(vertexPosLocation, GL_FLOAT, offset, 3, sizeof(Vec3f));

	offset += sizeof(QVector3D);
	int vertexNormalLocation = this->shaderProgram.attributeLocation("vUV");
	this->shaderProgram.enableAttributeArray(vertexNormalLocation);
	this->shaderProgram.setAttributeBuffer(vertexNormalLocation, GL_FLOAT, offset, 2, sizeof(Vec2f));

	this->glDrawArrays(GL_TRIANGLES, 0, this->chamberVertexCount);

	this->chamberVertices.release();

	this->shaderProgram.release();
}
