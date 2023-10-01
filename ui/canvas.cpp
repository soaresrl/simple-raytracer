#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <thread>
#include <qdebug.h>

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;

#include "canvas.h"


Canvas::Canvas(QWidget* parent) : QOpenGLWidget(parent), elementBuffer(QOpenGLBuffer::IndexBuffer)
{
	qDebug() << "initialize canvas";
	camera = new FlyCamera(this->width(), this->height());

	m_timer = new QTimer(this);

	connect(m_timer, &QTimer::timeout, this, &Canvas::animate);
}

Canvas::~Canvas()
{
}

void Canvas::Clear() {
	objects.clear();
	octrees.clear();
	primitives.Clear();

	update();
}

void Canvas::animate()
{
	uint now_ms = QDateTime::currentMSecsSinceEpoch();
	uint delta_ms = now_ms - m_start_time_ms;
	m_start_time_ms = now_ms;

	update();
	// std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Canvas::BuildOctreesFromPrimitives() {
	qDebug() << "Build Octree";

	std::vector<shared_ptr<Voxel>> octs = primitives.BuildOctreeFromPrimitives();

	octrees.assign(octs.begin(), octs.end());

	update();
}

void Canvas::AddPrimitive(uint type)
{
	switch (type)
	{
	case 0:
	{
		qDebug() << "Add Sphere";
		Sphere* sphere = new Sphere( point3{1.5f, 1.5f, 0.0}, 1.5 );

		sphere->ComputeBoundingBox();

		Drawable* d = new Drawable();

		d->solid = sphere;
		
		primitives.Add(d);

		emit PrimitiveAdded(d, QString("Sphere"));
	}
	break;
	case 1:
	{
		Block* block = new Block( point3{0.0f, 0.0f, 0.0}, 1.0f, 1.0f, 1.0f );
		
		block->ComputeBoundingBox();

		Drawable* d = new Drawable();

		d->solid = block;

		primitives.Add(d);

		emit PrimitiveAdded(d, QString("Block"));
	}
	break;
	case 2:
	{
		Cylinder* cylinder = new Cylinder( point3{0.0f, 0.0f, 0.0}, 1.5, 3.0 );
		
		cylinder->ComputeBoundingBox();

		Drawable* d = new Drawable();

		d->solid = cylinder;

		primitives.Add(d);

		emit PrimitiveAdded(d, QString("Cylinder"));
	}
	break;
	case 3:
	{
		Cone* cone = new Cone( point3{0.0f, 0.0f, 0.0}, 1.5, 3.0 );
		
		cone->ComputeBoundingBox();

		Drawable* d = new Drawable();

		d->solid = cone;

		primitives.Add(d);

		emit PrimitiveAdded(d, QString("Cone"));
	}
	break;
	case 4:
	{
		Cylinder* tronco = new Cylinder(point3{ 0.0f, 0.0f, 0.0f }, 1.0f, 10.0f);
		Block* base = new Block(point3{ 0.0f, .0f, 0.0f }, 4.0, 1.5f, 4.0);
		Block* engine = new Block(point3{ 0.0f, 10.0f, -1.0f }, 2.0, 2.0f, 4.0);
		Block* rotor1 = new Block(point3{ 0.0f, 10.0f, 2.0f }, 10.0f, 2.0f, 2.0f);

		tronco->ComputeBoundingBox();
		base->ComputeBoundingBox();
		engine->ComputeBoundingBox();
		rotor1->ComputeBoundingBox();

		Drawable* troncoD = new Drawable();
		Drawable* baseD = new Drawable();
		Drawable* engineD = new Drawable();
		Drawable* rotor1D = new Drawable();

		troncoD->solid = tronco;
		baseD->solid = base;
		engineD->solid = engine;
		rotor1D->solid = rotor1;

		primitives.Add(troncoD);
		primitives.Add(baseD);
		primitives.Add(engineD);
		primitives.Add(rotor1D);

		BuildOctreesFromPrimitives();

		ApplyUnionFromDrawable(troncoD, engineD);
		ApplyUnionFromDrawable(troncoD, rotor1D);
		ApplyUnionFromDrawable(troncoD, baseD);

		emit PrimitiveAdded(troncoD, QString("Theme"));

		update();
	}
	break;
	default:
		break;
	}
}

void Canvas::initializeGL() {
	initializeOpenGLFunctions();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	program = new QOpenGLShaderProgram();
	program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/vert.glsl");
	program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/frag.glsl");

	if (!program->link()) {
		std::cerr << "> [ERROR]: failed to link shader program: " << program->log().toStdString();
		return;
	}

	mvpMatrixLoc = program->uniformLocation("ModelViewProjection");

	if (!blackNodesVAO.create() || !blackNodesVBO.create() || !elementBuffer.create())
	{
		std::cerr << "ERROR: failed to create vertex array object" << std::endl;
		return;
	}
}

void Canvas::DrawObjects()
{
	for (auto* obj : primitives.solids)
	{
		QMatrix4x4* modelMatrix = new QMatrix4x4();

		modelMatrix->setToIdentity();

		QMatrix4x4 mvpProjection = camera->getViewProjection() * (*modelMatrix);

		program->setUniformValue(mvpMatrixLoc, mvpProjection);

		blackNodesVAO.bind();

		blackNodesVBO.bind();
		blackNodesVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
		blackNodesVBO.allocate(obj->nodesPoints.data(), obj->nodesPoints.size() * sizeof(float));
		blackNodesVBO.release();

		elementBuffer.bind();
		elementBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		elementBuffer.allocate(obj->elementsIndices.data(), obj->elementsIndices.size() * sizeof(uint));
		elementBuffer.release();

		this->glEnableVertexAttribArray(0);
		blackNodesVBO.bind();
		this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0 * sizeof(float));
		elementBuffer.bind();

		//glDrawArrays(GL_POINTS, 0, nodesPoints.size() / 3);
		glDrawElements(GL_LINES, obj->elementsIndices.size(), GL_UNSIGNED_INT, 0 * sizeof(uint));

		blackNodesVAO.release();
		blackNodesVBO.release();
		elementBuffer.release();

		this->glDisableVertexAttribArray(0);
	}
}

void Canvas::paintGL() {
	glViewport(0, 0, width(), height());

	program->bind();

	/*for (PointCloud* pointCloud : pointClouds)
	{
		DrawPointCloud(pointCloud->points);
	}*/
	DrawObjects();


	program->release();
}

void Canvas::resizeGL(int w, int h) {
	camera->setAspectRatio(h, w);

	update();
}

void Canvas::keyPressEvent(QKeyEvent* ev) {
	switch (ev->key())
	{
	case Qt::Key_W:
		camera->moveForward(1.0f);
		break;
	case Qt::Key_S:
		camera->moveForward(-1.0f);
		break;
	case Qt::Key_A:
		camera->moveRight(-1.0f);
		break;
	case Qt::Key_D:
		camera->moveRight(1.0f);
		break;
	case Qt::Key_Up:
		camera->moveUp(1.0f);
		break;
	case Qt::Key_Down:
		camera->moveUp(-1.0f);
		break;
	case Qt::Key_6:
		for (auto d : primitives.solids)
		{
			TranslateOct(d->octree, 0.3, 0.0, 0.0);

			d->nodesPoints.clear();
			d->elementsIndices.clear();

			GetPointsFromTraverse(d->octree, &d->nodesPoints, &d->elementsIndices);
		}
		break;
	case Qt::Key_4:
		for (auto d : primitives.solids)
		{
			TranslateOct(d->octree, -0.3, 0.0, 0.0);

			d->nodesPoints.clear();
			d->elementsIndices.clear();

			GetPointsFromTraverse(d->octree, &d->nodesPoints, &d->elementsIndices);
		}
		break;
	case Qt::Key_2:
		for (auto d : primitives.solids)
		{
			TranslateOct(d->octree, 0.0, -0.3, 0.0);

			d->nodesPoints.clear();
			d->elementsIndices.clear();

			GetPointsFromTraverse(d->octree, &d->nodesPoints, &d->elementsIndices);
		}
		break;
	case Qt::Key_8:
		for (auto d : primitives.solids)
		{
			TranslateOct(d->octree, 0.0, 0.3, 0.0);

			d->nodesPoints.clear();
			d->elementsIndices.clear();

			GetPointsFromTraverse(d->octree, &d->nodesPoints, &d->elementsIndices);
		}
		break;
	case Qt::Key_H:
		for (auto d : primitives.solids)
		{
			ScaleOct(d->octree, 1.25);

			d->nodesPoints.clear();
			d->elementsIndices.clear();

			GetPointsFromTraverse(d->octree, &d->nodesPoints, &d->elementsIndices);
		}
		break;
	case Qt::Key_J:
		for (auto d : primitives.solids)
		{
			ScaleOct(d->octree, 0.85);

			d->nodesPoints.clear();
			d->elementsIndices.clear();

			GetPointsFromTraverse(d->octree, &d->nodesPoints, &d->elementsIndices);
		}
		break;
	case Qt::Key_R:
	{
		m_timer->start(1000.0f); // 60 FPS
		m_start_time_ms = QDateTime::currentMSecsSinceEpoch();
	}
		break;
	default:
		break;
	}

	update();
}

void Canvas::ApplyUnionFromDrawable(Drawable* first, Drawable* second) {
	shared_ptr<Voxel> union_octree = Union(first->octree, second->octree);

	first->octree = union_octree;

	first->nodesPoints.clear();
	first->elementsIndices.clear();

	primitives.Remove(second);

	GetPointsFromTraverse(first->octree, &first->nodesPoints, &first->elementsIndices);

	update();
}

void Canvas::ApplyIntersectionFromDrawable(Drawable* first, Drawable* second) {
	shared_ptr<Voxel> union_octree = Intersection(first->octree, second->octree);

	first->octree = union_octree;

	first->nodesPoints.clear();
	first->elementsIndices.clear();

	primitives.Remove(second);

	GetPointsFromTraverse(first->octree, &first->nodesPoints, &first->elementsIndices);

	update();
}

void Canvas::ApplyDifferenceFromDrawable(Drawable* first, Drawable* second) {
	shared_ptr<Voxel> union_octree = Difference(first->octree, second->octree);

	first->octree = union_octree;

	first->nodesPoints.clear();
	first->elementsIndices.clear();

	primitives.Remove(second);

	GetPointsFromTraverse(first->octree, &first->nodesPoints, &first->elementsIndices);

	update();
}

void Canvas::WriteFile() {
	int i = 0;
	for (auto& obj : primitives.solids)
	{
		std::ofstream out_octree("29_09_TREE_" + std::to_string(i) + ".txt");
		WriteFromTraverse(obj->octree, out_octree);

		i++;
	}
}

void Canvas::ReadFile() {
	std::ifstream in("29_09_TREE_0.txt");

	auto oct = ReadFromFile(in);

	Drawable* d = new Drawable();

	d->octree = oct;

	primitives.Add(d, false);

	update();

	emit PrimitiveAdded(d, QString("File"));
}