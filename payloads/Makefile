.PHONY: all clean

all: write-arm32 write-arm64 exec-arm32 exec-arm64

write-arm32:
	@arm-none-eabi-as -c write-arm32.S -o write-arm32.o
	@arm-none-eabi-objcopy -O binary write-arm32.o write-arm32.bin

write-arm64:
	@aarch64-linux-gnu-as -c write-arm64.S -o write-arm64.o
	@aarch64-linux-gnu-objcopy -O binary write-arm64.o write-arm64.bin

exec-arm32:
	@arm-none-eabi-as -c exec-arm32.S -o exec-arm32.o
	@arm-none-eabi-objcopy -O binary exec-arm32.o exec-arm32.bin

exec-arm64:
	@aarch64-linux-gnu-as -c exec-arm64.S -o exec-arm64.o
	@aarch64-linux-gnu-objcopy -O binary exec-arm64.o exec-arm64.bin

clean:
	@rm -fr *.o
	@rm -fr *.bin
