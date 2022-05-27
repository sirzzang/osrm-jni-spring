#include "com_eraser_jniosrm_OsrmJNI.h"
#include "osrm/route_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/storage_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include <cstdlib>
#include <jni.h>

using namespace osrm;

JNIEXPORT jint JNICALL Java_com_eraser_jniosrm_OsrmJNI_main
  (JNIEnv *, jobject)
{

    // OSRM 엔진 설정
    EngineConfig config;

    config.storage_config = boost::filesystem::path{"/home/eraser/projects/osrm-backend/data/south-korea-latest.osrm"}; // TODO: 이렇게 하는 거 맞나?
    config.use_shared_memory = false;
    config.algorithm = EngineConfig::Algorithm::MLD;

    // OSRM 클래스
    const OSRM osrm{config};

    // OSRM route 서비스
    RouteParameters params;

    params.coordinates.push_back({util::FloatLongitude{126.485927}, util::FloatLatitude{37.491975}});
    params.coordinates.push_back({util::FloatLongitude{126.508846}, util::FloatLatitude{37.490757}});

    // OSRM route 출력
    engine::api::ResultT result = json::Object();

    // OSRM route 입력
    const auto status = osrm.Route(params, result);

    auto &json_result = result.get<json::Object>();

    if (status == Status::Ok)
    {
        auto &routes = json_result.values["routes"].get<json::Array>();

        // 첫 번째 route 결과에서 distance, duration 추출
        auto &route = routes.values.at(0).get<json::Object>();
        const auto distance = route.values["distance"].get<json::Number>().value;
        const auto duration = route.values["duration"].get<json::Number>().value;

        // 결과가 없는 경우
        if (distance == 0 || duration == 0)
        {
            std::cout << "Note: distance or duration is zero. ";
            std::cout << "Query might be outside of the OSM extract. \n\n";
        }

        // 결과 반환
        std::cout << "Distance: " << distance << "meter(s) \n";
        std::cout << "Duration: " << duration << "second(s) \n";
        return duration;
    }
    else if (status == Status::Error)
    {
        // 에러 코드, 메시지
        const auto code = json_result.values["code"].get<json::String>().value;
        const auto message = json_result.values["message"].get<json::String>().value;

        std::cout << "Error Code: " << code << "\n";
        std::cout << "Error Message: " << message << "\n";
        return -1;
    }

    return EXIT_SUCCESS;
}
