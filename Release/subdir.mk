################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../consoleLib.c \
../consoleMenu.c \
../gameField.c \
../helpMenu.c \
../main.c 

OBJS += \
./consoleLib.o \
./consoleMenu.o \
./gameField.o \
./helpMenu.o \
./main.o 

C_DEPS += \
./consoleLib.d \
./consoleMenu.d \
./gameField.d \
./helpMenu.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


