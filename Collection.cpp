#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
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
    std::cout << std::setw(52) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
    std::cout << std::left << "Collection Name: " << name << std::endl;
    std::cout << std::setw(52) << std::setfill('-') << "" << std::endl;
    for (auto &document : documents)
    {
        std::cout << std::setw(52) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
        std::cout << std::setw(25) << "Field Name" << '|' << std::setw(25) << "Field Value" << '|' << std::endl;
        std::cout << std::setw(52) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
        for (auto &field : document.getFields())
        {
            std::cout << std::setw(25) << field.meta.name << '|' << std::setw(25);
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
            std::cout << '|' << std::endl;
        }
        std::cout << std::setw(52) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
    }
}

void Collection::serialize(const std::string &filename) const {
    std::ofstream ofstr(filename, std::ios::binary);
    if (ofstr.is_open()){
        // write the size of the collection name
        size_t nameSize = name.size();
        ofstr.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
        // write the actual collection name
        ofstr.write(name.c_str(), nameSize);
        // write the number of fields
        size_t fieldsNum = schema.size();
        ofstr.write(reinterpret_cast<const char*>(&fieldsNum), sizeof(size_t));
        // write the schema
        for (auto& fieldMD: schema){
            fieldMD.serialize(ofstr);
        }
        // write the number of documents
        size_t documentsNum = documents.size();
        ofstr.write(reinterpret_cast<const char*>(&documentsNum), sizeof(size_t));
        // write the documents
        for (auto& document: documents){
            document.serialize(ofstr);
        }
        ofstr.close();
    } else {
        throw std::string("Could not open the file for writing.");
    }
}

void Collection::deserialize(const std::string &filename) {
    std::ifstream ifstr(filename, std::ios::binary);
    if (ifstr.is_open()){
        // read the size of the collection name
        size_t nameSize;
        ifstr.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
        // read the actual collection name
        name.resize(nameSize);
        ifstr.read(&name[0], nameSize);
        // read the number of fields
        size_t fieldsNum;
        ifstr.read(reinterpret_cast<char*>(&fieldsNum), sizeof(size_t));
        // read the schema
        schema.resize(fieldsNum);
        for (auto& field: schema){
            field.deserialize(ifstr);
        }
        // read the number of documents
        size_t documentsNum;
        ifstr.read(reinterpret_cast<char*>(&documentsNum), sizeof(size_t));
        // read the documents
        documents.resize(documentsNum);
        for (auto& document: documents){
            document.deserialize(ifstr, fieldsNum);
        }
    } else {
        throw std::string("Could not open file for reading.");
    }
}