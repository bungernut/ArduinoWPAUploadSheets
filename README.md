# ArduinoWPAUploadSheets

Got sheets working with the script:  
https://create.arduino.cc/projecthub/24Ishan/log-temperature-data-to-google-sheets-0b189b  
https://script.google.com/d/1zwxepnNveuW3bVCYewSnyGh0PAIBFxz-YQArSx-AQgxu38fhasaHRHRC/edit?usp=sharing

Nano 33 IoT uses WiFiNINA libs to connect to WiFi.  
Example `ConnectWithWPA` is a good start with this, said needed firmware update, but still managed to connect.   

Running the WiFiNINA Firmware Check results in. Again, lets not bother to do an update unless we can't make this work...
```
20:49:57.122 -> WiFiNINA firmware check.
20:49:57.122 -> 
20:49:57.875 -> Firmware version installed: 1.2.3
20:49:57.875 -> Latest firmware version available : 1.4.1
20:49:57.875 -> 
20:49:57.875 -> Check result: NOT PASSED
20:49:57.875 ->  - The firmware version on the module do not match the
20:49:57.875 ->    version required by the library, you may experience
20:49:57.875 ->    issues or failures.
```

