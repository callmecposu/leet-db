#include <algorithm>
#include <iomanip>
#include <iostream>
#include "Collection.h"

void Collection::setSchema(const std::vector<FieldMetaData> &schema)
{
    this->schema = schema;
}

void Collection::addDocument(const Document &document)
{
    // check if the dcoument satisfies the schema
    for (auto &field : document.getFields())
    {
        const FieldMetaData md = field.meta;
        auto match = std::find_if(schema.begin(), schema.end(), [md](FieldMetaData &fmd)
                                  { return fmd.name == md.name && fmd.type == md.type; });
        if (match == schema.end())
        {
            throw std::string("Document's structure does not match the schema.");
        }
    }
    // add document to the collection
    documents.push_back(document);
}

void Collection::printDocuments() const
{
    for (auto &document : documents)
    {
        std::cout << "Collection Name: " << name << std::endl;
        std::cout << std::setw(25) << "Field Name" << std::setw(25) << "Field Value" << std::endl;
        for (auto &field : document.getFields())
        {
            std::cout << std::setw(25) << field.meta.name << std::setw(25);
            switch (field.meta.type)
            {
            case FieldType::INT:
                std::cout << field.value.intVal;
                break;
            case FieldType::DOUBLE:
                std::cout << field.value.doubleVal;
                break;
            case FieldType::STRING:
                std::cout << field.value.stringVal;
                break;
            default:
                break;
            }
            std::cout << std::endl;
        }
    }
}

void Collection::serialize(const std::string &filename) const {}

void Collection::deserialize(const std::string &filename) {}