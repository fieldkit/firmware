#!/bin/bash

TOKEN=`fk-local.sh /user/transmission-token | jq -r ".token"`

echo $TOKEN

fkc --address 192.168.0.209 --transmission-url http://192.168.0.100:8080/ingestion --transmission-token "$TOKEN"
