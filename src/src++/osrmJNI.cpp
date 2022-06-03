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

const osrm::OSRM *createOsrmInstance()
{
    /**
     * OSRM 클래스 정적 할당
     * - 컴파일 시에 생성
     * - 정적 생성에 의해 스택 메모리 상에 위치
     * cf) 동적 할당: OSRM *osrm; osrm = new OSRM();
     * cf) 기본형 변수 &, 참조형 변수 new?
     *
     * OSRM 클래스 소멸자
     * OSRM::~OSRM() = default;
     * OSRM::OSRM(OSRM &&) noexcept = default;
     * OSRM &OSRM::operator=(OSRM &&) noexcept = default;
     */

    // OSRM instance
    EngineConfig config;
    config.storage_config = boost::filesystem::path{"/home/eraser/projects/osrm-backend/data/south-korea-latest.osrm"};
    config.use_shared_memory = false;
    config.algorithm = EngineConfig::Algorithm::MLD;

    // OSRM 초기화
    /**
     * uniform 초기화
     * - config를 받아서 초기화? EngineConfig 위치를 받아서 -> &config가 인자
     * - OSRM::OSRM(engine::EngineConfig &config)
     */
    const OSRM osrm{config};
    std::cout << "OSRM instance created: " << &osrm << std::endl;

    // 상수 포인터로 상수 변수를 가리킴
    const OSRM *osrmPtr = &osrm;
    std::cout << "*osrmPtr: " << osrmPtr << std::endl; //  0x7f2b4ba0c248
    std::cout << typeid(osrmPtr).name() << std::endl;  // 0x7f2b4ba0c248
    // 포인터가 가리키는 상수의 값 역참조 불가능

    // NOTE: addreess of local varible 'osrm' returned
    return &osrm;
}

JNIEXPORT jlong JNICALL Java_com_eraser_jniosrm_OsrmJNI_getOsrmPointer(JNIEnv *env, jobject)
{
    std::cout << "returned: " << createOsrmInstance() << std::endl;
    return 1;
};

JNIEXPORT jobject JNICALL Java_com_eraser_jniosrm_OsrmJNI_getOsrmResponse(JNIEnv *env, jobject, jdouble fromLongitude, jdouble fromLatitude, jdouble toLongitude, jdouble toLatitude)
{

    jclass clazz;
    jmethodID constructor;
    jobject response;

    // JNIEnv
    /**
     * NOTE
     * - JNI 인터페이스 포인터(*env)는 자바와 네티이브 메소드를 연결하는 인터페이스 포인터이다
     * - JNIEnv 인자를 통해 메소드와 매핑되는 각 네티이브 함수에 접근하고 사용할 수 있다
     */
    std::cout << "JNIENV: " << (long)&env << std::endl;

    // get java OsrmResponse class
    clazz = env->FindClass("com/eraser/jniosrm/OsrmResponse");
    std::cout << "class loaded: " << (long)&clazz << std::endl;

    // get java OsrmResponse class constructor
    constructor = env->GetMethodID(clazz, "<init>", "()V");
    std::cout << "get constructor from class: " << (long)&constructor << std::endl;

    // java OsrmResponse class fields
    /**
     * NOTE
     * - 필드 ID를 얻어 와서, 필드 값을 얻거나, 필드 값을 설정할 때 사용한다
     * - 필드 ID는 C pointer type이다
     *      - 참고: https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/types.html#field_and_method_ids
     * TODO
     * - jfieldID도 메모리 해제해야 하나?
     * - 해제해줘야 한다고 하면, 어떻게 해줘야 하나?
     *      - C pointer type이므로, delete local def로는 안 되는 듯?
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
    /**
     * NOTE
     * - native code에서 자바 object를 생성하려면,
     *      - 1) FindClass: 생성할 객체의 class를 얻고,
     *      - 2) GetMethodId: 생성할 객체의 constructor를 얻고(<init>, ()v 주의)
     *      - 3) NewObject: 얻은 클래스의 constructor로 인스턴스를 생성한다.
     */
    response = env->NewObject(clazz, constructor);
    std::cout << "osrm response instance: " << (long)&response << std::endl;

    // OSRM 엔진 설정
    /**
     * TODO
     * - 엔진 storage_config 설정 이렇게 하는 거 맞나?
     * - OSRM 관련 인스턴스(config, params 등등) 메모리 해제해야 하나?
     * - 아니면, 해제 안하고 컨테이너 개념처럼 싱글톤으로 로드해서 계속 못 쓰나? C++ native 코드에서?
     */
    EngineConfig config;
    config.storage_config = boost::filesystem::path{"/home/eraser/projects/osrm-backend/data/south-korea-latest.osrm"};
    config.use_shared_memory = false;
    config.algorithm = EngineConfig::Algorithm::MLD;
    std::cout << &config << std::endl;

    // OSRM instance
    const OSRM osrm{config};
    std::cout << "typeid &osrm: " << &osrm << std::endl;

    // OSRM route parameters
    /**
     * RouteParameters 정적 생성
     * TODO: RouteParameters 소멸자 존재하나?
     */
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

    // delete c pointer types
    /**
     * jfieldID, jmethodID는 C pointer type이므로, deleteLocalRef로 메모리 해제 불가능
     * C pointer 타입 메모리 해제 방식으로 해제해야 함
     */
    delete code_fid;
    delete message_fid;
    delete duration_fid;
    delete distance_fid;

    return response;
}
