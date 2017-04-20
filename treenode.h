#ifndef TREENODE_H
#define TREENODE_H

#include <QObject>
#include <QVector>

class TreeNode
{
public:
    explicit TreeNode(TreeNode *parent=nullptr);
    virtual ~TreeNode();

    void appendChild(TreeNode *child);

    TreeNode *child(qint32 row);
    qint32 childCount() const;
    virtual qint32 columnCount() const;
    qint32 row() const;
    TreeNode *parentNode();

    QVector<TreeNode*> m_childNodes;
    TreeNode *m_parentNode;
};

#endif // TREENODE_H
