package com.eraser.route.common.annotation;

import com.eraser.route.common.validation.CoordinateValidator;

import javax.validation.Constraint;
import javax.validation.Payload;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Constraint(validatedBy = CoordinateValidator.class)
@Target(ElementType.PARAMETER)
@Retention(RetentionPolicy.RUNTIME)
public @interface CoordinateValue {
    String message() default "Coordinate value must not be empty.";
    Class<?>[] groups() default {};
    Class<? extends Payload>[] payload() default {};
}
