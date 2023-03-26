SOURCE_DIR	:=	src
SOURCES		:=	$(shell find $(SOURCE_DIR) -type f -name '*.c' | tr '\n' ' ')
LIBRARY_DIR	:=	lib
LIBRARIES	:=	$(shell find $(LIBRARY_DIR) -type f -name '*.a' | tr '\n' ' ')
INCLUDES	:=	./include
FLAGS		:=	-I $(INCLUDES) -lm
OUTPUT_FILE	:=	engine

build:
	@echo " - Building engine."
	mpicc $(SOURCES) $(LIBRARIES) $(FLAGS) -o $(OUTPUT_FILE)

clean:
	@echo " - Cleaning files."
	rm $(OUTPUT_FILE)
  
