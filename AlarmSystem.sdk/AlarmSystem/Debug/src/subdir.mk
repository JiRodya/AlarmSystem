################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/clock.c \
../src/data_handle.c \
../src/interrupt_handler.c \
../src/io.c \
../src/main.c \
../src/oled.c 

OBJS += \
./src/clock.o \
./src/data_handle.o \
./src/interrupt_handler.o \
./src/io.o \
./src/main.o \
./src/oled.o 

C_DEPS += \
./src/clock.d \
./src/data_handle.d \
./src/interrupt_handler.d \
./src/io.d \
./src/main.d \
./src/oled.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../../AlarmSystem_bsp/ps7_cortexa9_0/include -I"C:\X_Projects\AlarmSystem\AlarmSystem.sdk\AlarmSystem" -I"C:\X_Projects\AlarmSystem\AlarmSystem.sdk\AlarmSystem\inc" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


