package com.eraser.jniosrm;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/route")
public class Controller {
    
    private OsrmJNI jni = new OsrmJNI();

    @GetMapping("/walking")
    public int getDurationByWalking(@RequestParam(value = "from") String from,
                                    @RequestParam(value = "to") String to) {
                                        
        String[] fromCoordinates = from.split(",");
        double fromLon = Double.parseDouble(fromCoordinates[0]);
        double fromLat = Double.parseDouble(fromCoordinates[1]);

        String[] toCoordinates = to.split(",");
        double toLon = Double.parseDouble(toCoordinates[0]);
        double toLat = Double.parseDouble(toCoordinates[1]);

        int returnedDuration = jni.returnOsrmRoute(fromLon, fromLat, toLon, toLat);
        

        return returnedDuration;
    }

}
