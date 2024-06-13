#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include "World.h"

class Editor : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core {
	Q_OBJECT;

public:
	explicit Editor(QWidget* parent = nullptr);

protected:
	void initializeGL() override;

	void resizeGL(int w, int h) override;

	void paintGL() override;

private:
	World world;

	QOpenGLShaderProgram shaderProgram;
	QOpenGLBuffer chamberVertices{QOpenGLBuffer::Type::VertexBuffer};
	int chamberVertexCount;

	QMatrix4x4 projection;
	float distance;
	QVector3D target;
	float fov;
	QQuaternion rotation;
};
