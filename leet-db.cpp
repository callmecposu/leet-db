#include "Collection.h"

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
    col.addDocument(doc1);
    col.printDocuments();
    return 0;
}