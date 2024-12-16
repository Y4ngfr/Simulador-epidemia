all:
	@ gcc epidemia.c main.c -g -o simulacao -lm

delete:
	@ rm -r output_dot/*
	@ rm -r output_svg/*
	@ rm -r plot/*
	@ rm -r graficos/*
	@ rm -r csv_out/*