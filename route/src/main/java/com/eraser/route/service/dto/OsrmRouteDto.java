package com.eraser.route.service.dto;

import lombok.Getter;

@Getter
public class OsrmRouteDto {
    private String code;
    private String message;
    private double distance;
    private double duration;
}
