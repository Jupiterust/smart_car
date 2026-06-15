################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../src/User/LQ_MotorServo.c" 

COMPILED_SRCS += \
"src/User/LQ_MotorServo.src" 

C_DEPS += \
"./src/User/LQ_MotorServo.d" 

OBJS += \
"src/User/LQ_MotorServo.o" 


# Each subdirectory must supply rules for building sources it contributes
"src/User/LQ_MotorServo.src":"../src/User/LQ_MotorServo.c" "src/User/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/User/LQ_MotorServo.o":"src/User/LQ_MotorServo.src" "src/User/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-src-2f-User

clean-src-2f-User:
	-$(RM) ./src/User/LQ_MotorServo.d ./src/User/LQ_MotorServo.o ./src/User/LQ_MotorServo.src

.PHONY: clean-src-2f-User

