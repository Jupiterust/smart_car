################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../src/APP/LQ_Atom_Motor.c" \
"../src/APP/LQ_CCU6_Timer.c" \
"../src/APP/LQ_GPIO_KEY.c" \
"../src/APP/LQ_GPIO_LED.c" \
"../src/APP/LQ_I2C_9AX.c" \
"../src/APP/LQ_I2C_VL53.c" \
"../src/APP/LQ_ICM20602.c" \
"../src/APP/LQ_IIC_Gyro.c" \
"../src/APP/LQ_IPSLCD.c" \
"../src/APP/LQ_LSM6DSR.c" \
"../src/APP/LQ_OLED096.c" \
"../src/APP/LQ_STM_Timer.c" \
"../src/APP/LQ_TFT18.c" \
"../src/APP/LQ_TFT2.c" \
"../src/APP/LQ_TFTPicFont.c" \
"../src/APP/LQ_Tom_Servo.c" 

O_SRCS += \
"../src/APP/LQ_MPU6050_DMP.o" 

COMPILED_SRCS += \
"src/APP/LQ_Atom_Motor.src" \
"src/APP/LQ_CCU6_Timer.src" \
"src/APP/LQ_GPIO_KEY.src" \
"src/APP/LQ_GPIO_LED.src" \
"src/APP/LQ_I2C_9AX.src" \
"src/APP/LQ_I2C_VL53.src" \
"src/APP/LQ_ICM20602.src" \
"src/APP/LQ_IIC_Gyro.src" \
"src/APP/LQ_IPSLCD.src" \
"src/APP/LQ_LSM6DSR.src" \
"src/APP/LQ_OLED096.src" \
"src/APP/LQ_STM_Timer.src" \
"src/APP/LQ_TFT18.src" \
"src/APP/LQ_TFT2.src" \
"src/APP/LQ_TFTPicFont.src" \
"src/APP/LQ_Tom_Servo.src" 

C_DEPS += \
"./src/APP/LQ_Atom_Motor.d" \
"./src/APP/LQ_CCU6_Timer.d" \
"./src/APP/LQ_GPIO_KEY.d" \
"./src/APP/LQ_GPIO_LED.d" \
"./src/APP/LQ_I2C_9AX.d" \
"./src/APP/LQ_I2C_VL53.d" \
"./src/APP/LQ_ICM20602.d" \
"./src/APP/LQ_IIC_Gyro.d" \
"./src/APP/LQ_IPSLCD.d" \
"./src/APP/LQ_LSM6DSR.d" \
"./src/APP/LQ_OLED096.d" \
"./src/APP/LQ_STM_Timer.d" \
"./src/APP/LQ_TFT18.d" \
"./src/APP/LQ_TFT2.d" \
"./src/APP/LQ_TFTPicFont.d" \
"./src/APP/LQ_Tom_Servo.d" 

OBJS += \
"src/APP/LQ_Atom_Motor.o" \
"src/APP/LQ_CCU6_Timer.o" \
"src/APP/LQ_GPIO_KEY.o" \
"src/APP/LQ_GPIO_LED.o" \
"src/APP/LQ_I2C_9AX.o" \
"src/APP/LQ_I2C_VL53.o" \
"src/APP/LQ_ICM20602.o" \
"src/APP/LQ_IIC_Gyro.o" \
"src/APP/LQ_IPSLCD.o" \
"src/APP/LQ_LSM6DSR.o" \
"src/APP/LQ_OLED096.o" \
"src/APP/LQ_STM_Timer.o" \
"src/APP/LQ_TFT18.o" \
"src/APP/LQ_TFT2.o" \
"src/APP/LQ_TFTPicFont.o" \
"src/APP/LQ_Tom_Servo.o" 


# Each subdirectory must supply rules for building sources it contributes
"src/APP/LQ_Atom_Motor.src":"../src/APP/LQ_Atom_Motor.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_Atom_Motor.o":"src/APP/LQ_Atom_Motor.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_CCU6_Timer.src":"../src/APP/LQ_CCU6_Timer.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_CCU6_Timer.o":"src/APP/LQ_CCU6_Timer.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_GPIO_KEY.src":"../src/APP/LQ_GPIO_KEY.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_GPIO_KEY.o":"src/APP/LQ_GPIO_KEY.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_GPIO_LED.src":"../src/APP/LQ_GPIO_LED.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_GPIO_LED.o":"src/APP/LQ_GPIO_LED.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_I2C_9AX.src":"../src/APP/LQ_I2C_9AX.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_I2C_9AX.o":"src/APP/LQ_I2C_9AX.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_I2C_VL53.src":"../src/APP/LQ_I2C_VL53.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_I2C_VL53.o":"src/APP/LQ_I2C_VL53.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_ICM20602.src":"../src/APP/LQ_ICM20602.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_ICM20602.o":"src/APP/LQ_ICM20602.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_IIC_Gyro.src":"../src/APP/LQ_IIC_Gyro.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_IIC_Gyro.o":"src/APP/LQ_IIC_Gyro.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_IPSLCD.src":"../src/APP/LQ_IPSLCD.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_IPSLCD.o":"src/APP/LQ_IPSLCD.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_LSM6DSR.src":"../src/APP/LQ_LSM6DSR.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_LSM6DSR.o":"src/APP/LQ_LSM6DSR.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_OLED096.src":"../src/APP/LQ_OLED096.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_OLED096.o":"src/APP/LQ_OLED096.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_STM_Timer.src":"../src/APP/LQ_STM_Timer.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_STM_Timer.o":"src/APP/LQ_STM_Timer.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_TFT18.src":"../src/APP/LQ_TFT18.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_TFT18.o":"src/APP/LQ_TFT18.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_TFT2.src":"../src/APP/LQ_TFT2.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_TFT2.o":"src/APP/LQ_TFT2.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_TFTPicFont.src":"../src/APP/LQ_TFTPicFont.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_TFTPicFont.o":"src/APP/LQ_TFTPicFont.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"src/APP/LQ_Tom_Servo.src":"../src/APP/LQ_Tom_Servo.c" "src/APP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2012 -D__CPU__=tc26xb "-fE:/aurix/Longqi_project/LQ_Encoder__LIB/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"src/APP/LQ_Tom_Servo.o":"src/APP/LQ_Tom_Servo.src" "src/APP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-src-2f-APP

clean-src-2f-APP:
	-$(RM) ./src/APP/LQ_Atom_Motor.d ./src/APP/LQ_Atom_Motor.o ./src/APP/LQ_Atom_Motor.src ./src/APP/LQ_CCU6_Timer.d ./src/APP/LQ_CCU6_Timer.o ./src/APP/LQ_CCU6_Timer.src ./src/APP/LQ_GPIO_KEY.d ./src/APP/LQ_GPIO_KEY.o ./src/APP/LQ_GPIO_KEY.src ./src/APP/LQ_GPIO_LED.d ./src/APP/LQ_GPIO_LED.o ./src/APP/LQ_GPIO_LED.src ./src/APP/LQ_I2C_9AX.d ./src/APP/LQ_I2C_9AX.o ./src/APP/LQ_I2C_9AX.src ./src/APP/LQ_I2C_VL53.d ./src/APP/LQ_I2C_VL53.o ./src/APP/LQ_I2C_VL53.src ./src/APP/LQ_ICM20602.d ./src/APP/LQ_ICM20602.o ./src/APP/LQ_ICM20602.src ./src/APP/LQ_IIC_Gyro.d ./src/APP/LQ_IIC_Gyro.o ./src/APP/LQ_IIC_Gyro.src ./src/APP/LQ_IPSLCD.d ./src/APP/LQ_IPSLCD.o ./src/APP/LQ_IPSLCD.src ./src/APP/LQ_LSM6DSR.d ./src/APP/LQ_LSM6DSR.o ./src/APP/LQ_LSM6DSR.src ./src/APP/LQ_OLED096.d ./src/APP/LQ_OLED096.o ./src/APP/LQ_OLED096.src ./src/APP/LQ_STM_Timer.d ./src/APP/LQ_STM_Timer.o ./src/APP/LQ_STM_Timer.src ./src/APP/LQ_TFT18.d ./src/APP/LQ_TFT18.o ./src/APP/LQ_TFT18.src ./src/APP/LQ_TFT2.d ./src/APP/LQ_TFT2.o ./src/APP/LQ_TFT2.src ./src/APP/LQ_TFTPicFont.d ./src/APP/LQ_TFTPicFont.o ./src/APP/LQ_TFTPicFont.src ./src/APP/LQ_Tom_Servo.d ./src/APP/LQ_Tom_Servo.o ./src/APP/LQ_Tom_Servo.src

.PHONY: clean-src-2f-APP

