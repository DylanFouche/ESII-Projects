# ESII-Projects
Mini projects for the Embedded Systems II course.

## Install Mosquitto
```bash
$ sudo apt-get install mosquitto
$ sudo apt-get install mosquitto-clients
$ sudo apt-get install libmosquitto-dev libmosquittopp-dev
```

## Compile
```bash
$ make
```

## Run
```bash
$ make run
```


## Notes
The following mqtt broker was used:
  test.mosquitto.org ; port 1883.
 
To subscribe to the test broker:
```bash
$ mosquitto_sub -h test.mosquitto.org -t "projb/sensors/#" -v
```

## Clean
```bash
$ make clean   //remove object files
$ make nuke    //remove object files and executables
```

## File structure
  src:      source code cpp files  
  include:  header files   
  build:    object files  
  bin:      executables 
