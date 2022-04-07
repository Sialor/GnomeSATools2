#pragma once

#include <QTreeWidgetItem>

class ImgTree
{
	QTreeWidgetItem* m_imgTree;

public:
	QTreeWidgetItem* add_item(QTreeWidgetItem* parent, QString name);

	void fill_items();
};