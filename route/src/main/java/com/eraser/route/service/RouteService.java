package com.eraser.route.service;

import com.eraser.route.service.dto.OsrmRouteDto;

public interface RouteService {
    public OsrmRouteDto getRouteByWalking(String fromCoordinates, String toCoordinates);
}
