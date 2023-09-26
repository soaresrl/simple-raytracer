#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include <QString>
#include <QList>

#include <qdebug.h>

#include "ui/canvas.h"

#include "octree/voxel.h"

class TreeItem
{
public:
	TreeItem(QString str, TreeItem* parent = nullptr, Drawable* obj = nullptr) :
		m_str(str), m_pParent(parent), m_object(obj)
	{
	}

	~TreeItem()
	{
		qDebug() << "~TreeItem:" << m_str;
		qDeleteAll(m_children);
	}
	
	void appendChild(TreeItem* pChild) { m_children.append(pChild); }
	void insertChild(int idx, TreeItem* pChild) { m_children.insert(idx, pChild); }
	
	void removeChild(TreeItem* p)
	{
		int index = m_children.indexOf(p);
		if (index < 0) { return; }
		m_children.removeAt(index);
		delete p;
	}

	QString data() { return m_str; }
	void setData(QString str) { m_str = str; }
	int childCount() { return m_children.size(); }
	QList<TreeItem*>& children() { return m_children; }

	int columnCount() const
	{
		return 1;/*m_itemData.count();*/
	}

	TreeItem* parentItem()
	{
		return m_pParent;
	}

	TreeItem* child(int row)
	{
		if (row < 0 || row >= childCount()) { return NULL; }
		return m_children[row];
	}

	int row()
	{
		if (m_pParent) {
			return m_pParent->children().indexOf(this);
		}
		return 0;
	}

	void copy(TreeItem* p)
	{
		m_str = p->m_str;
		for (int i = 0; i < p->m_children.size(); i++) {
			insertChild(i, new TreeItem(p->m_children[i]->m_str, this));
			this->m_children[i]->copy(p->m_children[i]);
		}
	}

public:
	Drawable*			m_object;

private:
	QString				m_str;
	TreeItem*			m_pParent;
	QList<TreeItem*>	m_children;
};

#endif // !TREE_ITEM_H
