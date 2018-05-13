CFLAGS = -O3 -lm
LIBS = -lOpenCL -fopenmp

# Change this variable to specify the device type
# to the OpenCL device type of choice. You can also
# edit the variable in the source.
ifndef DEVICE
	DEVICE = CL_DEVICE_TYPE_DEFAULT
endif

# Check our platform and make sure we define the APPLE variable
# and set up the right compiler flags and libraries
PLATFORM = $(shell uname -s)
ifeq ($(PLATFORM), Darwin)
	LIBS = -framework OpenCL
endif

CFLAGS += -D DEVICE=$(DEVICE)

vadd: vadd.c util.c
	$(CC) $^ $(CCFLAGS) $(LIBS) -o $@


clean:
	$(RM) vadd
