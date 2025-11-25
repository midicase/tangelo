FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://bsp.cfg \
   file://0001-parallel-memories-do-not-use-size-property.patch"
KERNEL_FEATURES:append = " bsp.cfg"
