obj = main.o md4.o

main:$(obj)
	gcc -o main $(obj)

%.o:%.c
	gcc -c $<

clean:
	rm *.o
	rm main