################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../VU-Metre/vu.c 

OBJS += \
./VU-Metre/vu.o 

C_DEPS += \
./VU-Metre/vu.d 


# Each subdirectory must supply rules for building sources it contributes
VU-Metre/%.o VU-Metre/%.su VU-Metre/%.cyclo: ../VU-Metre/%.c VU-Metre/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/romingo/Desktop/ENSEA/3A/TP/2425_ESE_AutoRadio_CARONELLO_PACE/shell" -I"/Users/romingo/Desktop/ENSEA/3A/TP/2425_ESE_AutoRadio_CARONELLO_PACE/callBack" -I"/Users/romingo/Desktop/ENSEA/3A/TP/2425_ESE_AutoRadio_CARONELLO_PACE/VU-Metre" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-VU-2d-Metre

clean-VU-2d-Metre:
	-$(RM) ./VU-Metre/vu.cyclo ./VU-Metre/vu.d ./VU-Metre/vu.o ./VU-Metre/vu.su

.PHONY: clean-VU-2d-Metre

