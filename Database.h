#ifndef DATABASE_H
#define DATABASE_G

#include <unordered_map>

#include "Collection.h"

class Database
{
private:
    std::unordered_map<std::string, Collection> collections;

public:
    void createCollection(const std::string &collectionName);
    void addFieldToCollection(const std::string &collectionName, FieldType type, const std::string &fieldName);
    void addDocumentToCollection(const std::string &collectionName, const std::vector<FieldValue> &values);
    void serializeCollection(const std::string &collectionName, const std::string &filename) const;
    void deserializeCollection(const std::string &collectionName, const std::string &filename);
};

#endif