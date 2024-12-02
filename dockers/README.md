# Docker configuration

Grafana docker deployment 

```
docker run -d -p 3000:3000 --name=grafana-2 \
  --user "$(id -u)" \
  --volume "$PWD/custom.ini:/etc/grafana/grafana.ini" \
  --volume "$PWD/data:/var/lib/grafana" \
  grafana/grafana-oss
``` 
Influxdb 2 deployment
```
docker run  --name influxdb2  --publish 8086:8086  --mount type=volume,source=influxdb2-data,target=/var/lib/influxdb2  --mount type=volume,source=influxdb2-config,target=/etc/influxdb2 influxdb:2
```
Telegraf deployment 

```
sudo docker run --name=telegraf-1 --net=container:influxdb2 -v /opt/telegraf/telegraf.conf:/etc/telegraf/telegraf.conf:ro telegraf
```

## General docker usage commands

To find internal docker routing Ip addresses you can use `docker network inspect bridge`
