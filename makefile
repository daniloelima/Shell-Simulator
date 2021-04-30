#Trabalho realizado pelos alunos Danilo Erler Lima, Enzo Baioco Cussuol e Vitor Berger Bonella
#Para utilizar o programa basta abrir o terminal na pasta do projeto e digitar "make"

compileandrun:
	gcc *.c -o runvsh
	./runvsh

clear:
	rm -rf runvsh