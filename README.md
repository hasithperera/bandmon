# bandmon

This is a project intended to deploy a distributed band monitoring system to better quantify 2m amateur band usage.

## Dependencies 

- Arduino board manager: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`

- MQTT pub/sub library placed in side `~/Arduino/libraries/` 

```
git clone git@github.com:knolleary/pubsubclient.git
```
- this can also be added using the library manager
- more info[link](https://pubsubclient.knolleary.net)

## Tools 

- [Mqtt_spy](https://github.com/eclipse/paho.mqtt-spy/releases/tag/) > 1.0.1-beta9. Running with jre 8.6 works


## To-do list

- [ ] Basic code for Wifi and MQTT connection
- [ ] Send repeater data
 - [x] EEPROM data read
- [ ] 
- [ ] Configurable setting screen
	- Change radio settings
- [ ] Server end logging and display 
