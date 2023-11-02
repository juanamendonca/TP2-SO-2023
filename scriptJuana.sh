#!/bin/bash

# Nombre del contenedor Docker
container_name="recursing_poitras"

# Iniciar el contenedor Docker
docker start $container_name

# Ejecutar el comando 'make clean' en el directorio /root/Toolchain dentro del contenedor
docker exec -it $container_name make clean -C /root/Toolchain

# Ejecutar el comando 'make clean' en el directorio /root/ dentro del contenedor
docker exec -it $container_name make clean -C /root/

# Ejecutar el comando 'make' en el directorio /root/Toolchain dentro del contenedor
docker exec -it $container_name make -C /root/Toolchain

# Ejecutar el comando 'make' en el directorio /root/ dentro del contenedor
docker exec -it $container_name make -C /root/

# Detener el contenedor Docker
docker stop $container_name