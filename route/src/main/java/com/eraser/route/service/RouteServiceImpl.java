package com.eraser.route.service;

import com.eraser.route.service.dto.RouteResponseDto;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.eraser.route.service.dto.OsrmRouteDto;

@Service
public class RouteServiceImpl implements RouteService {

    @Autowired
    OsrmJniUtil osrmJniUtil;

    @Override
    public RouteResponseDto getRouteByWalking(String fromCoordinates, String toCoordinates) {

        OsrmRouteDto osrmRouteDto = osrmJniUtil.returnOsrmRouteResponse(fromCoordinates, toCoordinates);

        return RouteResponseDto.builder()
                .distance(osrmRouteDto.getDistance())
                .duration(osrmRouteDto.getDuration())
                .build();
    }
}
