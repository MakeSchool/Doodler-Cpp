//
//  JSONPacker.cpp
//  Doodler
//
//  Created by Daniel Haaser on 6/2/15.
//
//

#include "JSONPacker.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace cocos2d;

namespace JSONPacker
{

LineData unpackLineDataJSON(std::string json)
{
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    
    rapidjson::Value& startDoc = document["startPoint"];
    rapidjson::Value& endDoc = document["endPoint"];
    rapidjson::Value& colorDoc = document["color"];
    
    Vec2 start = Vec2(startDoc["x"].GetDouble(), startDoc["y"].GetDouble());
    Vec2 end = Vec2(endDoc["x"].GetDouble(), endDoc["y"].GetDouble());
    float radius = document["radius"].GetDouble();
    
    float r = colorDoc["r"].GetDouble();
    float g = colorDoc["g"].GetDouble();
    float b = colorDoc["b"].GetDouble();
    float a = colorDoc["a"].GetDouble();
    Color4F color = Color4F(r, g, b, a);
    
    LineData result;
    result.startPoint = start;
    result.endPoint = end;
    result.radius = radius;
    result.color = color;
    
    return result;
}

std::string packLineData(const LineData lineData)
{
    rapidjson::Document document;
    document.SetObject();
    
    rapidjson::Value startPt(rapidjson::kObjectType);
    startPt.AddMember("x", lineData.startPoint.x, document.GetAllocator());
    startPt.AddMember("y", lineData.startPoint.y, document.GetAllocator());
    
    rapidjson::Value endPt(rapidjson::kObjectType);
    endPt.AddMember("x", lineData.endPoint.x, document.GetAllocator());
    endPt.AddMember("y", lineData.endPoint.y, document.GetAllocator());
    
    rapidjson::Value lineColor(rapidjson::kObjectType);
    lineColor.AddMember("r", lineData.color.r, document.GetAllocator());
    lineColor.AddMember("g", lineData.color.g, document.GetAllocator());
    lineColor.AddMember("b", lineData.color.b, document.GetAllocator());
    lineColor.AddMember("a", lineData.color.a, document.GetAllocator());
    
    document.AddMember("startPoint", startPt, document.GetAllocator());
    document.AddMember("endPoint", endPt, document.GetAllocator());
    document.AddMember("radius", lineData.radius, document.GetAllocator());
    document.AddMember("color", lineColor, document.GetAllocator());
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    
    return std::string(buffer.GetString(), buffer.Size());
}

}