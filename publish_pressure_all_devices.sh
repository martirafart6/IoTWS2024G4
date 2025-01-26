# Description: This script reads the pressure data from the Zephyr device and publishes it to the MQTT broker
# (without notification enabling)


mac_addr_1=$(grep -i "Zephyr_1" mac.txt | cut -d ' ' -f 1 | head -n 1)
if [ -z "$mac_addr_1" ]; then
    echo "No Zephyr device found"

fi

mac_addr_2=$(grep -i "Zephyr_2" mac.txt | cut -d ' ' -f 1 | head -n 1)
if [ -z "$mac_addr_2" ]; then
    echo "No Zephyr device found"

fi

mac_addr_3=$(grep -i "Zephyr_3" mac.txt | cut -d ' ' -f 1 | head -n 1)
if [ -z "$mac_addr_3" ]; then
    echo "No Zephyr device found"

fi

declare -a mac=($mac_addr_1 $mac_addr_2 $mac_addr_3)

for i in $mac_addr_1 $mac_addr_2 $mac_addr_3
do 
    echo $i
    #gatttool -b $i --characteristics -t random 
    manufacturer_name=$(gatttool -b $i--char-read --uuid 00002a29-0000-1000-8000-00805f9b34fb -t random | cut -d ':' -f 3)
    echo $manufacturer_name
    echo $manufacturer_name | xxd -r -p && echo ''
done

while true
do

    for i in $mac_addr_1 $mac_addr_2 $mac_addr_3
    do
        #pressure
        pressure=$(gatttool -b $i --char-read --uuid 00002a19-0000-1000-8000-00805f9b34fb -t random | cut -d ':' -f 3)
        gatttool -b $i --char-read --uuid 00002a19-0000-1000-8000-00805f9b34fb -t random

        pressure_no_space=$(echo "$pressure" | tr -d '[:blank:]')
        pressure_dec=$((0x$pressure_no_space)) 

        echo $pressure_dec 
        if [ $i == $mac_addr_1 ]; then
            mosquitto_pub -h localhost -t Zephyr_1/pressure -m $pressure_dec
        elif [ $i == $mac_addr_2 ]; then
            mosquitto_pub -h localhost -t Zephyr_2/pressure -m $pressure_dec
        elif [ $i == $mac_addr_3 ]; then
            mosquitto_pub -h localhost -t Zephyr_3/pressure -m $pressure_dec
        fi

    done

    sleep 5s
done