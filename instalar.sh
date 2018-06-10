#!/bin/sh
modulo="my_memory_device"
dispositivo="memory"
modo="0666"

echo "[REMOVENDO OBSOLETO]"
rmmod $modulo.ko || echo "SEM OBSOLETO"

# inserindo o modulo no kernel
echo "[INSERINDO]: insmod ./$modulo.ko \n"
insmod ./$modulo.ko || (echo "FALHA \n" && exit 1)

# removendo nós obsoleto
echo "[REMOVENDO]: rm -f /dev/$dispositivo \n"
rm -f /dev/$dispositivo || (echo "Nada removido, porque /dev/$dispositivo nao existe \n")

# Cria o arquivo especial de caractere
major=$(cat /proc/devices | awk '{ if($2 == "memory"){ print $1 } }')
echo "MAJOR capturado: $major \n"
echo "[CRIANDO]: mknod /dev/$dispositivo --mode=$modo c $major 0  \n"
mknod /dev/$dispositivo --mode=$modo c $major 0
