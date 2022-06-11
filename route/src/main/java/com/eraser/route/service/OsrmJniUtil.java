package com.eraser.route.service;

import com.eraser.route.service.dto.OsrmRouteDto;

public class OsrmJniUtil {
    
    private native OsrmRouteDto getOsrmRouteResponse(double fromLongitude, double fromLatitude, double toLongitude, double toLatitude);

    // load native
    static {
        System.load("libosrmjniroute");
    }

}
