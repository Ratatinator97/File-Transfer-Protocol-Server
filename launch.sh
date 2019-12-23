#!/bin/bash

cd ~/Documents/4TC-PRS/PRS_ProjetUDP-/

./serveur_client2 5050 > server_2000.log&

sleep 1
for i in {1..15}
do
   ./client2 127.0.0.1 5050 CV.pdf&
   sleep 10
done
pkill serveur_client2
echo TERMINE