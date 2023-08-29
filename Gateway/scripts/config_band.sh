#!/bin/bash

echo "copy single_chan_pkt_fwd/${1^^}/global_conf.json to waziup.wazigate-lora.forwarders:/root/conf/single_chan_pkt_fwd/"
docker cp /home/pi/scripts/single_chan_pkt_fwd/${1^^}/global_conf.json waziup.wazigate-lora.forwarders:/root/conf/single_chan_pkt_fwd/
docker exec -it --user root waziup.wazigate-lora.forwarders chown root:root /root/conf/single_chan_pkt_fwd/global_conf.json

echo "RAK2245"
echo "copy multi_chan_pkt_fwd/${1^^}/global_conf.json to waziup.wazigate-lora.forwarders:/root/conf/multi_chan_pkt_fwd/"
docker cp /home/pi/scripts/multi_chan_pkt_fwd/${1^^}/global_conf.json waziup.wazigate-lora.forwarders:/root/conf/multi_chan_pkt_fwd/
docker exec -it --user root waziup.wazigate-lora.forwarders chown root:root /root/conf/multi_chan_pkt_fwd/global_conf.json

echo "RAK5146"
echo "copy rak5146/global_conf/global_conf.${1^^}.json to rak5146 home folder"
cp /home/pi/scripts/rak5146/global_conf/global_conf.${1^^}.json /home/pi/scripts/rak5146/global_conf.json 
chown root:root /home/pi/scripts/rak5146/global_conf.json

echo "configuring chirpstack-gateway-bridge.toml for ${1^^}"
sed -i 's/region=.*/region="'"${1^^}"'"/g' /home/pi/scripts/chirpstack_conf/chirpstack-gateway-bridge.toml
echo "copy chirpstack-gateway-bridge.toml to waziup.wazigate-lora.chirpstack-gateway-bridge:/etc/chirpstack-gateway-bridge/"
docker cp /home/pi/scripts/chirpstack_conf/chirpstack-gateway-bridge.toml waziup.wazigate-lora.chirpstack-gateway-bridge:/etc/chirpstack-gateway-bridge/

echo "configuring chirpstack-network-server.toml for ${1^^}"
sed -i 's/^name=.*/name="'"${1^^}"'"/g' /home/pi/scripts/chirpstack_conf/chirpstack-network-server.toml
echo "copy chirpstack-network-server.toml to waziup.wazigate-lora.chirpstack-network-server:/etc/chirpstack-network-server/"
docker cp /home/pi/scripts/chirpstack_conf/chirpstack-network-server.toml waziup.wazigate-lora.chirpstack-network-server:/etc/chirpstack-network-server/