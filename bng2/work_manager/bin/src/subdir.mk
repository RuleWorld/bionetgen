################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/work_manager.cpp \

OBJS += \
./src/work_manager.o \

CPP_DEPS += \
./src/work_manager.d \


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp  
	@echo 'Building file: $<'
	@echo 'Invoking: mpic++ Compiler'
	$(CXX) $(CFLAGS) -O3 -std=c++0x -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"  
	@echo 'Finished building: $<'
	@echo ' '


