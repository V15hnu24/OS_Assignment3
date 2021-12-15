all: fifo_p1 fifo_p2 msg_p1 msg_p2 soc_p1 soc_p2

fifo_p1: fifo_p1.c
	gcc fifo_p1.c -o fifo_p1

fifo_p2: fifo_p2.c
	gcc fifo_p2.c -o fifo_p2

msg_p1: P1.c
	gcc P1.c -o P1

msg_p2: P2.c
	gcc P2.c -o P2

soc_p1: soc_p1.c
	gcc soc_p1.c -o soc_p1

soc_p2: soc_p2.c
	gcc soc_p2.c -o soc_p2