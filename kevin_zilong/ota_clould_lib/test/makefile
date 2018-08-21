
CC=/opt/buildroot-gcc463/usr/bin/mipsel-linux-gcc

test:
	$(CC) test.c -I ../inc -L ../lib  -o test -lota_cloud -lcurl -lzxlog -lcrypto -lssl

clean:
	rm test
