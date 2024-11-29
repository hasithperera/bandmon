# bandmon

This is a project intended to deploy a distributed band monitoring system to better quantify 2m amateur band usage.

## Dependencies 

- MQTT pub/sub library placed in side `~/Arduino/libraries/` 

```
git clone git@github.com:knolleary/pubsubclient.git
```
- other 

## Tools 

- [Mqtt_spy](https://github.com/eclipse/paho.mqtt-spy/releases/tag/) > 1.0.1-beta9. Running with jre 8.6 works


## To-do list

- [ ] Basic code for MQTT and monitor a single repeater
- [ ] Configurable setting screen
	- Change radio settings
- [ ] Server end logging and display 
