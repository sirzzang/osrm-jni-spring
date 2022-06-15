package com.eraser.route;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class RouteApplication {

	static {
		// System.loadLibrary("libosrmjniroute.so");
		System.load("/home/eraser/projects/osrm-jni-spring/route/src/native/libosrmjniroute.so");
	}

	public static void main(String[] args) {
		SpringApplication.run(RouteApplication.class, args);
	}

}
