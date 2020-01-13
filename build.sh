#!/bin/sh
docker build . -t propellerfactory/msdfgen:8aac4a02-f761-4957-912e-44fe1ca961bd -t propellerfactory/msdfgen:latest
docker push propellerfactory/msdfgen
