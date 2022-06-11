package com.eraser.route.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.eraser.route.service.OsrmJniUtil;

@Configuration
public class RouteAppConfig {
    
    @Bean
    public OsrmJniUtil osrmJniUtil() {
        return new OsrmJniUtil();
    }

}
