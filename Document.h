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
    Document(const std::vector<Field>& fields){ this->fields = fields;}
    const std::vector<Field>& getFields() const {return fields;};
};


#endif