package com.eraser.route.controller;

import com.eraser.route.common.response.CommonResponse;
import com.eraser.route.service.dto.RouteResponseDto;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.eraser.route.service.RouteService;

@RestController
public class RouteController {

    @Autowired
    RouteService routeService;
    
    @GetMapping("/route/walking")
    public ResponseEntity<CommonResponse<?>> getRouteByWalking(@RequestParam(required = true) String from,
                                                              @RequestParam(required = true) String to) {

        RouteResponseDto routeResponseDto = routeService.getRouteByWalking(from, to);

        // response body
        CommonResponse<RouteResponseDto> routeResponse = CommonResponse.onSuccess(
                "R2000",
                "Estimated time on foot derived successfully.",
                routeResponseDto
        );

        return ResponseEntity.status(HttpStatus.OK).body(routeResponse);
    }
}
