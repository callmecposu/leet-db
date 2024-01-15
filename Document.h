#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>

#include "Field.h"

class Document
{
private:
    std::vector<Field> fields;

public:
    Document(){};
    ~Document(){};
    Document(const std::vector<Field> &fields) { this->fields = fields; }
    void serialize(std::ofstream& ofstr) const {
        for(auto& field: fields){
            field.serialize(ofstr);
        }
    }
    void deserialize(std::ifstream& ifstr, size_t fieldsNum){
        fields.resize(fieldsNum);
        for (auto& field: fields){
            field.deserialize(ifstr);
        }
    }
    const std::vector<Field> &getFields() const { return fields; };
};

#endif