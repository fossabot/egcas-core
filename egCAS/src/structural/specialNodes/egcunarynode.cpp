#include "egcunarynode.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcUnaryNode::EgcUnaryNode() : m_child(nullptr)
{

}

EgcUnaryNode::EgcUnaryNode(const EgcUnaryNode& orig) : EgcContainerNode(orig)
{
        m_child = nullptr;
        EgcNode *originalChild = const_cast<EgcUnaryNode&>(orig).getChild();
        if (originalChild)
                m_child = originalChild->copy();

        //set the parent also
        if(m_child)
                m_child->provideParent(this);
}

EgcUnaryNode::~EgcUnaryNode()
{
        if (m_child) {
                delete m_child;
                m_child = nullptr;
        }
}

void EgcUnaryNode::setChild(const EgcNode& expression)
{
        if (m_child)
                delete m_child;
        m_child = const_cast<EgcNode*>(&expression);

        //set the parent also
        if(m_child)
                m_child->provideParent(this);
}

EgcNode* EgcUnaryNode::getChild(void) const
{
        return m_child;
}

EgcUnaryNode& EgcUnaryNode::operator=(const EgcUnaryNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (m_child) {
                delete m_child;
                m_child = nullptr;
        }
        //and create a new one
        EgcNode *originalChild = rhs.getChild();
        if (originalChild)
                m_child = originalChild->copy();

        return *this;
}

bool EgcUnaryNode::valid(void)
{
        if (m_child)
                if (m_child->valid())
                        return true;

        return false;
}

bool EgcUnaryNode::isUnaryExpression(void)
{
        return true;
}

void EgcUnaryNode::notifyContainerOnChildDeletion(EgcNode* child)
{
        if (m_child == child)
                m_child = nullptr;
}

bool EgcUnaryNode::isLeaf(void) const
{
        if (m_child == nullptr)
                return true;
        else
                return false;
}

bool EgcUnaryNode::transferPropertiesTo(EgcNode &to)
{
        bool retval = false;
        EgcUnaryNode &to_una = static_cast<EgcUnaryNode&>(to);

        if (to_una.m_child == nullptr) {
                if (to.isUnaryExpression()) {
                        retval = true;
                        to_una.m_child = m_child;
                        to_una.m_parent = m_parent;
                        m_parent->adjustChildPointers(*this, to);
                        m_child = nullptr;
                        m_parent = nullptr;
                }
        }

        return retval;
}

void EgcUnaryNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        if (m_child == &old_child)
                m_child = &new_child;
}

EgcNode* EgcUnaryNode::takeOwnership(EgcNode &child)
{
        EgcNode* retval = nullptr;

        if (m_child == &child) {
                m_child = nullptr;
                child.provideParent(nullptr);
                retval = &child;
        }

        return retval;
}

void EgcUnaryNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

EgcNode* EgcUnaryNode::getChild(quint32 index) const
{
        if (index == 0)
                return m_child;
        else
                return nullptr;
}

bool EgcUnaryNode::setChild(quint32 index, const EgcNode& expression)
{
        bool retval = true;

        if (index == 0) {
                if (m_child)
                        delete m_child;
                m_child = const_cast<EgcNode*>(&expression);

                //set the parent also
                if(m_child)
                        m_child->provideParent(this);
        } else {
                retval = false;
        }

        return retval;
}

quint32 EgcUnaryNode::getNumberChildNodes(void) const
{
        return 1;
}

bool EgcUnaryNode::isFirstChild(EgcNode &child) const
{
        if (m_child == &child)
                return true;
        else
                return false;
}

bool EgcUnaryNode::isLastChild(EgcNode &child) const
{
        if (m_child == &child)
                return true;
        else
                return false;
}

EgcNode* EgcUnaryNode::incrementToNextChild(EgcNode &previousChild) const
{
        (void) previousChild;

        return nullptr;
}

EgcNode* EgcUnaryNode::decrementToPrevChild(EgcNode &previousChild) const
{
        (void) previousChild;

        return nullptr;
}

bool EgcUnaryNode::getIndexChild(EgcNode& child, quint32& index) const
{
        if (child.getParent() == this) {
                index = 0;
                return true;
        }

        return false;
}
