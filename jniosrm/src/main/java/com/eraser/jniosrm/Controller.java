package com.eraser.jniosrm;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class Controller {

    private OsrmJNI jni = new OsrmJNI();

    @GetMapping("/route")
    public OsrmResponse getDurationByWalking(@RequestParam(value = "from") String from,
            @RequestParam(value = "to") String to) {

        String[] fromCoordinates = from.split(",");
        double fromLon = Double.parseDouble(fromCoordinates[0]);
        double fromLat = Double.parseDouble(fromCoordinates[1]);

        String[] toCoordinates = to.split(",");
        double toLon = Double.parseDouble(toCoordinates[0]);
        double toLat = Double.parseDouble(toCoordinates[1]);

        OsrmResponse returnedResponse = jni.returnOsrmResponse(fromLon, fromLat, toLon, toLat);
        System.out.println("returned to spring controller: " + returnedResponse);

        return returnedResponse;
    }

    @GetMapping("/osrm-pointer")
    public long OsrmPointer() {
        return jni.returnOsrmPointer();
    }

}
