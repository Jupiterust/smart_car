################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.c" 

COMPILED_SRCS += \
"Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.src" 

C_DEPS += \
"./Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.d" 

OBJS += \
"Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.o" 


# Each subdirectory must supply rules for building sources it contributes
"Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.src":"../Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.c" "Core/fashion_star_uart_servo_examples/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.o":"Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.src" "Core/fashion_star_uart_servo_examples/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Core-2f-fashion_star_uart_servo_examples

clean-Core-2f-fashion_star_uart_servo_examples:
	-$(RM) ./Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.d ./Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.o ./Core/fashion_star_uart_servo_examples/fashion_star_uart_servo_examples.src

.PHONY: clean-Core-2f-fashion_star_uart_servo_examples

