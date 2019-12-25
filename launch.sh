#!/bin/bash
var=10
cd ~/Documents/4TC-PRS/PRS_ProjetUDP-/


for i in {0..25}
do
    var=$[1000+$(($i*1000))]
    echo $var
    sleep 1
    ./serveur_client2RTT 9001 $var > "Client2_"$var".log"&
    sleep 1
    for j in {1..10}
    do
        ./client2 192.168.43.32 9001 truc.png
        sleep 4
    done
    pkill serveur_client2
done
sleep 10

for i in {0..25}
do
    var=$[1000+$(($i*1000))]
    echo $var
    sleep 1
    ./serveur_client2noRTT 9001 $var > "Client2_noRTT_"$var".log"&
    sleep 1
    for j in {1..10}
    do
        ./client2 192.168.43.32 9001 truc.png
        sleep 4
    done
    pkill serveur_client2
done

echo TERMINE