#include <iostream>

#include "Collection.h"

int main()
{
    // // std::vector<FieldMetaData> schema{FieldMetaData{"name", FieldType::STRING}, FieldMetaData{"age", FieldType::INT}, FieldMetaData{"salary", FieldType::DOUBLE}};
    
    // std::vector<FieldMetaData> schema{
    //     FieldMetaData{"general", FieldType::DOCUMENT, std::vector<FieldMetaData>{
    //         FieldMetaData{"name", FieldType::STRING},
    //         FieldMetaData{"age", FieldType::INT},
    //         FieldMetaData{"weight", FieldType::DOUBLE},
    //         FieldMetaData{"height", FieldType::DOUBLE}
    //     }},
    //     FieldMetaData{"additional", FieldType::DOCUMENT, std::vector<FieldMetaData>{
    //         FieldMetaData{"occupation", FieldType::STRING},
    //         FieldMetaData{"salary", FieldType::DOUBLE}
    //     }}
    // };
    // Collection col("peeps");

    // col.setSchema(schema);

    // // Document doc1(std::vector<Field>{
    // //     Field{ FieldMetaData{"name", FieldType::STRING}, FieldValue(std::string("John"))},
    // //     Field{ FieldMetaData{"age", FieldType::INT}, FieldValue(20)},
    // //     Field{ FieldMetaData{"salary", FieldType::DOUBLE}, FieldValue(13.37)},
    // // });
    // // Document doc2(std::vector<Field>{
    // //     Field{FieldMetaData{"name", FieldType::STRING}, FieldValue(std::string("Doe"))},
    // //     Field{FieldMetaData{"salary", FieldType::DOUBLE}, FieldValue(0.1)},
    // //     Field{FieldMetaData{"age", FieldType::INT}, FieldValue(228)},
    // // });

    // Document generalEmbeddedDoc1(
    //   std::vector<Field>{
    //     Field{FieldMetaData{"name", FieldType::STRING}, FieldValue(std::string("John"))},
    //     Field{FieldMetaData{"age", FieldType::INT}, FieldValue(20)},
    //     Field{FieldMetaData{"weight", FieldType::DOUBLE}, FieldValue(75.5)},
    //     Field{FieldMetaData{"height", FieldType::DOUBLE}, FieldValue(175.6)}
    //   }  
    // );
    // Document additionalEmbeddedDoc1(
    //     std::vector<Field>{
    //         Field{FieldMetaData{"occupation", FieldType::STRING}, FieldValue(std::string("Software Developer"))},
    //         Field{FieldMetaData{"salary", FieldType::DOUBLE}, FieldValue(86000.0)}
    //     }
    // );
    // Document doc1(
    //     std::vector<Field>{
    //         Field{
    //             FieldMetaData{"general", FieldType::DOCUMENT, std::vector<FieldMetaData>{
    //                 FieldMetaData{"name", FieldType::STRING},
    //                 FieldMetaData{"age", FieldType::INT},
    //                 FieldMetaData{"weight", FieldType::DOUBLE},
    //                 FieldMetaData{"height", FieldType::DOUBLE}
    //         }
    //     }, FieldValue(&generalEmbeddedDoc1)},
    //         Field{
    //             FieldMetaData{"additional", FieldType::DOCUMENT, std::vector<FieldMetaData>{
    //                 FieldMetaData{"occupation", FieldType::STRING},
    //                 FieldMetaData{"salary", FieldType::DOUBLE}
    //             }}, FieldValue(&additionalEmbeddedDoc1)
    //         }
    //     }
    // );

    // try{
    // col.addDocument(doc1);
    // // col.addDocument(doc2);
    // col.printDocuments();
    // std::cout << "Serializing col1..." << std::endl;
    // col.serialize("collections/peeps.bin");
    // std::cout << "Finished serializing col1!" << std::endl;
    // Collection col2("peeps restored");
    // std::cout << "Deserializing col2..." << std::endl;
    // col2.deserialize("collections/peeps.bin");
    // std::cout << "Finished deserializing col2!" << std::endl;
    // col2.printDocuments();
    // } catch(std::string err){
    //     std::cout << "ERROR: " << err << std::endl;
    // }

    // PRODUCTS SCHEMA TEST

    // {
    //   "product_id": "P001",
    //   "name": "Widget",
    //   "price": 19.99,
    //   "details": {
    //     "weight": "1.5 lbs",
    //     "dimensions": {
    //       "length": 5,
    //       "width": 3,
    //       "height": 7
    //     }
    //   }
    // },

    FieldMetaData dimensions{
        "dimensions",
        FieldType::DOCUMENT,
        std::vector<FieldMetaData>{
            FieldMetaData{"length", FieldType::INT},
            FieldMetaData{"width", FieldType::INT},
            FieldMetaData{"height", FieldType::INT},
        }
    };

    FieldMetaData details{
        "details",
        FieldType::DOCUMENT,
        std::vector<FieldMetaData>{
            FieldMetaData{"weight", FieldType::STRING},
            FieldMetaData{"dimensions", FieldType::DOCUMENT, dimensions.embeddedDocumentSchema}
        }
    };

    std::vector<FieldMetaData> productSchema{
        FieldMetaData{"product_id", FieldType::STRING},
        FieldMetaData{"name", FieldType::STRING},
        FieldMetaData{"price", FieldType::DOUBLE},
        FieldMetaData{"details", FieldType::DOCUMENT, details.embeddedDocumentSchema}
    };

    Collection products("products");
    products.setSchema(productSchema);

    Document p1(
        std::vector<Field>{
            Field{
                productSchema[0], //product_id
                FieldValue(std::string("P001"))
            },
            Field{
                productSchema[1], // name
                FieldValue(std::string("Widget"))
            },
            Field{
                productSchema[2], // price
                FieldValue(19.99)
            },
            Field{
                productSchema[3], // details
                FieldValue(
                    new Document(
                        std::vector<Field>{
                            Field{
                                details.embeddedDocumentSchema[0], //weight
                                FieldValue(std::string("1.5 lbs"))
                            },
                            Field{
                                details.embeddedDocumentSchema[1], //dimensions
                                FieldValue(
                                    new Document(
                                        std::vector<Field>{
                                            Field{
                                                dimensions.embeddedDocumentSchema[0], // length
                                                FieldValue(5)
                                            },
                                            Field{
                                                dimensions.embeddedDocumentSchema[1], //width
                                                FieldValue(3)
                                            },
                                            Field{
                                                dimensions.embeddedDocumentSchema[2], // height
                                                FieldValue(7)
                                            }
                                        }
                                    )
                                )
                            }
                        }
                    )
                )
            }
        }
    );

    products.addDocument(p1);
    products.printDocuments();
    std::cout << "Serializing products..." << std::endl;
    products.serialize("collections/products.bin");
    std::cout << "Finished serializing products!" << std::endl;
    Collection col2("products restored");
    std::cout << "Deserializing col2..." << std::endl;
    col2.deserialize("collections/products.bin");
    std::cout << "Finished deserializing col2!" << std::endl;
    col2.printDocuments();
    return 0;
}