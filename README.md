### Simple weather station based on Freescale FRDM-XFS-MULTI-B and STM32F429I-Disco. The Bluetooth 4.0 is using to communication.

#### Branches:
* **master**

  for now is empty

* **bluetooth\_sensors\_code**

 * implements connection between MCU and the bluetooth module via UART
 * implements connection between the MPL3115A2 sensor.
 >Note: This branch was used only to develop mentioned tasks and don't contain all required sources

* **lcd_code**

 implements the LCD driver to display measured and received by bluetooth weather data.
 > Note: At one point sources from branch: *bluetooth\_sensors\_code* were added so **currently this branch contain all valid sources**

* **linux_application**

 very, very simple linux application which parses output from _hcidump_ program. It is required to set up bluetooth module by _hcitool_ program before using this application.



#### About project:

This project was created for the subject at the university. The topic was: simple weather station based on the FRDM-FXS-MULTI-B board. This station should communicate with other devices like smartphones, tablets via bluetooth 4.0.
As brain we used STM32F492i-discovery board because it has the LCD on which were displayed measured weather data such as

* ambient temperature
* atmospheric pressure at sea level (in the source code can be found conversion for Krakow).
* humidity
* CO2

Our station also can retrieve advertised data from mobile device, these data are:

* current time
* weather description which is being downloaded from the internet.

We used bluetooth 4.0 module so was easy to share measured data with surroundings. To do that the advertising mode was used. Advertised data look something like that ( the space is just a visual separator ) :

**020106 0E FF8500FF 0AB3 0A0BC1 0129 20 030F**

 

- **020106**

 ```[3 bytes] constant ( the standard requires that )```

- **0E**

 ```[1 byte] how many bytes data will be sent```

- **FF8500FF**

 ```[4 bytes] constant```

- **0AB3**

 ```[2 bytes] simple ID -> to identify our station```

- **0A0BC1**

 ```[3 bytes] temperature: 0A - total part (signed) of temperature
    0BC1 16 bit unsigned value - represents the fractional part```

- **0129**

 ```[2 bytes] the atmospheric pressure```

- **20**

 ```[1 byte] humidity```

- **030F**
 
 ```[2 bytes] CO2 concentration```



The mobile application can also advertised data in format:

**1009 3034 31352D30342D3134 31343A3131**

Almost all of these numbers represent ASCII characters, so after conversion we get:

- **1009**

 ```[2 bytes, not ASCII] own ID to identify data that are designed to our weather station```

- **34 15-04-14 14:11**

 ` The first number: 34 represents description of weather`

 ` 15-04-14 current date`

 `14:11    current  time`

