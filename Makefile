all: des reciver
des: des.c
	gcc des.c -o des
reciver: reciver.c
	gcc reciver.c -o reciver
