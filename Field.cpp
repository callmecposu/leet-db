#include "Document.h"

void Field::serialize(std::ofstream &ofstr) const
{
    // serialize the field's metadata
    meta.serialize(ofstr);
    // serialize the field's value
    switch (meta.type)
    {
    case FieldType::INT:
        ofstr.write(reinterpret_cast<const char *>(&(value.intVal)), sizeof(int));
        break;
    case FieldType::DOUBLE:
        ofstr.write(reinterpret_cast<const char *>(&(value.doubleVal)), sizeof(double));
        break;
    case FieldType::DOCUMENT:
        value.embeddedDocVal->serialize(ofstr);
        break;
    case FieldType::STRING:
        // write the string size
        size_t stringSize = value.stringVal.size();
        ofstr.write(reinterpret_cast<const char *>(&stringSize), sizeof(size_t));
        // write the string contents
        ofstr.write(value.stringVal.c_str(), stringSize);
        break;
    }
}

void Field::deserialize(std::ifstream &ifstr)
{
    // deserialize the field's metadata
    meta.deserialize(ifstr);
    // deserialize the field's value
    switch (meta.type)
    {
    case FieldType::INT:
        ifstr.read(reinterpret_cast<char *>(&(value.intVal)), sizeof(int));
        break;
    case FieldType::DOUBLE:
        ifstr.read(reinterpret_cast<char *>(&(value.doubleVal)), sizeof(double));
        break;
    case FieldType::STRING:
        // read the string size
        size_t stringSize;
        ifstr.read(reinterpret_cast<char *>(&stringSize), sizeof(size_t));
        // read the string contents
        value.stringVal.resize(stringSize);
        ifstr.read(&(value.stringVal[0]), stringSize);
        break;
    case FieldType::DOCUMENT:
        value.embeddedDocVal = new Document();
        value.embeddedDocVal->deserialize(ifstr, meta.embeddedDocumentSchema.size());
        break;
    }
}
