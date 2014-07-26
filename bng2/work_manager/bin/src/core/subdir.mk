################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/core.cpp \

OBJS += \
./src/core/core.o \

CPP_DEPS += \
./src/core/core.d \


# Each subdirectory must supply rules for building sources it contributes
src/core/%.o: ../src/core/%.cpp  
	@echo 'Building file: $<'
	@echo 'Invoking: mpic++ Compiler'
	$(CXX) $(CFLAGS) -O3 -std=c++0x -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"  
	@echo 'Finished building: $<'
	@echo ' '


