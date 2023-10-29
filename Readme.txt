El trabajo se basa en el setup x64BareBones disponible en https://bitbucket.org/RowDaBoat/x64barebones/

1- Instalar los siguientes paquetes antes de compilar el proyecto
  nasm qemu gcc make

2- Instalar Docker para la compilación y descargar la imagen a utilizar

3- Suponiendo que se descargó la imagen mediante el siguiente comando
  docker pull agodio/itba-so:1.0
  Crear un contenedor de Docker ejecutando la instrucción a continuación
  docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -ti --name tpearqui agodio/itba-so:1.0
  
4- Correr el script.sh desde la terminal o, en su defecto, los comandos contenidos en él
  ./script.sh

5- Finalmente, ejecutar el siguiente comando para correr el proyecto desde qemu
  ./run.sh