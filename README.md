## Install platform IO cli

`export PATH=$PATH:~/.platformio/penv/bin`

```cpp
printf("Restarting now.\n");
fflush(stdout);
esp_restart();
```

`IDF_PATH=~/.platformio/packages/framework-espidf/`

You may have to add pyserial:
`sudo pip3 install pyserial`

Read device info:
`python3 $IDF_PATH/components/esptool_py/esptool/esptool.py --port /dev/cu.usbserial-A5XK3RJT flash_id`

Read partition table

`python3 $IDF_PATH/components/esptool_py/esptool/esptool.py --port /dev/cu.usbserial-A5XK3RJT read_flash 0x8000 0xc00 ptable.img`

We can now check the content of the `ptable.img` file
`python3 $IDF_PATH/components/partition_table/gen_esp32part.py ptable.img`

Here is an example of the result:

```bash
# Espressif ESP32 Partition Table
# Name, Type, SubType, Offset, Size, Flags
nvs,data,nvs,0x9000,20K,
otadata,data,ota,0xe000,8K,
app0,app,ota_0,0x10000,1280K,
app1,app,ota_1,0x150000,1280K,
spiffs,data,spiffs,0x290000,1472K,
```

## FS

Create a folder at the first level called `data`.

To upload the data

`pio run --target uploadfs`

## Alternative web server

https://github.com/me-no-dev/ESPAsyncWebServer#using-platformio

## OTA

pio run -t upload --upload-port square.local
pio run -t uploadfs --upload-port square.local

## Test webserver

```bash
npm i --global loadtest
loadtest -n 1000 -c 4 http://square.patiny.com/command/A
```

## Setup

Z: Layout model
AA: 0 (RGB), 1 (BRG)

Cubes

I: 1 (one line)
J: 10 (10 groups of 3 leds per line)

## Flame simulation

https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-candle/
