package com.eraser.route.common.exception;

public enum ErrorCode {

    // Client request error
    INVALID_REQUEST_PARAMETER("R4001", "Check if request parameters are valid");

    private String code;
    private String message;

    ErrorCode(String code, String message) {
        this.code = code;
        this.message = message;
    }

    public String getCode() {
        return code;
    }

    public String getMessage() {
        return message;
    }
}