package com.eraser.route.common.exception;

public enum ErrorCode {

    // Client request error
    INVALID_REQUEST_PARAMETER("R4401", "Check if request parameters are valid."),
    INVALID_INPUT_VALUE("R4402", "Check if request values for input are valid.");

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
