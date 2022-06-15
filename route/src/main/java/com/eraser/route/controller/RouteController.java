package com.eraser.route.controller;

import com.eraser.route.service.dto.RouteResponseDto;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.eraser.route.service.RouteService;
import com.eraser.route.service.dto.OsrmRouteDto;

@RestController
public class RouteController {

    @Autowired
    RouteService routeService;
    
    @GetMapping("/route/walking")
    public RouteResponseDto getRouteByWalking(@RequestParam(required = true) String from, @RequestParam(required = true) String to) {

        return routeService.getRouteByWalking(from, to);

    }
}
