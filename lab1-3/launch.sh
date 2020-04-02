cat bootloader.com prob.com > system.com
dd if=system.com of=system.img bs=1440k  count=1 conv=sync
bochs -q -f bochsrc

