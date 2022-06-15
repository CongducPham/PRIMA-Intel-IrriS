#!/bin/bash

docker exec -it waziup.wazigate-lora.forwarders grep freq conf/single_chan_pkt_fwd/global_conf.json
docker exec -it waziup.wazigate-lora.chirpstack-network-server grep 'name="' /etc/chirpstack-network-server/chirpstack-network-server.toml
docker exec -it waziup.wazigate-lora.chirpstack-gateway-bridge grep -E 'region="E|frequency_'  /etc/chirpstack-gateway-bridge/chirpstack-gateway-bridge.toml
