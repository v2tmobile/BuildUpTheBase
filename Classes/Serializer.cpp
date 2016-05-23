#include "Serializer.h"


#include <json/document.h>
#include <json/stringbuffer.h>
#include <json/writer.h>

#include "FileOperation.h"

#include "HouseBuilding.h"

BaseSerializer::BaseSerializer(std::string filename)
    : filename(filename)
{

};

rj::Document BaseSerializer::get_document(std::string filename) const
{
    if (filename == "") filename = this->filename;

    return FileIO::open_json(filename);
}

void BaseSerializer::save_document(rj::Document& doc, std::string filename) const
{
    if (filename == "") filename = this->filename;

    FileIO::save_json(filename, doc);
}

void BaseSerializer::add_member(rj::Document& doc, std::string key, rj::Value& value)
{
    //build the Values that become the key and values
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value doc_key = rapidjson::Value();
    //NOTE need to use allocator here, AND move the key later it seems like,
    //otherwise the value gets cleaned up
    doc_key.SetString(key.c_str(), key.size(), allocator);

    //all the member to the document
    doc.AddMember(doc_key.Move(), value.Move(), allocator);
};

void BaseSerializer::set_string(rj::Document & doc, std::string key, std::string value)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value doc_value = rapidjson::Value();
    doc_value.SetString(value.c_str(), value.size(), allocator);
    
    this->add_member(doc, key, doc_value);

};

void BaseSerializer::set_double(rj::Document & doc, std::string key, double value)
{
    rapidjson::Value doc_value = rapidjson::Value();
    doc_value.SetDouble(value);
    
    this->add_member(doc, key, doc_value);

};

void BaseSerializer::set_int(rj::Document & doc, std::string key, int value)
{
    rapidjson::Value doc_value = rapidjson::Value();
    doc_value.SetInt(value);

    this->add_member(doc, key, doc_value);
};

BuildingSerializer::BuildingSerializer(std::string filename, spBuilding building)
    : BaseSerializer(filename), building(building)
{
}

void BuildingSerializer::serialize()
{
    rapidjson::Document doc = rapidjson::Document();
    doc.SetObject();

    this->serialize_building_level(doc);
    this->serialize_ingredients(doc);
    this->serialize_workers(doc);

    this->save_document(doc);
}

void BuildingSerializer::serialize_building_level(rapidjson::Document& doc)
{
    this->set_int(doc, "building_level", this->building->building_level);
}

void BuildingSerializer::serialize_ingredients(rapidjson::Document& doc)
{
}

void BuildingSerializer::serialize_workers(rapidjson::Document& doc)
{
}