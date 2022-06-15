#!/bin/bash

echo "copy $1/global_conf.json to waziup.wazigate-lora.forwarders:/root/conf/single_chan_pkt_fwd/"
docker cp /home/pi/scripts/single_chan_pkt_fwd/$1/global_conf.json waziup.wazigate-lora.forwarders:/root/conf/single_chan_pkt_fwd/

echo "copy $1/chirpstack-network-server.toml waziup.wazigate-lora.chirpstack-network-server:/etc/chirpstack-network-server/"
docker cp /home/pi/scripts/chirpstack_conf/$1/chirpstack-network-server.toml waziup.wazigate-lora.chirpstack-network-server:/etc/chirpstack-network-server/

echo "copy $1/chirpstack-gateway-bridge.toml waziup.wazigate-lora.chirpstack-gateway-bridge:/etc/chirpstack-gateway-bridge/"
docker cp /home/pi/scripts/chirpstack_conf/$1/chirpstack-gateway-bridge.toml waziup.wazigate-lora.chirpstack-gateway-bridge:/etc/chirpstack-gateway-bridge/