package com.eraser.route.common.validation;

import com.eraser.route.common.annotation.CoordinateValue;

import javax.validation.ConstraintValidator;
import javax.validation.ConstraintValidatorContext;
import java.text.MessageFormat;
import java.util.regex.Pattern;

public class CoordinateValidator implements ConstraintValidator<CoordinateValue, String> {

    private static final Pattern COORDINATE_PATTERN = Pattern
            .compile("(([+-]?\\d+\\.?\\d+)\\s*,\\s*([+-]?\\d+\\.?\\d+))$");

    @Override
    public boolean isValid(String value, ConstraintValidatorContext context) {

        if (value == null || value.length() == 0) {
            return false;
        }

        boolean isValidCoordinate = COORDINATE_PATTERN.matcher(value).matches();

        // // 올바르지 않은 좌표 형식일 때 constraint 에러 메시지 변경
        // if (!isValidCoordinate) {
        // context.disableDefaultConstraintViolation();
        // context.buildConstraintViolationWithTemplate(
        // MessageFormat.format("Coordinate string value {0} is not valid.",
        // value)).addConstraintViolation();
        // }
        return isValidCoordinate;
    }
}
