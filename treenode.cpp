#include "treenode.h"

TreeNode::TreeNode(TreeNode *parent)
{
    m_parentNode = parent;
}

TreeNode::~TreeNode()
{

}

void TreeNode::appendChild(TreeNode *child)
{
    m_childNodes.append(child);
}

TreeNode *TreeNode::child(qint32 row)
{
    return m_childNodes.value(row);
}

qint32 TreeNode::childCount() const
{
    return m_childNodes.count();
}

qint32 TreeNode::columnCount() const
{

}

qint32 TreeNode::row() const
{
    if (m_parentNode) {
        return m_parentNode->m_childNodes.indexOf(const_cast<TreeNode*>(this));
    }
    return 0;
}

TreeNode *TreeNode::parentNode()
{
    return m_parentNode;
}
