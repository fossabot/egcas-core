#ifndef EGCVARIABLENODE_H
#define EGCVARIABLENODE_H

#include <QString>
#include "../specialNodes/egcnode.h"

/**
 * @brief The EgcVariableNode class is a class that holds the leafes with variable names
 */
class EgcVariableNode : public EgcNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcVariableNode, EgcNodeType::VariableNode);

public:
        ///std constructor
        EgcVariableNode();
        ///destructor
        virtual ~EgcVariableNode();
        /**
         * @brief setValue set the variable name (value)
         * @param varName the variable name as a string
         * @param subscript the subscript of the variable if any
         */
        virtual void setValue(const QString& varName, const QString& subscript);
        /**
         * @brief setValue set the raw variable name (value) maybe including stuffed subscript
         * @param varName the variable name as a string. This can include the stuffed subscript
         */
        virtual void setValueRaw(const QString& varName);
        /**
         * @brief getValue returns the variable name (without subscript)
         * @return the variable name
         */
        QString& getValue(void);
        /**
         * @brief getSubscript returns the subscript of a variable name
         * @return the subscript of this variable object
         */
        virtual QString& getSubscript(void);
        /**
         * @brief getStuffedVar returns the stuffed variable name (with subscript)
         * @return the stuffed variable name (a "_" in the variable name is stuffed into "__",
         * and variable name and subscript is seperated via "_")
         */
        virtual QString getStuffedVar(void);
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * A variable expression is valid if the value is not empty.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
protected:
        QString m_value;          ///< the variable name used
        QString m_subscript;      ///< the subscript if any        
};

#endif // EGCVARIABLENODE_H