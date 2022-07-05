package com.eraser.route.common.response;

import com.eraser.route.common.exception.ErrorCode;
import lombok.Builder;
import lombok.Getter;

@Builder
@Getter
public class CommonResponse<T> {

    // Getter 없으면 HttpMediaTypeNotAcceptableException
    private Status status;
    private String code;
    private String message;
    private T data;

    // Success Response with data
    // NOTE: https://akageun.github.io/2019/06/30/lombok-builder-generic.html
    public static <T> CommonResponse<T> onSuccess(String code, String message, T data) {
        return CommonResponse.<T>builder()
                .status(Status.SUCCESS)
                .code(code)
                .message(message)
                .data(data)
                .build();
    }

    // Fail Response with data
    public static <T> CommonResponse<T> onFail(ErrorCode errorCode, T data) {
        return CommonResponse.<T>builder()
                .status(Status.FAIL)
                .code(errorCode.getCode())
                .message(errorCode.getMessage())
                .data(data)
                .build();
    }

    // Success Response without data
    public CommonResponse onSuccess(String code, String message) {
        return CommonResponse.builder()
                .status(Status.SUCCESS)
                .code(code)
                .message(message)
                .build();
    }

    // Fail Response without data
    public CommonResponse onFail(ErrorCode errorCode) {
        return CommonResponse.builder()
                .status(Status.FAIL)
                .code(errorCode.getCode())
                .message(errorCode.getMessage())
                .build();
    }

    public enum Status {
        SUCCESS, FAIL;
    }
}
