.PHONY: all clean

all: write-arm32 exec-arm32

write-arm32:
	@arm-none-eabi-as -c write-arm32.S -o write-arm32.o
	@arm-none-eabi-objcopy -O binary write-arm32.o write-arm32.bin

exec-arm32:
	@arm-none-eabi-as -c exec-arm32.S -o exec-arm32.o
	@arm-none-eabi-objcopy -O binary exec-arm32.o exec-arm32.bin

clean:
	@rm -fr *.o
	@rm -fr *.bin
