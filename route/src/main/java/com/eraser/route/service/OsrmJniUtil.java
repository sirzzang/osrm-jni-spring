package com.eraser.route.service;

import com.eraser.route.service.dto.OsrmRouteDto;

public class OsrmJniUtil {

    // wrapper method for native
    public OsrmRouteDto returnOsrmRouteResponse(String fromCoordinates, String toCoordinates) {
        
        // split coordinates
        String[] fromCoordinatesStrings = fromCoordinates.split(",");
        double fromLongitude = Double.parseDouble(fromCoordinatesStrings[0]);
        double fromLatitude = Double.parseDouble(fromCoordinatesStrings[1]);

        String[] toCoordinateStrings = toCoordinates.split(",");
        double toLongitude = Double.parseDouble(toCoordinateStrings[0]);
        double toLatitude = Double.parseDouble(toCoordinateStrings[1]);

        // get response
        return getOsrmRouteResponse(fromLongitude, fromLatitude, toLongitude, toLatitude);

    }
    
    // native method
    private static native OsrmRouteDto getOsrmRouteResponse(double fromLongitude, double fromLatitude, double toLongitude, double toLatitude);

}
