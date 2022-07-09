package com.eraser.route.service;

import com.eraser.route.service.dto.RouteResponseDto;

public interface RouteService {
    public RouteResponseDto getRouteByWalking(String fromCoordinates, String toCoordinates);
}
