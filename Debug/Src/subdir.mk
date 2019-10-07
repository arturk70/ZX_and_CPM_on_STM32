################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/cpm_display.c \
../Src/ili9341_driver.c \
../Src/main.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/cpm_display.o \
./Src/ili9341_driver.o \
./Src/main.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/cpm_display.d \
./Src/ili9341_driver.d \
./Src/main.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/cpm_display.o: ../Src/cpm_display.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 '-DHSE_VALUE=8000000' -DSTM32F103xB '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' '-DVDD_VALUE=3300' '-DLSI_VALUE=40000' '-DHSI_VALUE=8000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/cpm_display.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/ili9341_driver.o: ../Src/ili9341_driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 '-DHSE_VALUE=8000000' -DSTM32F103xB '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' '-DVDD_VALUE=3300' '-DLSI_VALUE=40000' '-DHSI_VALUE=8000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ili9341_driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 '-DHSE_VALUE=8000000' -DSTM32F103xB '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' '-DVDD_VALUE=3300' '-DLSI_VALUE=40000' '-DHSI_VALUE=8000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/stm32f1xx_it.o: ../Src/stm32f1xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 '-DHSE_VALUE=8000000' -DSTM32F103xB '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' '-DVDD_VALUE=3300' '-DLSI_VALUE=40000' '-DHSI_VALUE=8000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f1xx_it.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 '-DHSE_VALUE=8000000' -DSTM32F103xB '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' '-DVDD_VALUE=3300' '-DLSI_VALUE=40000' '-DHSI_VALUE=8000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 '-DHSE_VALUE=8000000' -DSTM32F103xB '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' '-DVDD_VALUE=3300' '-DLSI_VALUE=40000' '-DHSI_VALUE=8000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/system_stm32f1xx.o: ../Src/system_stm32f1xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 '-DHSE_VALUE=8000000' -DSTM32F103xB '-DHSE_STARTUP_TIMEOUT=100' '-DLSE_STARTUP_TIMEOUT=5000' -DDEBUG '-DLSE_VALUE=32768' '-DVDD_VALUE=3300' '-DLSI_VALUE=40000' '-DHSI_VALUE=8000000' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=1' -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f1xx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

