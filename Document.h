#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>
#include <iostream>

#include "Field.h"

class Document
{
private:
    std::vector<Field> fields;

public:
    Document(){};
    ~Document(){};
    Document(const std::vector<Field> &fields) { this->fields = fields; }
    void serialize(std::ofstream &ofstr) const
    {
        for (auto &field : fields)
        {
            field.serialize(ofstr);
        }
    }
    void deserialize(std::ifstream &ifstr, size_t fieldsNum)
    {
        fields.resize(fieldsNum);
        for (auto &field : fields)
        {
            field.deserialize(ifstr);
        }
    }
    void print(size_t indentationLevel = 0) const
    {
        for (auto &field : fields)
        {
            for (size_t i = 0; i < indentationLevel; i++)
            {
                std::cout << "  ";
            }
            std::cout << field.meta.name << ": ";
            switch (field.meta.type)
            {
            case FieldType::INT:
                std::cout << field.value.intVal;
                break;
            case FieldType::DOUBLE:
                std::cout << field.value.doubleVal;
                break;
            case FieldType::STRING:
                std::cout << "\"" << field.value.stringVal << "\"";
                break;
            case FieldType::DOCUMENT:
                std::cout << std::endl;
                field.value.embeddedDocVal->print(indentationLevel + 1);
            }
            std::cout << std::endl;
        }
    }
    const std::vector<Field> &getFields() const { return fields; };
};

#endif