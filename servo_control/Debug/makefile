################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F3xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := servo_control
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
servo_control.elf \

MAP_FILES += \
servo_control.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
servo_control.list \


# All Target
all: main-build

# Main-build Target
main-build: servo_control.elf secondary-outputs

dependents:
	-cd C:/Users/benri/OneDrive/Documents/MTRX2700/mtrx2700-lec-example-with-notes/MTRX2700-2024/STMF3disco-BSP/Debug && $(MAKE) all

# Tool invocations
servo_control.elf servo_control.map: $(OBJS) $(USER_OBJS) C:/Users/benri/OneDrive/Documents/MTRX2700/mtrx2700-lec-example-with-notes/MTRX2700-2024/STMF3disco-BSP/Debug/libF3-components.a C:\Users\benri\OneDrive\Documents\MTRX2700\A3\servo_control\STM32F303VCTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "servo_control.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\benri\OneDrive\Documents\MTRX2700\A3\servo_control\STM32F303VCTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="servo_control.map" -Wl,--gc-sections -static -L"C:\Users\benri\OneDrive\Documents\MTRX2700\mtrx2700-lec-example-with-notes\MTRX2700-2024\STMF3disco-BSP\Debug" -L"C:\Users\benri\OneDrive\Documents\MTRX2700\mtrx2700-lec-example-with-notes\MTRX2700-2024\STMF3disco-BSP\Drivers\STM32F3xx_HAL_Driver\Inc" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

servo_control.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "servo_control.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout servo_control.elf servo_control.list servo_control.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified
C:/Users/benri/OneDrive/Documents/MTRX2700/mtrx2700-lec-example-with-notes/MTRX2700-2024/STMF3disco-BSP/Debug/libF3-components.a:

-include ../makefile.targets
