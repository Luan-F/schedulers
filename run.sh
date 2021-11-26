# Script feito para testar os algoritmos
# Para usá-lo basta executar sh run.sh <nome do programa compilado>
# Todos os resultados ficam num arquivo com extensão .output
# * É necessário que o nome do programa compilado termine com .out
#
# Exemplo:
# 	Se temos um programa a.out usaremos
#	sh run.sh a

cenarios=(
	cenario1.txt
	cenario2.txt
	cenario3.txt
	cenario4.txt
	cenario5.txt
)

processos=(
	1000
	5000
	10000
	20000
	50000
)

rm -f $1.output
touch $1.output
for i in {0..4}
do
	echo "Cenário $i" >> $1.output
	./$1.out ${cenarios[i]} ${processos[i]} >> $1.output
	echo "------------------------------------------------" >> $1.output
done
