#!/bin/bash
cd ~/Documents/4TC-PRS/PRS_ProjetUDP-/

sleep 1
./serveur_client2 9001  > "Scenario1.log"&
sleep 1
for j in {1..10}
do
    ./client1 192.168.43.32 9001 trash.xlsx 0
    sleep 100
done
echo "Phase 1 OVER"
pkill serveur_client2

./serveur_client2 9002  > "Scenario2.log"&
sleep 10

for j in {1..10}
do
    ./client2 192.168.43.32 9002 trash.xlsx 0
    sleep 250
done
pkill serveur_client2
echo "Phase 2 OVER"
./serveur_client2 9003  > "Scenario3.log"&
sleep 10

for j in {1..10}
do
    ./client2 192.168.43.32 9003 trash.xlsx 0
done
pkill serveur_client2

echo TERMINE