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

    // java OsrmResponse class
    // jclass clazz = env->FindClass("OsrmResponse");
    jclass clazz = env->FindClass("com/eraser/jniosrm/OsrmResponse");
    std::cout << "class loaded: " << &clazz << std::endl;

    // java OsrmResponse constructor and fields
    jmethodID init = env->GetMethodID(clazz, "<init>", "()V");

    // java OsrmResponse fields
    jfieldID code_fid = env->GetFieldID(clazz, "code", "Ljava/lang/String;");
    jfieldID message_fid = env->GetFieldID(clazz, "message", "Ljava/lang/String;");
    jfieldID duration_fid = env->GetFieldID(clazz, "duration", "D"); // 메모리 주소: 0x7fcbe2742000
    jfieldID distance_fid = env->GetFieldID(clazz, "distance", "D");
    std::cout << "duration_fid: " << &duration_fid << std::endl;

    // java OsrmResponse 인스턴스 생성
    jobject osrm_response = env->NewObject(clazz, init);
    std::cout << "osrm response instance: " << &osrm_response << std::endl;

    // OSRM 엔진 설정
    EngineConfig config;
    config.storage_config = boost::filesystem::path{"/home/eraser/projects/osrm-backend/data/south-korea-latest.osrm"}; // TODO: 이렇게 하는 거 맞나?
    config.use_shared_memory = false;
    config.algorithm = EngineConfig::Algorithm::MLD;

    // OSRM 클래스
    const OSRM osrm{config};

    // OSRM route 서비스
    RouteParameters params;

    params.coordinates.push_back({util::FloatLongitude{fromLongitude}, util::FloatLatitude{fromLatitude}});
    params.coordinates.push_back({util::FloatLongitude{toLongitude}, util::FloatLatitude{toLatitude}});

    // OSRM route 출력
    engine::api::ResultT result = json::Object();

    // OSRM route 입력
    const auto status = osrm.Route(params, result);
    auto &json_result = result.get<json::Object>();

    // 코드, 메시지
    const auto code = json_result.values["code"].get<json::String>().value;
    const auto message = json_result.values["message"].get<json::String>().value;
    std::cout << "RESULT DONE!" << std::endl;
    std::cout << "code: " << code << std::endl;
    std::cout << "message: " << message << std::endl;

    // 코드, 메시지 응답
    jstring code_str = env->NewStringUTF(code.c_str());
    jstring message_str = env->NewStringUTF(message.c_str());

    // 요청 성공 시
    if (status == Status::Ok)
    {
        auto &routes = json_result.values["routes"].get<json::Array>();

        // 첫 번째 route 결과에서 distance, duration 추출
        auto &route = routes.values.at(0).get<json::Object>();

        // std::cout << &route; // 메모리 주소: 0x7f100c047490
        const auto distance = route.values["distance"].get<json::Number>().value;
        const auto duration = route.values["duration"].get<json::Number>().value;

        // 결과가 없는 경우
        if (distance == 0 || duration == 0)
        {
            std::cout << "Note: distance or duration is zero. ";
            std::cout << "Query might be outside of the OSM extract. \n\n";
        }

        // // 테스트
        // jniResult.values["duration"] = duration;
        // jniResult.values["distance"] = distance;
        // std::cout << "jniResult: " << &jniResult << std::endl;
        // std::cout << "jniResult, duration: " << jniResult.values["duration"].get<json::Number>().value << std::endl;

        // OsrmResponse 응답
        env->SetObjectField(osrm_response, code_fid, code_str);
        env->SetObjectField(osrm_response, message_fid, message_str);
        env->SetDoubleField(osrm_response, duration_fid, duration);
        env->SetDoubleField(osrm_response, distance_fid, distance);
    }
    else if (status == Status::Error)
    {
        // 에러 코드, 메시지
        const auto code = json_result.values["code"].get<json::String>().value;
        const auto message = json_result.values["message"].get<json::String>().value;

        // 만약 위도, 경도 거꾸로 써서 보낸 경우에 어떻게 나오는가?
        // Error Code: InvalidValue
        // Error Message: Invalid coordinate value.

        std::cout << "Error Code: " << code << "\n";
        std::cout << "Error Message: " << message << "\n";

        // osrm 응답
        env->SetObjectField(osrm_response, code_fid, code_str);
        env->SetObjectField(osrm_response, message_fid, message_str);
    }

    return osrm_response;
}
