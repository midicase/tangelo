Updated BSP build steps:

extract TE0820-te0820-rd-vivado_2024.2-build_1_20250214171345.zip

$ tar -xvf FPGAs_AdaptiveSoCs_Unified_2024.2_1113_1001.tar.gz
$ cd FPGAs_AdaptiveSoCs_Unified_2024.2_1113_1001
$ ./xsetup
Pick the petalinux install.
Install to /opt/xilinx

$ source /opt/xilinx/PetaLinux/2024.2/tool/settings.sh

In some working folder:

$ petalinux-create --type project --template zynqMP --name te0820

Copy this project's petalinux/project-spec folder over te0820/project-spec created from above

$ cd te0820
$ petalinux-config --get-hw-description=supplied.xsa   

Exit since already configured

Note: The default is te0820-rd/prebuilt/hardware/2eg_1i_2gb/te0820-rd_2eg_1i_2gb.xsa in the zip above if want to test with reference board.


$ petalinux-build

Generate BOOT.BIN

$ petalinux-package boot --pmufw ./images/linux/pmufw.elf --atf images/linux/bl31.elf --fsbl ./images/linux/zynqmp_fsbl.elf --u-boot images/linux/u-boot.elf --fpga images/linux/system.bit --force


https://drive.google.com/drive/folders/1iCIf66F-hAvJ4awL5kw-bzrWVx0TShRs?usp=drive_link

Transfer top level boot.bin, boot.scr, image.ub to FAT32 mSD card. These are just vanilla boot files.

Create a folder on the mSD "toflash" or some such, name does not matter. copy part.sh in the above gdrive folder to the toflash folder.


Transfer peta linux generated images/linux/BOOT.BIN, images/linux/boot.scr, images/linux/image.ub to mSD toflash folder.    


clone https://github.com/midicase/buildroot

Switch to rumo branch.

TODO: list pre-reqs here.

$ make tangelo_defconfig
$ make

Transfer output/images/rootfs.tar.gz to the mSD toFlash

boot off the mSD.

run the part.sh script in the toflash folder. 

























