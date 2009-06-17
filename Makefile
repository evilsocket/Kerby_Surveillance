CFLAGS= -Wunused -O3 -funroll-loops -fno-rtti -fomit-frame-pointer -ffast-math -fno-stack-protector -ffunction-sections
LFLAGS= -lpthread -lX11 

all:
	g++ *.cpp -o kerbyd $(CFLAGS) $(LFLAGS)
clean:
	rm -f *.o kerbyd
