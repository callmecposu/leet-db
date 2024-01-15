#include "Collection.h"
#include <iostream>

int main()
{
    std::vector<FieldMetaData> schema{FieldMetaData{"name", FieldType::STRING}, FieldMetaData{"age", FieldType::INT}, FieldMetaData{"salary", FieldType::DOUBLE}};
    Collection col("peeps");
    col.setSchema(schema);
    Document doc1(std::vector<Field>{
        Field{ FieldMetaData{"name", FieldType::STRING}, FieldValue(std::string("John"))},
        Field{ FieldMetaData{"age", FieldType::INT}, FieldValue(20)},
        Field{ FieldMetaData{"salary", FieldType::DOUBLE}, FieldValue(13.37)},
    });
    Document doc2(std::vector<Field>{
        Field{FieldMetaData{"name", FieldType::STRING}, FieldValue(std::string("Doe"))},
        Field{FieldMetaData{"salary", FieldType::DOUBLE}, FieldValue(0.1)},
        Field{FieldMetaData{"age", FieldType::INT}, FieldValue(228)},
    });
    try{
    col.addDocument(doc1);
    col.addDocument(doc2);
    col.printDocuments();
    std::cout << "Serializing col1..." << std::endl;
    col.serialize("collections/peeps.bin");
    std::cout << "Finished serializing col1!" << std::endl;
    Collection col2("peeps restored");
    std::cout << "Deserializing col2..." << std::endl;
    col2.deserialize("collections/peeps.bin");
    std::cout << "Finished deserializing col2!" << std::endl;
    col2.printDocuments();
    } catch(std::string err){
        std::cout << "ERROR: " << err << std::endl;
    }
    return 0;
}