package com.eraser.route.service;

import com.eraser.route.service.dto.RouteResponseDto;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import com.eraser.route.service.dto.OsrmRouteDto;

@Service
@RequiredArgsConstructor
public class RouteServiceImpl implements RouteService {

    private final OsrmJniUtil osrmJniUtil;

    @Override
    public RouteResponseDto getRouteByWalking(String fromCoordinates, String toCoordinates) {

        OsrmRouteDto osrmRouteDto = osrmJniUtil.returnOsrmRouteResponse(fromCoordinates, toCoordinates);

        return RouteResponseDto.builder()
                .distance(osrmRouteDto.getDistance())
                .duration(osrmRouteDto.getDuration())
                .build();
    }
}
