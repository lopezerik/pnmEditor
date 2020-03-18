SRC=mainEditor.C image.C sink.C source.C PNMreader.C PNMwriter.C filters.C logging.C
OBJ=$(SRC:.C=.o) #changes 'image.C' to 'image.o', etc.

prog: $(OBJ)
	g++ $(OBJ) -o pnmEditor

.C.o: $<
	g++ -Wall -g -I. -c $<

clean: #different machines either create 'pnmEditor' or 'pnmEditor.exe' when 'make' is called
	rm *.o pnmEditor pnmEditor.exe
