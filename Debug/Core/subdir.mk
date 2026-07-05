################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Core/Communication.c" \
"../Core/IR_sensor.c" \
"../Core/Motor.c" \
"../Core/Servo.c" \
"../Core/Task.c" \
"../Core/cheju.c" \
"../Core/motified_app.c" 

COMPILED_SRCS += \
"Core/Communication.src" \
"Core/IR_sensor.src" \
"Core/Motor.src" \
"Core/Servo.src" \
"Core/Task.src" \
"Core/cheju.src" \
"Core/motified_app.src" 

C_DEPS += \
"./Core/Communication.d" \
"./Core/IR_sensor.d" \
"./Core/Motor.d" \
"./Core/Servo.d" \
"./Core/Task.d" \
"./Core/cheju.d" \
"./Core/motified_app.d" 

OBJS += \
"Core/Communication.o" \
"Core/IR_sensor.o" \
"Core/Motor.o" \
"Core/Servo.o" \
"Core/Task.o" \
"Core/cheju.o" \
"Core/motified_app.o" 


# Each subdirectory must supply rules for building sources it contributes
"Core/Communication.src":"../Core/Communication.c" "Core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/Communication.o":"Core/Communication.src" "Core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Core/IR_sensor.src":"../Core/IR_sensor.c" "Core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/IR_sensor.o":"Core/IR_sensor.src" "Core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Core/Motor.src":"../Core/Motor.c" "Core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/Motor.o":"Core/Motor.src" "Core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Core/Servo.src":"../Core/Servo.c" "Core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/Servo.o":"Core/Servo.src" "Core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Core/Task.src":"../Core/Task.c" "Core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/Task.o":"Core/Task.src" "Core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Core/cheju.src":"../Core/cheju.c" "Core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/cheju.o":"Core/cheju.src" "Core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Core/motified_app.src":"../Core/motified_app.c" "Core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"Core/motified_app.o":"Core/motified_app.src" "Core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Core

clean-Core:
	-$(RM) ./Core/Communication.d ./Core/Communication.o ./Core/Communication.src ./Core/IR_sensor.d ./Core/IR_sensor.o ./Core/IR_sensor.src ./Core/Motor.d ./Core/Motor.o ./Core/Motor.src ./Core/Servo.d ./Core/Servo.o ./Core/Servo.src ./Core/Task.d ./Core/Task.o ./Core/Task.src ./Core/cheju.d ./Core/cheju.o ./Core/cheju.src ./Core/motified_app.d ./Core/motified_app.o ./Core/motified_app.src

.PHONY: clean-Core

