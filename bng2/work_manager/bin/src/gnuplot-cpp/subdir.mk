################################################################################
# Automatically-generated file. Do not edit!
################################################################################
-include ../../mpipath.mk
CFLAGS = -ggdb
DEFINES = -DDEBUGGA
INCLUDES = 
LIBS = -lstdc++
# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gnuplot-cpp/gnuplot-cpp.cc \

OBJS += \
./src/gnuplot-cpp/gnuplot-cpp.o \

# Each subdirectory must supply rules for building sources it contributes
src/gnuplot-cpp/%.o: ../src/gnuplot-cpp/%.cc  
	@echo 'Building file: $<'
	@echo 'Invoking: mpic++ Compiler'
	$(CXX) -std=c++0x -c $(CFLAGS) $(DFINES) $(INCLUDES) $(LIBS) -o "$@" "$<"  
	@echo 'Finished building: $<'
	@echo ' '


