package com.eraser.route.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.eraser.route.service.dto.OsrmRouteDto;

@Service
public class RouteServiceImpl implements RouteService {

    @Autowired
    OsrmJniUtil osrmJniUtil;

    @Override
    public OsrmRouteDto getRouteByWalking(String fromCoordinates, String toCoordinates) {

        return osrmJniUtil.returnOsrmRouteResponse(fromCoordinates, toCoordinates);
    
    }


    
}
