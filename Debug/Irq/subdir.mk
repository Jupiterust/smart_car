################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Irq/irq.c" 

COMPILED_SRCS += \
"Irq/irq.src" 

C_DEPS += \
"./Irq/irq.d" 

OBJS += \
"Irq/irq.o" 


# Each subdirectory must supply rules for building sources it contributes
"Irq/irq.src":"../Irq/irq.c" "Irq/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Irq/irq.o":"Irq/irq.src" "Irq/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Irq

clean-Irq:
	-$(RM) ./Irq/irq.d ./Irq/irq.o ./Irq/irq.src

.PHONY: clean-Irq

