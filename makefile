program: 18050111021.c
	mpicc -o program 18050111021.c

run_one:
	OMPI_MCA_btl_vader_single_copy_mechanism=none mpirun -np 1 ./program 1021 output.txt

run_two:
	OMPI_MCA_btl_vader_single_copy_mechanism=none mpirun -np 2 ./program 1021 output.txt

run_three:
	OMPI_MCA_btl_vader_single_copy_mechanism=none mpirun -np 3 ./program 1021 output.txt

run_four:
	OMPI_MCA_btl_vader_single_copy_mechanism=none mpirun -np 4 ./program 1021 output.txt


complete:
	echo "task completed"
# It runs without warning
#OMPI_MCA_btl_vader_single_copy_mechanism=none mpirun -np 1 ./program 1021 output.txt

clean:
	rm -f *.o
	rm -f program

.PHONY: clean
.PHONY: run_one
.PHONY: run_two
.PHONY: run_three
.PHONY: run_four
.PHONY: run_five
.PHONY: complete
