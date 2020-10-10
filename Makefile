all: clear compilerun clear

compilerun:
	@ gcc *.c -o f
	./f

clear:
	@ -rm -f
