# Description: This script reads the pressure data from the Zephyr device and publishes it to the MQTT broker
# (without notification enabling)


mac_addr=$(grep -i "Zephyr" mac.txt | cut -d ' ' -f 1 | head -n 1)
if [ -z "$mac_addr" ]; then
    echo "No Zephyr device found"
    exit 1
fi


gatttool -b $mac_addr --characteristics -t random 
manufacturer_name=$(gatttool -b $mac_addr --char-read --uuid 00002a29-0000-1000-8000-00805f9b34fb -t random | cut -d ':' -f 3)
echo $manufacturer_name
echo $manufacturer_name | xxd -r -p && echo ''


while true
do
    pkill -f 2.sh
    echo "killed 2.sh"


    #pressure
    pressure=$(gatttool -b $mac_addr --char-read --uuid 00002a19-0000-1000-8000-00805f9b34fb -t random | cut -d ':' -f 3)
    gatttool -b $mac_addr --char-read --uuid 00002a19-0000-1000-8000-00805f9b34fb -t random

    pressure_no_space=$(echo "$pressure" | tr -d '[:blank:]')
    pressure_dec=$((0x$pressure_no_space)) 

    echo $pressure_dec 
    mosquitto_pub -h localhost -t pressure -m $pressure_dec

        #temp
    temperature=$(tail -1 temperature.txt | cut -d ' ' -f 7)
    temperature_no_space=$(echo "$temperature" | tr -d '[:blank:]')
    #check if is a number
    if ! [[ $temperature_no_space =~ ^[0-9a-fA-F]+$ ]]; then
        echo "Temperature is not a number"
        sleep 5s
        continue
    fi
    temperature_dec=$((0x$temperature_no_space))

    echo $temperature_dec
    mosquitto_pub -h localhost -t temperature -m $temperature_dec


    sleep 5s
done