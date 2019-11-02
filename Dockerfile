##
## Stage 1 - Compile golang code
##
ARG TAG=latest
FROM ubuntu:disco as build

# Create app directory
WORKDIR /usr/src/app

# Some basic utilities
RUN apt-get -y update && apt-get -y install cmake g++ libfreetype6-dev

# Bring in dockerize
COPY --from=propellerfactory/dockerize /dockerize /dockerize

# Bring in the source
COPY . ./

# Build the executable
RUN cmake . && cmake --build .

# Figure out the shared library dependencies
RUN /dockerize ./msdfgen

# Make an empty directory
RUN mkdir /empty



##
## Stage 2 - The deployment container
##
FROM scratch as deploy
WORKDIR /
COPY --from=build /empty /tmp/
COPY --from=build /usr/src/app/msdfgen /msdfgen
COPY --from=build /usr/src/app/libmsdfgen.a /libmsdfgen.a
COPY --from=build /usr/src/app/msdfgen.h /msdfgen.h
COPY --from=build /usr/src/app/core/*.h /core/
COPY --from=build /usr/src/app/libs /
COPY --from=build /usr/src/app/libs /libs/