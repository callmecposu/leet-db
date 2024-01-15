#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

enum class FieldType {
    INT,
    DOUBLE,
    STRING
};

struct FieldMetadata {
    FieldType type;
    std::string name;
};

union FieldValue {
    int intValue;
    double doubleValue;
    std::string stringValue;

    FieldValue(int value) : intValue(value) {}
    FieldValue(double value) : doubleValue(value) {}
    FieldValue(const std::string& value) : stringValue(value) {}
    ~FieldValue() {}
};

class Collection {
public:
    Collection(const std::string& name) : name(name) {}

    void addField(FieldType type, const std::string& name) {
        fields.emplace_back(FieldMetadata{type, name});
    }

    void addDocument(const std::vector<FieldValue>& values) {
        if (values.size() != fields.size()) {
            std::cerr << "Error: Number of values does not match the number of fields." << std::endl;
            return;
        }

        documents.push_back(values);
    }

    void serialize(const std::string& filename) const {
        std::ofstream outFile(filename, std::ios::binary);

        if (outFile.is_open()) {
            // Write collection name
            size_t nameSize = name.size();
            outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
            outFile.write(name.c_str(), nameSize);

            // Write number of fields
            size_t numFields = fields.size();
            outFile.write(reinterpret_cast<const char*>(&numFields), sizeof(numFields));

            // Write field metadata
            for (const auto& field : fields) {
                outFile.write(reinterpret_cast<const char*>(&field), sizeof(FieldMetadata));
            }

            // Write number of documents
            size_t numDocuments = documents.size();
            outFile.write(reinterpret_cast<const char*>(&numDocuments), sizeof(numDocuments));

            // Write document values
            for (const auto& document : documents) {
                for (const auto& value : document) {
                    switch (value.intValue) {
                        case FieldType::INT:
                            outFile.write(reinterpret_cast<const char*>(&value.intValue), sizeof(int));
                            break;
                        case FieldType::DOUBLE:
                            outFile.write(reinterpret_cast<const char*>(&value.doubleValue), sizeof(double));
                            break;
                        case FieldType::STRING:
                            size_t strSize = value.stringValue.size();
                            outFile.write(reinterpret_cast<const char*>(&strSize), sizeof(size_t));
                            outFile.write(value.stringValue.c_str(), strSize);
                            break;
                    }
                }
            }

            outFile.close();
        } else {
            std::cerr << "Unable to open file for writing!" << std::endl;
        }
    }

    void deserialize(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);

        if (inFile.is_open()) {
            // Read collection name
            size_t nameSize;
            inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
            name.resize(nameSize);
            inFile.read(&name[0], nameSize);

            // Read number of fields
            size_t numFields;
            inFile.read(reinterpret_cast<char*>(&numFields), sizeof(numFields));

            // Read field metadata
            fields.resize(numFields);
            for (auto& field : fields) {
                inFile.read(reinterpret_cast<char*>(&field), sizeof(FieldMetadata));
            }

            // Read number of documents
            size_t numDocuments;
            inFile.read(reinterpret_cast<char*>(&numDocuments), sizeof(numDocuments));

            // Read document values
            documents.resize(numDocuments);
            for (auto& document : documents) {
                document.resize(numFields);
                for (auto& value : document) {
                    switch (value.intValue) {
                        case FieldType::INT:
                            inFile.read(reinterpret_cast<char*>(&value.intValue), sizeof(int));
                            break;
                        case FieldType::DOUBLE:
                            inFile.read(reinterpret_cast<char*>(&value.doubleValue), sizeof(double));
                            break;
                        case FieldType::STRING:
                            size_t strSize;
                            inFile.read(reinterpret_cast<char*>(&strSize), sizeof(size_t));
                            value.stringValue.resize(strSize);
                            inFile.read(&value.stringValue[0], strSize);
                            break;
                    }
                }
            }

            inFile.close();
        } else {
            std::cerr << "Unable to open file for reading!" << std::endl;
        }
    }

    void printDocuments() const {
        for (const auto& document : documents) {
            std::cout << "Document:";
            for (size_t i = 0; i < fields.size(); ++i) {
                std::cout << " " << fields[i].name << ":";

                switch (document[i].intValue) {
                    case FieldType::INT:
                        std::cout << document[i].intValue;
                        break;
                    case FieldType::DOUBLE:
                        std::cout << document[i].doubleValue;
                        break;
                    case FieldType::STRING:
                        std::cout << document[i].stringValue;
                        break;
                }
            }
            std::cout << std::endl;
        }
    }

private:
    std::string name;
    std::vector<FieldMetadata> fields;
    std::vector<std::vector<FieldValue>> documents;
};

class Database {
public:
    void createCollection(const std::string& collectionName) {
        collections.emplace(collectionName, collectionName);
    }

    void addFieldToCollection(const std::string& collectionName, FieldType type, const std::string& fieldName) {
        auto it = collections.find(collectionName);
        if (it != collections.end()) {
            it->second.addField(type, fieldName);
        } else {
            std::cerr << "Error: Collection not found." << std::endl;
        }
    }

    void addDocumentToCollection(const std::string& collectionName, const std::vector<FieldValue>& values) {
        auto it = collections.find(collectionName);
        if (it != collections.end()) {
            it->second.addDocument(values);
        } else {
            std::cerr << "Error: Collection not found." << std::endl;
        }
    }

    void serializeCollection(const std::string& collectionName, const std::string& filename) const {
        auto it = collections.find(collectionName);
        if (it != collections.end()) {
            it->second.serialize(filename);
        } else {
            std::cerr << "Error: Collection not found." << std::endl;
        }
    }

    void deserializeCollection(const std::string& collectionName, const std::string& filename) {
        auto it = collections.find(collectionName);
        if (it != collections.end()) {
            it->second.deserialize(filename);
        } else {
            std::cerr << "Error: Collection not found." << std::endl;
        }
    }

    void printDocumentsInCollection(const std::string& collectionName) const {
        auto it = collections.find(collectionName);
        if (it != collections.end()) {
            it->second.printDocuments();
        } else {
            std::cerr << "Error: Collection not found." << std::endl;
        }
    }

private:
    std::unordered_map<std::string, Collection> collections;
};

int main() {
    Database db;

    // Create a collection
    db.createCollection("employees");

    // Add fields to the collection
    db.addFieldToCollection("employees", FieldType::STRING, "name");
    db.addFieldToCollection("employees", FieldType::INT, "age");
    db.addFieldToCollection("employees", FieldType::DOUBLE, "salary");

    // Add documents to the collection
    std::vector<FieldValue> document1 = {FieldValue("John Doe"), FieldValue(30), FieldValue(50000.0)};
    std::vector<FieldValue> document2 = {FieldValue("Jane Smith"), FieldValue(25), FieldValue(60000.0)};
    db.addDocumentToCollection("employees", document1);
    db.addDocumentToCollection("employees", document2);

    // Serialize the collection
    db.serializeCollection("employees", "employees_collection.bin");

    // Deserialize the collection
    db.deserializeCollection("employees", "employees_collection.bin");

    // Print documents in the collection
    std::cout << "Documents in the 'employees' collection:" << std::endl;
    db.printDocumentsInCollection("employees");

    return 0;
}
