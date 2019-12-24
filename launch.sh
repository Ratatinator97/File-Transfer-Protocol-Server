#!/bin/bash
var=10
cd ~/Documents/4TC-PRS/PRS_ProjetUDP-/

for i in {0..20}
do
    var=$[1000+$(($i*1000))]
    echo $var
    ./serveur_client2 5050 $var > "serverSecondarytest_"$var".log"&
    sleep 1
    for j in {1..30}
    do
        ./client2 127.0.0.1 5050 truc.png&
        sleep 8
    done
    pkill serveur_client2
done

echo TERMINEe