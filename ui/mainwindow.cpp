#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pTreeModel = new TreeModel(this);

    ui->treeView->setModel(m_pTreeModel);

    auto header = ui->treeView->header();

    header->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(header, &QTreeView::customContextMenuRequested, this, &MainWindow::slotCustomMenuRequested);

    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::click_tree);
    connect(this, &MainWindow::addObject, ui->openGLWidget, &Canvas::AddPrimitive);
    connect(this, &MainWindow::buildOctreeFromPrimitives, ui->openGLWidget, &Canvas::BuildOctreesFromPrimitives);
    connect(this, &MainWindow::clearCanvas, ui->openGLWidget, &Canvas::Clear);

    connect(ui->openGLWidget, &Canvas::PrimitiveAdded, this, &MainWindow::AddPrimitiveToSceneTree);

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::selectItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::click_tree(QModelIndex index)
{
    qDebug() << "click";

    m_selIndex = index;
}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
        
    QMenu* menu = new QMenu(this);

    auto buildOctreeAction = new QAction("Build Octrees", this);
    auto clearAction = new QAction("Clear Scene", this);

    auto sphereAction = new QAction("Add Sphere", this);
    auto cylinderAction = new QAction("Add Cylinder", this);
    auto coneAction = new QAction("Add Cone", this);
    auto blockAction = new QAction("Add Block", this);

    auto unionAction = new QAction("Apply Union", this);
    auto intersectionAction = new QAction("Apply Intersection", this);
    auto differenceAction = new QAction("Apply Difference", this);

    connect(buildOctreeAction, &QAction::triggered, this, &MainWindow::buildOctree);
    connect(clearAction, &QAction::triggered, this, &MainWindow::clearScene);

    connect(sphereAction, &QAction::triggered, this, &MainWindow::addSphere);
    connect(cylinderAction, &QAction::triggered, this, &MainWindow::addCylinder);
    connect(coneAction, &QAction::triggered, this, &MainWindow::addCone);
    connect(blockAction, &QAction::triggered, this, &MainWindow::addBlock);

    connect(unionAction, &QAction::triggered, this, &MainWindow::applyUnion);
    connect(intersectionAction, &QAction::triggered, this, &MainWindow::applyIntersection);
    connect(differenceAction, &QAction::triggered, this, &MainWindow::applyDifference);

    menu->addAction(buildOctreeAction);
    menu->addAction(clearAction);
    menu->addSeparator();
    menu->addAction(sphereAction);
    menu->addAction(cylinderAction);
    menu->addAction(coneAction);
    menu->addAction(blockAction);
    menu->addSeparator();
    menu->addAction(unionAction);
    menu->addAction(intersectionAction);
    menu->addAction(differenceAction);

    menu->popup(ui->treeView->header()->mapToGlobal(pos));
}

void MainWindow::buildOctree() {
    emit buildOctreeFromPrimitives();
}

void MainWindow::clearScene() {
    emit clearCanvas();

    m_pTreeModel->removeRows(0, m_pTreeModel->rowCount(), QModelIndex());
}

void MainWindow::addSphere() {
    emit addObject(0);
}

void MainWindow::addBlock() {
    emit addObject(1);
}

void MainWindow::addCylinder() {
    emit addObject(2);
}

void MainWindow::addCone() {
    emit addObject(3);
}

void MainWindow::selectItem(const QItemSelection& selected, const QItemSelection& deselected) {
    auto size = ui->treeView->selectionModel()->selectedIndexes().size();
    if (size > 2)
    {
        QList<QModelIndex> lst = ui->treeView->selectionModel()->selectedIndexes();
        ui->treeView->selectionModel()->select(lst.last(), QItemSelectionModel::Deselect);

        return;
    }

    if (!deselected.isEmpty() && size < 2) {
        if (static_cast<TreeItem*>(deselected.indexes().first().internalPointer()) == first)
        {
            first = second;
            second = nullptr;
        }
        return;
    }

    if (!selected.isEmpty())
    {
        if (first == nullptr) first = static_cast<TreeItem*>(selected.indexes().first().internalPointer());
        else second = static_cast<TreeItem*>(selected.indexes().first().internalPointer());
    }
}

void MainWindow::applyUnion() {
    ui->openGLWidget->ApplyUnionFromDrawable(first->m_object, second->m_object);
}

void MainWindow::applyIntersection() {
    ui->openGLWidget->ApplyIntersectionFromDrawable(first->m_object, second->m_object);
}

void MainWindow::applyDifference() {
    ui->openGLWidget->ApplyDifferenceFromDrawable(first->m_object, second->m_object);
}


void MainWindow::AddPrimitiveToSceneTree(Drawable* d, QString type) {
    m_pTreeModel->addTree(type, QModelIndex(), d);

    ui->treeView->update();
}