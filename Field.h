#ifndef FIELD_H
#define FIELD_H

#include <fstream>
#include <string>

class Document;

enum class FieldType
{
    INT,
    DOUBLE,
    STRING,
    DOCUMENT
};

struct FieldMetaData
{
    std::string name;
    FieldType type;
    std::vector<FieldMetaData> embeddedDocumentSchema;

    void serialize(std::ofstream &ofstr) const
    {
        // write the size of the field name
        size_t nameSise = name.size();
        ofstr.write(reinterpret_cast<const char *>(&nameSise), sizeof(size_t));
        // write the actual field name
        ofstr.write(name.c_str(), nameSise);
        // write the enum's underlying integral type value
        auto typeValue = static_cast<std::underlying_type_t<FieldType>>(type);
        ofstr.write(reinterpret_cast<const char *>(&typeValue), sizeof(std::underlying_type_t<FieldType>));
        // if the field type is document write the embedded schema
        if (type == FieldType::DOCUMENT)
        {
            // write the number of fields in the embedded schema
            size_t embeddedSchemaFieldsNum = embeddedDocumentSchema.size();
            ofstr.write(reinterpret_cast<const char *>(&embeddedSchemaFieldsNum), sizeof(size_t));
            // write the embedded schema
            for (auto &field : embeddedDocumentSchema)
            {
                field.serialize(ofstr);
            }
        }
    }

    void deserialize(std::ifstream &ifstr)
    {
        // read the size of the field name
        size_t nameSize;
        ifstr.read(reinterpret_cast<char *>(&nameSize), sizeof(size_t));
        // read the actual field name
        name.resize(nameSize);
        ifstr.read(&name[0], nameSize);
        // read the enum as its underlying integral type
        std::underlying_type_t<FieldType> typeValue;
        ifstr.read(reinterpret_cast<char *>(&typeValue), sizeof(std::underlying_type_t<FieldType>));
        // convert the integral value back to the enum type
        type = static_cast<FieldType>(typeValue);
        // if the field type is document read the embedded schema
        if (type == FieldType::DOCUMENT)
        {
            // read the number of fields in the embedded schema
            size_t embeddedSchemaFieldsNum;
            ifstr.read(reinterpret_cast<char *>(&embeddedSchemaFieldsNum), sizeof(size_t));
            // read the embedded schema
            embeddedDocumentSchema.resize(embeddedSchemaFieldsNum);
            for (auto &field : embeddedDocumentSchema)
            {
                field.deserialize(ifstr);
            }
        }
    }
};

union FieldValue
{
    int intVal;
    double doubleVal;
    std::string stringVal;
    Document *embeddedDocVal;

    FieldValue(){};
    FieldValue(int val) : intVal(val){};
    FieldValue(double val) : doubleVal(val){};
    FieldValue(const std::string &val) : stringVal(val){};
    FieldValue(Document* val): embeddedDocVal(val){};

    FieldValue(const FieldValue &other)
    {
        *this = other;
    }

    FieldValue &operator=(const FieldValue &other)
    {
        intVal = other.intVal;
        doubleVal = other.doubleVal;
        stringVal = other.stringVal;
        embeddedDocVal = other.embeddedDocVal;
        return *this;
    }

    ~FieldValue(){};
};

struct Field
{
    FieldMetaData meta;
    FieldValue value;

    Field(){};
    Field(FieldMetaData fmd, FieldValue fv) : meta(fmd), value(fv){};
    Field(const Field &other)
    {
        *this = other;
    }
    Field &operator=(const Field &other)
    {
        meta = other.meta;
        value = other.value;
        return *this;
    }

    void serialize(std::ofstream &ofstr) const;
    // {
    //     // serialize the field's metadata
    //     meta.serialize(ofstr);
    //     // serialize the field's value
    //     switch (meta.type)
    //     {
    //     case FieldType::INT:
    //         ofstr.write(reinterpret_cast<const char *>(&(value.intVal)), sizeof(int));
    //         break;
    //     case FieldType::DOUBLE:
    //         ofstr.write(reinterpret_cast<const char *>(&(value.doubleVal)), sizeof(double));
    //         break;
    //     case FieldType::STRING:
    //         // write the string size
    //         size_t stringSize = value.stringVal.size();
    //         ofstr.write(reinterpret_cast<const char *>(&stringSize), sizeof(size_t));
    //         // write the string contents
    //         ofstr.write(value.stringVal.c_str(), stringSize);
    //         break;
    //     case FieldType::DOCUMENT:
    //         value.embeddedDocVal->serialize(ofstr);
    //         break;
    //     }
    // }

    void deserialize(std::ifstream &ifstr);
    // {
    //     // deserialize the field's metadata
    //     meta.deserialize(ifstr);
    //     // deserialize the field's value
    //     switch (meta.type)
    //     {
    //     case FieldType::INT:
    //         ifstr.read(reinterpret_cast<char *>(&(value.intVal)), sizeof(int));
    //         break;
    //     case FieldType::DOUBLE:
    //         ifstr.read(reinterpret_cast<char *>(&(value.doubleVal)), sizeof(double));
    //         break;
    //     case FieldType::STRING:
    //         // read the string size
    //         size_t stringSize;
    //         ifstr.read(reinterpret_cast<char *>(&stringSize), sizeof(size_t));
    //         // read the string contents
    //         value.stringVal.resize(stringSize);
    //         ifstr.read(&(value.stringVal[0]), stringSize);
    //         break;
    //     case FieldType::DOCUMENT:
    //         value.embeddedDocVal->deserialize(ifstr, meta.embeddedDocumentSchema.size());
    //         break;
    //     }
    // }
};

#endif