echo "COMPILE"
#Apaga compilações anterios
make -C /lib/modules/$(uname -r)/build M=`pwd` clean
#Compila o modulo
make -C /lib/modules/$(uname -r)/build M=`pwd` modules

