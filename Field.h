#ifndef FIELD_H
#define FIELD_H

#include <string>

enum class FieldType
{
    INT,
    DOUBLE,
    STRING
};

struct FieldMetaData
{
    std::string name;
    FieldType type;
};

union FieldValue
{
    int intVal;
    double doubleVal;
    std::string stringVal;

    FieldValue(){};
    FieldValue(int val) : intVal(val){};
    FieldValue(double val) : doubleVal(val){};
    FieldValue(const std::string &val) : stringVal(val){};

    FieldValue(const FieldValue &other)
    {
        *this = other;
    }

    FieldValue &operator=(const FieldValue &other)
    {
        intVal = other.intVal;
        doubleVal = other.doubleVal;
        stringVal = other.stringVal;
        return *this;
    }

    ~FieldValue(){};
};

struct Field
{
    FieldMetaData meta;
    FieldValue value;

    Field(){};
    Field(FieldMetaData fmd, FieldValue fv): meta(fmd), value(fv){};
    Field(const Field &other)
    {
        *this = other;
    }
    Field &operator=(const Field &other)
    {
        meta = other.meta;
        value = other.value;
        return *this;
    }
};

#endif