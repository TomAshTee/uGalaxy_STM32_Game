################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GFX_ssd1327.c \
../Core/Src/buttonFSM.c \
../Core/Src/game_logic.c \
../Core/Src/graphics_map.c \
../Core/Src/input.c \
../Core/Src/itoa.c \
../Core/Src/main.c \
../Core/Src/ssd1327.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

OBJS += \
./Core/Src/GFX_ssd1327.o \
./Core/Src/buttonFSM.o \
./Core/Src/game_logic.o \
./Core/Src/graphics_map.o \
./Core/Src/input.o \
./Core/Src/itoa.o \
./Core/Src/main.o \
./Core/Src/ssd1327.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o 

C_DEPS += \
./Core/Src/GFX_ssd1327.d \
./Core/Src/buttonFSM.d \
./Core/Src/game_logic.d \
./Core/Src/graphics_map.d \
./Core/Src/input.d \
./Core/Src/itoa.d \
./Core/Src/main.d \
./Core/Src/ssd1327.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/GFX_ssd1327.cyclo ./Core/Src/GFX_ssd1327.d ./Core/Src/GFX_ssd1327.o ./Core/Src/GFX_ssd1327.su ./Core/Src/buttonFSM.cyclo ./Core/Src/buttonFSM.d ./Core/Src/buttonFSM.o ./Core/Src/buttonFSM.su ./Core/Src/game_logic.cyclo ./Core/Src/game_logic.d ./Core/Src/game_logic.o ./Core/Src/game_logic.su ./Core/Src/graphics_map.cyclo ./Core/Src/graphics_map.d ./Core/Src/graphics_map.o ./Core/Src/graphics_map.su ./Core/Src/input.cyclo ./Core/Src/input.d ./Core/Src/input.o ./Core/Src/input.su ./Core/Src/itoa.cyclo ./Core/Src/itoa.d ./Core/Src/itoa.o ./Core/Src/itoa.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/ssd1327.cyclo ./Core/Src/ssd1327.d ./Core/Src/ssd1327.o ./Core/Src/ssd1327.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su

.PHONY: clean-Core-2f-Src

