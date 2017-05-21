MPI = mpicc
CC = gcc
FLAGS = -g
PTHREAD = -lpthread

UTILS = utils.h

GREETING_I = Greeting/greeting.c
GREETING_O = Greeting/gree_run

TRAPECIO_I = Trapecio/trapecio.c
TRAPECIO_O = Trapecio/trap_run

OUTPUT_I = Output/mpi_output.c
OUTPUT_O = Output/out_run

VECSCALAR_I = VectorScalar/vector_scalar.c
VECSCALAR_O = VectorScalar/vs_run

PSUMSERIAL_I = PrefixSums/serial_psum.c
PSUMSERIAL_O = PrefixSums/sps_run

PSUMPARALLEL_I = PrefixSums/parallel_psum.c
PSUMPARALLEL_O = PrefixSums/pps_run

PSUMSCAN_I = PrefixSums/scan_psum.c
PSUMSCAN_O = PrefixSums/scps_run

RINGPASS_I = RingPass/ring_pass.c
RINGPASS_O = RingPass/rp_run

GSVECTOR_I = GSV/gs_vector.c
GSVECTOR_O = GSV/gs_run

_314_I = 3_14/3_14.c
_314_O = 3_14/3_14_run

TYPECONT_I = TypeCont/type_cont.c
TYPECONT_O = TypeCont/tc_run

TYPEVEC_I = TypeVec/type_vec.c
TYPEVEC_O = TypeVec/tv

TYPEINDEX_I = TypeIndexed/type_indexed.c
TYPEINDEX_O = TypeIndexed/ti_run

TRAPECIOPU_I = TrapecioPackUnpack/trap_pu.c
TRAPECIOPU_O = TrapecioPackUnpack/tpu_run

ODDEVEN_I = OddEven/odd_even_serial.c
ODDEVEN_O = OddEven/oes_run

ODDEVENP_I = OddEven/odd_even_parallel.c
ODDEVENP_O = OddEven/oep_run

MATRIXVECTOR_I = MatrixVector/matrix_x_vector.c
MATRIXVECTOR_O = MatrixVector/mv_run

PI_I = PI/pi.c
PI_O = PI/pi_run

PC_I = ProducerConsumer/pc.c
PC_O = ProducerConsumer/pc_run

BARRIER_I = Barrier/barrier.c
BARRIER_O = Barrier/b_run

LINKEDLIST_I = LinkedList/linked_list.c
LINKEDLIST_O = LinkedList/ll_run

STRTOCK_I = Strtock/strtock.c
STRTOCK_O = Strtock/str_run

all:$(GREETING_I) $(TRAPECIO_I)
	$(MPI) $(FLAGS) $(GREETING_I) -o $(GREETING_O)
	$(MPI) $(FLAGS) $(TRAPECIO_I) -o $(TRAPECIO_O)
	$(MPI)	$(FLAGS) $(OUTPUT_I) -o $(OUTPUT_O)
	$(MPI)	$(FLAGS) $(VECSCALAR_I) -o $(VECSCALAR_O)
	$(CC) $(FLAGS) $(PSUMSERIAL_I) -o $(PSUMSERIAL_O)
	$(MPI)	$(FLAGS) $(PSUMPARALLEL_I) -o $(PSUMPARALLEL_O)	
	$(MPI)	$(FLAGS) $(PSUMSCAN_I) -o $(PSUMSCAN_O)	
	$(MPI)	$(FLAGS) $(RINGPASS_I) -o $(RINGPASS_O)
	$(MPI)	$(FLAGS) $(GSVECTOR_I) -o $(GSVECTOR_O)	
	$(CC)	$(FLAGS) $(_314_I) -o $(_314_O)	
	$(MPI)	$(FLAGS) $(TYPECONT_I) -o $(TYPECONT_O)	
	$(MPI)	$(FLAGS) $(TYPEVEC_I) -o $(TYPEVEC_O)	
	$(MPI)	$(FLAGS) $(TYPEINDEX_I) -o $(TYPEINDEX_O)
	$(MPI)	$(FLAGS) $(TRAPECIOPU_I) -o $(TRAPECIOPU_O)	
	$(CC)	$(FLAGS) $(ODDEVEN_I) -o $(ODDEVEN_O)	
	$(MPI)	$(FLAGS) $(ODDEVENP_I) -o $(ODDEVENP_O)	
	$(CC)	$(FLAGS) $(MATRIXVECTOR_I) 	-o $(MATRIXVECTOR_O) $(PTHREAD)
	$(CC) $(FLAGS) $(PI_I) -o $(PI_O) $(PTHREAD)
	$(CC) $(FLAGS)	$(PC_I) -o $(PC_O) $(PTHREAD)
	$(CC) $(FLAGS) $(BARRIER_I)	-o $(BARRIER_O) $(PTHREAD)
	$(CC) $(FLAGS) -O2 $(LINKEDLIST_I) -o $(LINKEDLIST_O) $(PTHREAD)	
	$(CC) $(FLAGS) $(STRTOCK_I) -o $(STRTOCK_O)	 $(PTHREAD)

$(GREETING_O):$(GREETING_I)
	$(MPI) $(FLAGS) $(GREETING_I) -o $(GREETING_O)

$(TRAPECIO_O):$(TRAPECIO_I)
	$(MPI) $(FLAGS) $(TRAPECIO_I) -o $(TRAPECIO_O)

$(OUTPUT_O):$(OUTPUT_I)
	$(MPI)	$(FLAGS) $(OUTPUT_I) -o $(OUTPUT_O)

$(VECSCALAR_O):$(VECSCALAR_I)
	$(MPI)	$(FLAGS) $(VECSCALAR_I) -o $(VECSCALAR_O)

$(PSUMSERIAL_O):$(PSUMSERIAL_I)
	$(CC) $(FLAGS) $(PSUMSERIAL_I) -o $(PSUMSERIAL_O)

$(PSUMPARALLEL_O):&(PSUMPARALLEL_I)
	$(MPI)	$(FLAGS) $(PSUMPARALLEL_I) -o $(PSUMPARALLEL_O)	

$(PSUMSCAN_O):&(PSUMSCAN_I)
	$(MPI)	$(FLAGS) $(PSUMSCAN_I) -o $(PSUMSCAN_O)	

$(RINGPASS_O):$(RINGPASS_I)
	$(MPI)	$(FLAGS) $(RINGPASS_I) -o $(RINGPASS_O)


$(GSVECTOR_O):$(GSVECTOR_I)
	$(MPI)	$(FLAGS) $(GSVECTOR_I) -o $(GSVECTOR_O)	

$(_314_O):$(_314_I)
	$(CC)	$(FLAGS) $(_314_I) -o $(_314_O)	

$(TYPECONT_O):$(TYPECONT_I)
	$(MPI)	$(FLAGS) $(TYPECONT_I) -o $(TYPECONT_O)	

$(TYPEVEC_O):$(TYPECONT_I)
	$(MPI)	$(FLAGS) $(TYPEVEC_I) -o $(TYPEVEC_O)	

$(TYPEINDEX_O):$(TYPEINDEX_I)
	$(MPI)	$(FLAGS) $(TYPEINDEX_I) -o $(TYPEINDEX_O)

$(TRAPECIOPU_O):$(TRAPECIOPU_I)
	$(MPI)	$(FLAGS) $(TRAPECIOPU_I) -o $(TRAPECIOPU_O)	

$(ODDEVEN_O):$(ODDEVEN_I)
	$(CC)	$(FLAGS) $(ODDEVEN_I) -o $(ODDEVEN_O)	

$(ODDEVENP_O):$(ODDEVENP_I)
	$(MPI)	$(FLAGS) $(ODDEVENP_I) -o $(ODDEVENP_O)	

$(MATRIXVECTOR_O):$(MATRIXVECTOR_I)
	$(CC) $(FLAGS) $(MATRIXVECTOR_I) -o $(MATRIXVECTOR_O) $(PTHREAD)

$(PI_O):$(PI_I)
	$(CC) $(FLAGS) -00 $(PI_I) -o $(PI_O) $(PTHREAD)

$(PC_O):$(PC_I)
	$(CC) $(FLAGS)	$(PC_I) -o $(PC_O) $(PTHREAD)

$(BARRIER_O):$(BARRIER_I)
	$(CC) $(FLAGS) $(BARRIER_I)	-o $(BARRIER_O) $(PTHREAD)

$(LINKEDLIST_O):$(LINKEDLIST_I)
	$(CC) $(FLAGS) $(LINKEDLIST_I) -o $(LINKEDLIST_O) $(PTHREAD)	

$(STRTOCK_O): $(STRTOCK_I)
	$(CC) $(FLAGS) $(STRTOCK_I) -o $(STRTOCK_O)	 $(PTHREAD)
