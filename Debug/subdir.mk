################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -std=c99 -DUSE_STDPERIPH_DRIVER -DSTM32L1XX_MD -I"/Users/nathan/Development/stm32/accelerometer" -I"/Users/nathan/Development/stm32/accelerometer/Board" -I"/Users/nathan/Development/stm32/accelerometer/User" -I"/Users/nathan/Development/stm32/accelerometer/STM32L1xx_StdPeriph_Driver/inc" -I"/Users/nathan/Development/stm32/accelerometer/CMSIS/ST/STM32L1xx/Include" -I"/Users/nathan/Development/stm32/accelerometer/CMSIS/Include" -O0 -g3 -DWARF2 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m3 -O0 -ffunction-sections -fdata-sections -g -Wall -w -mlong-calls -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


