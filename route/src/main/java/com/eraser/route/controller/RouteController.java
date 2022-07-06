package com.eraser.route.controller;

import com.eraser.route.common.annotation.CoordinateValue;
import com.eraser.route.common.response.CommonResponse;
import com.eraser.route.service.dto.RouteResponseDto;
import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.eraser.route.service.RouteService;

import javax.validation.Valid;

@Validated
@RestController
@RequiredArgsConstructor
public class RouteController {

    private final RouteService routeService;
    
    @GetMapping("/route/walking")
    public ResponseEntity<CommonResponse<?>> getRouteByWalking(@RequestParam(required = true) @CoordinateValue String from,
                                                              @RequestParam(required = true) @CoordinateValue String to) {

        RouteResponseDto routeResponseDto = routeService.getRouteByWalking(from, to);

        // response body
        CommonResponse<RouteResponseDto> routeResponse;
        if (routeResponseDto.getDistance() == 0 || routeResponseDto.getDuration() == 0) {
            routeResponse = CommonResponse.onSuccess(
                    "R2040",
                    "Estimated time on foot derived, but needs check because of zero value.",
                    routeResponseDto
            );
        } else {
            routeResponse = CommonResponse.onSuccess(
                    "R2000",
                    "Estimated time on foot derived successfully.",
                    routeResponseDto
            );
        }

        return ResponseEntity.status(HttpStatus.OK).body(routeResponse);
    }
}
