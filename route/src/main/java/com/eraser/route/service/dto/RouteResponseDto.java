package com.eraser.route.service.dto;

import lombok.Builder;
import lombok.Getter;

@Getter
public class RouteResponseDto {
    double distance;
    double duration;

    @Builder
    public RouteResponseDto(double duration, double distance) {
        this.distance = distance;
        this.duration = duration;
    }
}
