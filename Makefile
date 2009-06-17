CFLAGS= -w -O3 -funroll-loops -fno-rtti -fomit-frame-pointer -ffast-math -fno-stack-protector -ffunction-sections
LFLAGS=

all:
	g++ *.cpp -o kerbyd $(CFLAGS) $(LFLAGS)
install:
	#For testing purpose only, do not run this !
	scp -r ../kerby root@ganryu:/root
clean:
	rm -f *.o kerbyd
