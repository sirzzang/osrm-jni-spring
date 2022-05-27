package com.eraser.jniosrm;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/route")
public class Controller {
    
    private OsrmJNI jni = new OsrmJNI();

    @GetMapping("/walking")
    public int main() {
        int returnedMain = jni.returnMain();
        return returnedMain; 
    }
}
