#!/bin/sh
docker build . -t propellerfactory/msdfgen:8c1a6bc5-620d-4e11-84aa-a02adce9e9e4 -t propellerfactory/msdfgen:latest
docker push propellerfactory/msdfgen
