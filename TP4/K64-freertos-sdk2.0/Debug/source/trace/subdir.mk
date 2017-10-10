################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/trace/SEGGER_RTT.c \
../source/trace/trcKernelPort.c \
../source/trace/trcSnapshotRecorder.c \
../source/trace/trcStreamingPort.c \
../source/trace/trcStreamingRecorder.c 

OBJS += \
./source/trace/SEGGER_RTT.o \
./source/trace/trcKernelPort.o \
./source/trace/trcSnapshotRecorder.o \
./source/trace/trcStreamingPort.o \
./source/trace/trcStreamingRecorder.o 

C_DEPS += \
./source/trace/SEGGER_RTT.d \
./source/trace/trcKernelPort.d \
./source/trace/trcSnapshotRecorder.d \
./source/trace/trcStreamingPort.d \
./source/trace/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
source/trace/%.o: ../source/trace/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VDC12" -I../freertos/Source/include -I../startup -I../board -I../utilities -I../freertos/Source/portable/GCC/ARM_CM4F -I../CMSIS -I../source -I../drivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


