package com.eraser.route.common.exception;

import com.eraser.route.common.response.CommonResponse;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.MissingServletRequestParameterException;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

import javax.validation.ConstraintViolationException;

@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler
    private ResponseEntity<CommonResponse<?>> handleMissingServletRequestParameterException(
            MissingServletRequestParameterException e) {

        CommonResponse<?> errorResponse =
                CommonResponse.onFail(ErrorCode.INVALID_REQUEST_PARAMETER, e.getMessage());

        return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(errorResponse);
    }

    @ExceptionHandler
    private ResponseEntity<CommonResponse<?>> handleConstraintViolationException(
            ConstraintViolationException e) {

        CommonResponse<?> errorResponse =
                CommonResponse.onFail(ErrorCode.INVALID_INPUT_VALUE, e.getMessage());

        return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(errorResponse);
    }
}
