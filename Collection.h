#ifndef COLLECTION_H
#define COLLECTION_H

#include "Document.h"

class Collection
{
private:
    std::string name;
    std::vector<FieldMetaData> schema;
    std::vector<Document> documents;
public:
    Collection(const std::string& name): name(name){};
    ~Collection(){};
    void setSchema(const std::vector<FieldMetaData>& schema);
    void addDocument(const Document& document);
    void serialize(const std::string& filename) const;
    void deserialize(const std::string& filename);
    void printDocuments() const;
};
#endif