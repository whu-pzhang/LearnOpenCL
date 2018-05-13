CFLAGS = -std=c99 -Wall -O3 -lm
LDFLAGS = -lOpenCL -fopenmp

# Change this variable to specify the device type
# to the OpenCL device type of choice. You can also
# edit the variable in the source.
ifndef DEVICE
	DEVICE = CL_DEVICE_TYPE_GPU
endif

# Check our platform and make sure we define the APPLE variable
# and set up the right compiler flags and libraries
PLATFORM = $(shell uname -s)
ifeq ($(PLATFORM), Darwin)
	LDFLAGS = -framework OpenCL
endif

CFLAGS += -D DEVICE=$(DEVICE)

all : platforminfo deviceinfo vadd

platforminfo: platforminfo.c

deviceinfo: deviceinfo.c

vadd: util.c


clean:
	$(RM) \
	platforminfo \
	deviceinfo \
	vadd \
