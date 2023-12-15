################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/myMain/libs/rtc_lib/rtc_lib.c 

OBJS += \
./Core/myMain/libs/rtc_lib/rtc_lib.o 

C_DEPS += \
./Core/myMain/libs/rtc_lib/rtc_lib.d 


# Each subdirectory must supply rules for building sources it contributes
Core/myMain/libs/rtc_lib/%.o Core/myMain/libs/rtc_lib/%.su Core/myMain/libs/rtc_lib/%.cyclo: ../Core/myMain/libs/rtc_lib/%.c Core/myMain/libs/rtc_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L071xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-myMain-2f-libs-2f-rtc_lib

clean-Core-2f-myMain-2f-libs-2f-rtc_lib:
	-$(RM) ./Core/myMain/libs/rtc_lib/rtc_lib.cyclo ./Core/myMain/libs/rtc_lib/rtc_lib.d ./Core/myMain/libs/rtc_lib/rtc_lib.o ./Core/myMain/libs/rtc_lib/rtc_lib.su

.PHONY: clean-Core-2f-myMain-2f-libs-2f-rtc_lib

