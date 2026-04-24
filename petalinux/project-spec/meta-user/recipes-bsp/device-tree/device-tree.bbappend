FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://system-user.dtsi"

require ${@'device-tree-sdt.inc' if d.getVar('SYSTEM_DTFILE') != '' else ''}

RM_WORK_EXCLUDE_ITEMS += "build"

do_install:append () {
  for PP in *.dts.pp; do
    sed -e '/^#/d' < ${B}/${PP} > ${TOPDIR}/${PP}
  done
}

