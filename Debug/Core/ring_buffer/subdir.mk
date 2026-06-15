################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Core/ring_buffer/ring_buffer.c" 

COMPILED_SRCS += \
"Core/ring_buffer/ring_buffer.src" 

C_DEPS += \
"./Core/ring_buffer/ring_buffer.d" 

OBJS += \
"Core/ring_buffer/ring_buffer.o" 


# Each subdirectory must supply rules for building sources it contributes
"Core/ring_buffer/ring_buffer.src":"../Core/ring_buffer/ring_buffer.c" "Core/ring_buffer/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/ring_buffer/ring_buffer.o":"Core/ring_buffer/ring_buffer.src" "Core/ring_buffer/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Core-2f-ring_buffer

clean-Core-2f-ring_buffer:
	-$(RM) ./Core/ring_buffer/ring_buffer.d ./Core/ring_buffer/ring_buffer.o ./Core/ring_buffer/ring_buffer.src

.PHONY: clean-Core-2f-ring_buffer

