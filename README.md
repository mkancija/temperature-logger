# temperature-logger

simple temperature logger using 18b20 and dht22 sensors.
Logger will backup all data to SD card and then try to send data over local network or send cached data to bluetooth reciever if requested.

## SD Card

If present and intialized logger will use SD card to store lines of cached data.

## Bluettoth

When connected, all cached data will be transfered to bluetooth reciever.
Logger keeps last line and date of stored data.
(It is up to the client app to organize data according to this information)

## Network

Using simple network GET method, logger will try to send data to API endpoint.
API endpoint will identify logger module using sent JWT token and store snet data.

# logger web frontend

# Logger PHP API Endpoint

