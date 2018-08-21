cc=/opt/buildroot-gcc463/usr/bin/mipsel-linux-gcc
libota_cloud.so: clean
	$(cc) -I ./inc -L ./lib -lcurl -lzxlog src/*.c -o libota_cloud.so -fpic -shared

clean:
	rm libota_cloud.so -f
	
