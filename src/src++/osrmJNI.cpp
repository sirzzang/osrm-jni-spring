#include "osrm/route_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/storage_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include "com_eraser_jniosrm_OsrmJNI.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include <cstdlib>
#include <jni.h>

using namespace osrm;

JNIEXPORT jobject JNICALL Java_com_eraser_jniosrm_OsrmJNI_getOsrmResponse(JNIEnv *env, jobject, jdouble fromLongitude, jdouble fromLatitude, jdouble toLongitude, jdouble toLatitude)
{

    jclass clazz;
    jmethodID constructor;
    jobject response;

    // JNIEnv
    std::cout << "JNIENV: " << (long)&env << std::endl;

    // get java OsrmResponse class
    clazz = env->FindClass("com/eraser/jniosrm/OsrmResponse");
    std::cout << "class loaded: " << (long)&clazz << std::endl;

    // get java OsrmResponse class constructor
    constructor = env->GetMethodID(clazz, "<constructor>", "()V");
    std::cout << "get constructor from class: " << (long)&constructor << std::endl;

    // java OsrmResponse class fields
    /**
     * TODO
     * - jfieldID도 메모리 반환해야 하나?
     * - 반환해줘야 한다고 하면, 어떻게 해줘야 하나?
     */
    jfieldID code_fid = env->GetFieldID(clazz, "code", "Ljava/lang/String;");
    jfieldID message_fid = env->GetFieldID(clazz, "message", "Ljava/lang/String;");
    jfieldID duration_fid = env->GetFieldID(clazz, "duration", "D");
    jfieldID distance_fid = env->GetFieldID(clazz, "distance", "D");
    std::cout << "get code field code_fid: " << (long)&code_fid << std::endl;
    std::cout << "get message field message_fid: " << (long)&message_fid << std::endl;
    std::cout << "get duration field duration_fid: " << (long)&duration_fid << std::endl;
    std::cout << "get distance field distance_fid: " << (long)&distance_fid << std::endl;

    // instantiate java object
    response = env->NewObject(clazz, constructor);
    std::cout << "osrm response instance: " << (long)&response << std::endl;

    // OSRM 엔진 설정
    // TODO: 엔진 static하게 한 번만 로드해놓을 수 없나? C++ 코드 단에서?
    EngineConfig config;
    config.storage_config = boost::filesystem::path{"/home/eraser/projects/osrm-backend/data/south-korea-latest.osrm"}; // TODO: 이렇게 하는 거 맞나?
    config.use_shared_memory = false;
    config.algorithm = EngineConfig::Algorithm::MLD;

    // OSRM instance
    const OSRM osrm{config};

    // OSRM route parameters
    RouteParameters params;
    params.coordinates.push_back({util::FloatLongitude{fromLongitude}, util::FloatLatitude{fromLatitude}});
    params.coordinates.push_back({util::FloatLongitude{toLongitude}, util::FloatLatitude{toLatitude}});

    // OSRM route result
    engine::api::ResultT result = json::Object();

    // OSRM route call
    const auto status = osrm.Route(params, result);
    auto &json_result = result.get<json::Object>();

    // OSRM route response code, message
    const auto code = json_result.values["code"].get<json::String>().value;
    const auto message = json_result.values["message"].get<json::String>().value;
    // std::cout << "RESULT DONE!" << std::endl;
    // std::cout << "code: " << code << std::endl;
    // std::cout << "message: " << message << std::endl;

    // code, message to be returned
    jstring code_str = env->NewStringUTF(code.c_str());
    jstring message_str = env->NewStringUTF(message.c_str());
    std::cout << "code: " << (long)&code << std::endl;
    std::cout << "code_str: " << (long)&code_str << std::endl;
    std::cout << "message: " << (long)&message << std::endl;
    std::cout << "message_str: " << (long)&message_str << std::endl;

    // OSRM route success
    if (status == Status::Ok)
    {
        auto &routes = json_result.values["routes"].get<json::Array>();

        // extract the first route result
        auto &route = routes.values.at(0).get<json::Object>();

        // extract distance, duration from the route result
        const auto distance = route.values["distance"].get<json::Number>().value;
        const auto duration = route.values["duration"].get<json::Number>().value;

        if (distance == 0 || duration == 0)
        {
            // message += "Distance or duration is zero. Query might be outside of the OSM extract.";
            std::cout << "Note: distance or duration is zero. ";
            std::cout << "Query might be outside of the OSM extract. \n\n";
        }

        // duration, response to be returned
        env->SetDoubleField(response, duration_fid, duration);
        env->SetDoubleField(response, distance_fid, distance);
    }

    // code, message to be returned
    env->SetObjectField(response, code_fid, code_str);
    env->SetObjectField(response, message_fid, message_str);

    // delete reference
    env->DeleteLocalRef(code_str);
    env->DeleteLocalRef(message_str);

    // end of native code
    std::cout << "\n\n";

    return response;
}
