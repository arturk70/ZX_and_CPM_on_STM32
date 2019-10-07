################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ZX_and_CPM_on_STM32.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ZX_and_CPM_on_STM32.list \

OBJCOPY_BIN += \
ZX_and_CPM_on_STM32.bin \


# All Target
all: ZX_and_CPM_on_STM32.elf secondary-outputs

# Tool invocations
ZX_and_CPM_on_STM32.elf: $(OBJS) $(USER_OBJS) C:\work\Projects\STM32\ZX_and_CPM_on_STM32\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "ZX_and_CPM_on_STM32.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\work\Projects\STM32\ZX_and_CPM_on_STM32\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ZX_and_CPM_on_STM32.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ZX_and_CPM_on_STM32.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ZX_and_CPM_on_STM32.list"
	@echo 'Finished building: $@'
	@echo ' '

ZX_and_CPM_on_STM32.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ZX_and_CPM_on_STM32.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
