#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>

#include "osrm/route_parameters.hpp"
#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/storage_config.hpp"
#include "osrm/json_container.hpp"
#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include "com_eraser_route_service_OsrmJniUtil.h"

using namespace osrm;

JNIEXPORT jobject JNICALL Java_com_eraser_route_service_OsrmJniUtil_getOsrmRouteResponse
  (JNIEnv *env, jobject, jdouble fromLongitude, jdouble fromLatitude, jdouble toLongitude, jdouble toLatitude)
{
    
    jclass clazz;
    jmethodID constructor;
    jfieldID code_fid;
    jfieldID message_fid;
    jfieldID distance_fid;
    jfieldID duration_fid;
    jobject response;

    // load OsrmRouteDto class
    clazz = env -> FindClass("com/eraser/route/service/dto/OsrmRouteDto");

    // methodID, fieldID from OsrmRouteDto class and initialize corresponding instance fields
    constructor = env -> GetMethodID(clazz, "<init>", "()V");
    code_fid = env -> GetFieldID(clazz, "code", "Ljava/lang/String");
    message_fid = env -> GetFieldID(clazz, "message", "Ljava/lang/String");
    distance_fid = env -> GetFieldID(clazz, "distance", "D");
    duration_fid = env -> GetFieldID(clazz, "duration", "D");

    // instantiate java object
    response = env -> NewObject(clazz, constructor);

    // OSRM engine
    EngineConfig config;
    config.storage_config = boost::filesystem::path{"/home/eraser/projects/osrm-backend/data/south-korea-latest.osrm"};
    config.use_shared_memory = false;
    config.algorithm = EngineConfig::Algorithm::MLD;

    // OSRM instance
    const OSRM osrm{config};

    // OSRM route parameters
    RouteParameters params;
    params.coordinates.push_back(
        {util::FloatLongitude{fromLongitude}, util::FloatLatitude{fromLatitude}}
    );
    params.coordinates.push_back(
        {util::FloatLongitude{toLongitude}, util::FloatLatitude{toLatitude}}
    );

    // OSRM route api result
    engine::api::ResultT result = json::Object();
    const auto status = osrm.Route(params, result);
    auto &json_result = result.get<json::Object>();

    // OSRM response code, message to be returned
    const auto code = json_result.values["code"].get<json::String>().value;
    const auto message = json_result.values["message"].get<json::String>().value;
    jstring code_str = env -> NewStringUTF(code.c_str());
    jstring message_str = env -> NewStringUTF(message.c_str());
    
    // OSRM distance, duration to be returned
    if (status == Status::Ok) {
        auto &routes =json_result.values["routes"].get<json::Array>();
        
        // extract first route object
        /**
         *  @brief alternative route objects should not be used(TODO)
         */
        auto &route = routes.values.at(0).get<json::Object>();

        const auto distance = route.values["distance"].get<json::Number>().value;
        const auto duration = route.values["duration"].get<json::Number>().value;

        // set distance, duration field
        env -> SetDoubleField(response, duration_fid, duration);
        env -> SetDoubleField(response, distance_fid, distance);
    }

    // set code, message field
    env -> SetObjectField(response, code_fid, code_str);
    env -> SetObjectField(response, message_fid, message_str);

    // delete reference not used
    env -> DeleteLocalRef(code_str);
    env -> DeleteLocalRef(message_str);

    return response;
};
