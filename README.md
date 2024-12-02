# bandmon

This is a project intended to deploy a distributed band monitoring system to better quantify 2m repeater usage. 

The goal was to implement a system that can monitor a repeater audio output and detect activity and generate a summary to evaluate activity. 

funding provided by the WVU Amateur radio club [W8CUL](https://wvuarc.orgs.wvu.edu)
## Dependencies 

- Arduino board manager: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
- MQTT pub/sub library placed in side `~/Arduino/libraries/` 

```
git clone git@github.com:knolleary/pubsubclient.git
```
- this can also be added using the library manager
- more info[link](https://pubsubclient.knolleary.net)

## Tools 

- [Mqtt_spy](https://github.com/eclipse/paho.mqtt-spy/releases/) > 1.0.1-beta9. Running with jre 8.6 works well in linux/Windows. 
	- MAC still has trouble running it correctly
- [esp_tools](https://github.com/espressif/esptool) - For installation and firmware update

## To-do list

- [x] Basic code for Wifi and MQTT connection
- [x] Send repeater activity based on audio input
 - [x] EEPROM data save
- [ ] Configurable settings TUI
- [ ] Hardware design 
    - [ ] final PCB design - audio based
    - [ ] 3D printable case design
- [x] Server end logging
	- [x] Display with Grafana
	- [x] Deploy in the ARC server
- [ ] Write an article about it 
    - [ ] reach out to other local clubs
