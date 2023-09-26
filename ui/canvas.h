#ifndef CANVAS_H
#define CANVAS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOPenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QKeyEvent>
#include <QElapsedTimer>
#include <QTimer>
#include <QDateTime>

#include <vector>

#include "utils/utility.h"

#include "camera/flycamera.h"

#include "octree/voxel.h"

#include "materials/lambertian.h"

#include "octree/primitives/sphere.h"
#include "octree/primitives/block.h"
#include "octree/primitives/cylinder.h"
#include "octree/primitives/cone.h"
#include "octree/boolean_operations/boolean_operations.h"

#include "octree/octree.h"

struct Drawable
{
	std::vector<float> nodesPoints;
	std::vector<uint> elementsIndices;
	shared_ptr<Voxel> octree = nullptr;
	Solid* solid = nullptr;
};

class SolidList
{
public:
	SolidList(){}
	~SolidList(){}

	void Add(Drawable* s) {
		solids.push_back(s);

		ComputeBoundingBox();
	}

	void ComputeBoundingBox() {
		for (auto s : solids)
		{

			if (min.x() > s->solid->Min().x())
			{
				min.setX(s->solid->Min().x());
			}

			if (min.y() > s->solid->Min().y())
			{
				min.setY(s->solid->Min().y());
			}

			if (min.z() > s->solid->Min().z())
			{
				min.setZ(s->solid->Min().z());
			}

			if (max.x() < s->solid->Max().x())
			{
				max.setX(s->solid->Max().x());
			}

			if (max.y() < s->solid->Max().y())
			{
				max.setY(s->solid->Max().y());
			}

			if (max.z() < s->solid->Max().z())
			{
				max.setZ(s->solid->Max().z());
			}
		}
	}

	std::vector<shared_ptr<Voxel>> BuildOctreeFromPrimitives() {
		std::vector<shared_ptr<Voxel>> octrees;

		for (auto* i : solids)
		{
			auto red = make_shared<Lambertian>(color(.65, .05, .05));

			double size = std::max(std::max((max.x() - min.x()), max.y() - min.y()), (max.z() - min.z()));

			point3 center = (max + min) / 2.0f;

			shared_ptr<Voxel> octree = make_shared<Voxel>(size, center, red);

			BuildOctree(i->solid, octree, 6);

			std::vector<float> nodesPoints;
			std::vector<uint> elementsIndices;

			GetPointsFromTraverse(octree, &nodesPoints, &elementsIndices);

			i->elementsIndices = elementsIndices;
			i->nodesPoints = nodesPoints;
			i->octree = octree;

			octrees.push_back(octree);
		}

		return octrees;
	}

	void Remove(Drawable* d) {
		std::vector<Drawable*>::iterator it;

		for (it = solids.begin(); it != solids.end(); it++) {
			if (*it == d) {
				break;
			}
		}

		solids.erase(it);
	}

	void Clear() {
		solids.clear();

		min = point3{ std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
		max = point3{ std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min() };
	}
public:
	std::vector<Drawable*> solids;
private:
	point3 min;
	point3 max;
};

class Canvas : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:

	Canvas(QWidget* parent = nullptr);
	~Canvas() override;

	void ApplyUnionFromDrawable(Drawable* first, Drawable* second);
	void ApplyIntersectionFromDrawable(Drawable* first, Drawable* second);
	void ApplyDifferenceFromDrawable(Drawable* first, Drawable* second);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void keyPressEvent(QKeyEvent* event) override;

	void DrawObjects();


	FlyCamera* camera;
	QOpenGLShaderProgram* program;

	QMatrix4x4 matrixMVP;
	QOpenGLVertexArrayObject blackNodesVAO;
	QOpenGLBuffer blackNodesVBO;

	QOpenGLBuffer elementBuffer;

	GLuint mvpMatrixLoc;

	QTimer* m_timer;
	uint m_start_time_ms;

	std::vector<shared_ptr<Voxel>> octrees;

	std::vector<Drawable> objects;
	SolidList primitives;

public slots:
	void AddPrimitive(uint type);
	void BuildOctreesFromPrimitives();
	void Clear();

private slots:
	void animate();

signals:
	void PrimitiveAdded(Drawable* d, QString type);
};

#endif // !CANVAS_H