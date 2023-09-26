#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <qmenu.h>

#include "ui/canvas.h"
#include "ui/tree_model.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void click_tree(QModelIndex index);
    void slotCustomMenuRequested(QPoint pos);
    void buildOctree();
    void clearScene();
    void addSphere();
    void addCylinder();
    void addCone();
    void addBlock();

    void applyUnion();
    void applyIntersection();
    void applyDifference();

    void selectItem(const QItemSelection& selected, const QItemSelection& deselected);

    void AddPrimitiveToSceneTree(Drawable* d, QString type);

signals:
    void addObject(uint type);
    void buildOctreeFromPrimitives();
    void clearCanvas();

private:
    Ui::MainWindow* ui;
    TreeModel*      m_pTreeModel;
    QModelIndex		m_selIndex;

    TreeItem*       first = nullptr;
    TreeItem*       second = nullptr;
};

#endif // !MAINWINDOW_H