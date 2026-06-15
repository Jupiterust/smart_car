################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Core/user_uart/user_uart.c" 

COMPILED_SRCS += \
"Core/user_uart/user_uart.src" 

C_DEPS += \
"./Core/user_uart/user_uart.d" 

OBJS += \
"Core/user_uart/user_uart.o" 


# Each subdirectory must supply rules for building sources it contributes
"Core/user_uart/user_uart.src":"../Core/user_uart/user_uart.c" "Core/user_uart/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/user_uart/user_uart.o":"Core/user_uart/user_uart.src" "Core/user_uart/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Core-2f-user_uart

clean-Core-2f-user_uart:
	-$(RM) ./Core/user_uart/user_uart.d ./Core/user_uart/user_uart.o ./Core/user_uart/user_uart.src

.PHONY: clean-Core-2f-user_uart

