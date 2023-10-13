This is for RAK2245 LoRa concentrator hat. For the new RAK5146 LoRa concentrator hat, the `global_conf.json` files are in the `rak5146/global_conf` folder. You will then need to replace the `rak5146/global_conf.json` file by the one in `rak5146/global_conf` folder according to your frequency plan. Then the procedure is the same, except that you do not need to log into the docker container (steps 2, 3, 4, 8, 9) as the support of RAK5146 is currently out of any container. You just need to perform steps 1, 5, 6, 7, 10 and 11, and run `sudo ./start.sh` in `rak5146` folder.

RAK2245
-------

To use the wazigate as a LoRa packet forwarder to TTN for instance, you do as follows:

1. Get the MAC address of your wazigate
   look for instance on the wazigate dashboard/Settings for the WiFi SSID
   Ex:  WAZIGATE_B827EB36AAAA means that MAC address is B827EB36AAAA

2. Open a shell inside the waziup.wazigate-lora.forwarders container:
   docker exec -it waziup.wazigate-lora.forwarders bash

3. Install vim or nano:
   apt-get install nano
   
4. Edit the global-config.json:
   nano conf//multi_chan_pkt_fwd/global_conf.json

5. replace the default "gateway_ID": "AA55A00000000000" by your wazigate id with FFFE inserted
   Ex: "gateway_ID": "B827EBFFFE36AAAA"
   CTRL-0 + return then CTRL-X to exit nano

6. replace the default "server_address": "waziup.wazigate-lora.chirpstack-gateway-bridge" by
   "server_address": "eu1.cloud.thethings.network"
   or the server TTN server associated to your region
   eu1.cloud.thethings.network for Europe and most of Africa countries
   nam1.cloud.thethings.network for America continent
   au1.cloud.thethings.network for Middle-East, Australia and Asia continents 
      
7. CTRL-O + return then CTRL-X to exit nano   
     
8. Close the container shell (go back to the RPi shell)

9. Restart the forwarders container:
   docker restart waziup.wazigate-lora.forwarders

10. Register a new gateway in your TTN console with gateway eui B827EBFFFE36AAAA

11. Then, of course, you need to create associated devices on your TTN console using the default devaddr in ABP mode

To do so, use following guidelines;

```
Device EUI: leave blank
Device address: 26011DAA
Device id: devaddr-26011daa
Frequency plan: Europe 863-870 MHz (SF12 for RX2)
LoRaWAN version: LoRaWAN Specification 1.0.1
NwkSKey: the default one (see in source code)
AppSKey: the default one (see in source code)
Payload formatter type: CayenneLPP
```				

Example (RAK2245 `global_conf.json`):

```
{
  "SX1301_conf": {
		"lorawan_public": true,
		"clksrc": 1,
		"antenna_gain": 0,
		"radio_0": {
				"enable": true,
				"type": "SX1257",
				"freq": 867500000,
				"rssi_offset": -166.0,
				"tx_enable": true,
				"tx_freq_min": 863000000,
				"tx_freq_max": 870000000
		},
		"radio_1": {
				"enable": true,
				"type": "SX1257",
				"freq": 868500000,
				"rssi_offset": -166.0,
				"tx_enable": false
		},
		"chan_multiSF_0": {
				"enable": true,
				"radio": 1,
				"if": -400000
		},
		"chan_multiSF_1": {
				"enable": true,
				"radio": 1,
				"if": -200000
		},
		"chan_multiSF_2": {
				"enable": true,
				"radio": 1,
				"if": 0
		},
		"chan_multiSF_3": {
				"enable": true,
				"radio": 0,
				"if": -400000
		},
		"chan_multiSF_4": {
				"enable": true,
				"radio": 0,
				"if": -200000
		},
		"chan_multiSF_5": {
				"enable": true,
				"radio": 0,
				"if": 0
		},
		"chan_multiSF_6": {
				"enable": true,
				"radio": 0,
				"if": 200000
		},
		"chan_multiSF_7": {
				"enable": true,
				"radio": 0,
				"if": 400000
		},
		"chan_Lora_std": {
				"enable": true,
				"radio": 1,
				"if": -200000,
				"bandwidth": 250000,
				"spread_factor": 7
		},
		"chan_FSK": {
				"enable": true,
				"radio": 1,
				"if": 300000,
				"bandwidth": 125000,
				"datarate": 50000
		},
		"tx_lut_0": {
				"pa_gain": 0,
				"mix_gain": 8,
				"rf_power": -6,
				"dig_gain": 0
		},
		"tx_lut_1": {
				"pa_gain": 0,
				"mix_gain": 10,
				"rf_power": -3,
				"dig_gain": 0
		},
		"tx_lut_2": {
				"pa_gain": 0,
				"mix_gain": 12,
				"rf_power": 0,
				"dig_gain": 0
		},
		"tx_lut_3": {
				"pa_gain": 1,
				"mix_gain": 8,
				"rf_power": 3,
				"dig_gain": 0
		},
		"tx_lut_4": {
				"pa_gain": 1,
				"mix_gain": 10,
				"rf_power": 6,
				"dig_gain": 0
		},
		"tx_lut_5": {
				"pa_gain": 1,
				"mix_gain": 12,
				"rf_power": 10,
				"dig_gain": 0
		},
		"tx_lut_6": {
				"pa_gain": 1,
				"mix_gain": 13,
				"rf_power": 11,
				"dig_gain": 0
		},
		"tx_lut_7": {
				"pa_gain": 2,
				"mix_gain": 9,
				"rf_power": 12,
				"dig_gain": 0
		},
		"tx_lut_8": {
				"pa_gain": 1,
				"mix_gain": 15,
				"rf_power": 13,
				"dig_gain": 0
		},
		"tx_lut_9": {
				"pa_gain": 2,
				"mix_gain": 10,
				"rf_power": 14,
				"dig_gain": 0
		},
		"tx_lut_10": {
				"pa_gain": 2,
				"mix_gain": 11,
				"rf_power": 16,
				"dig_gain": 0
		},
		"tx_lut_11": {
				"pa_gain": 3,
				"mix_gain": 9,
				"rf_power": 20,
				"dig_gain": 0
		},
		"tx_lut_12": {
			"desc": "TX gain table, index 12",
			"pa_gain": 3,
			"mix_gain": 10,
			"rf_power": 23,
			"dig_gain": 0
		},
		"tx_lut_13": {
			"desc": "TX gain table, index 13",
			"pa_gain": 3,
			"mix_gain": 11,
			"rf_power": 25,
			"dig_gain": 0
		},
		"tx_lut_14": {
			"desc": "TX gain table, index 14",
			"pa_gain": 3,
			"mix_gain": 12,
			"rf_power": 26,
			"dig_gain": 0
		},
		"tx_lut_15": {
			"desc": "TX gain table, index 15",
			"pa_gain": 3,
			"mix_gain": 14,
			"rf_power": 27,
			"dig_gain": 0
		}
  },
	"gateway_conf": {
		"gateway_ID": "B827EBFFFE36AAAA",
		/* change with default server address/ports, or overwrite in local_conf.json */
		"server_address": "eu1.cloud.thethings.network",
		"serv_port_up": 1700,
		"serv_port_down": 1700,
		/* adjust the following parameters for your network */
		"keepalive_interval": 10,
		"stat_interval": 30,
		"push_timeout_ms": 100,
		/* forward only valid packets */
		"forward_crc_valid": true,
		"forward_crc_error": false,
		"forward_crc_disabled": false,
		/* gps enable */
		"gps": true,
		"gps_tty_path": "/dev/ttyAMA0",
		"fake_gps": false,
		"ref_latitude": 10,
		"ref_longitude": 20,
		"ref_altitude": -1,
		"autoquit_threshold": 6
	}	
}
```