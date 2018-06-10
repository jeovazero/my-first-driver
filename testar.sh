echo  "[ Compilando ] \n"
sh compilar.sh
#################################################

echo  "[ Instalando ] \n"
sh instalar.sh

#################################################
echo  "[ Testando ] \n"
# ----------------------------------------------
echo  "(T1): echo -n 42531 > /dev/memory \n"
echo -n 42531 > /dev/memory

echo "cat /dev/memory"
cat /dev/memory

echo "\nFim (T1)\n"
# -----------------------------------------------
echo  "(T2): echo -n 2519 > /dev/memory \n"
echo -n 2519 > /dev/memory

echo "cat /dev/memory"
cat /dev/memory

echo "\nFim (T2)\n"
# -----------------------------------------------
echo  "(T3): echo -n 92183 > /dev/memory \n"
echo -n 92183 > /dev/memory

echo "cat /dev/memory"
cat /dev/memory

echo "\nFim (T3)\n"

echo "FIM DE TODOS OS TESTES \n"
