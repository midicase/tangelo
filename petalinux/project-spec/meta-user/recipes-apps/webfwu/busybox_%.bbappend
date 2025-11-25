FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " \
	    file://busybox-httpd.service.in \
           "


# HTTPD package
FILES:${PN}-httpd:append = " ${systemd_unitdir}/system/busybox-httpd.service"
SYSTEMD_PACKAGES += "${PN}-httpd"
SYSTEMD_SERVICE:${PN}-httpd = "busybox-httpd.service"

do_install:append() {
  install -d ${D}${systemd_unitdir}/system
  sed 's,@sbindir@,${sbindir},g' < ${WORKDIR}/busybox-httpd.service.in \
    > ${D}${systemd_unitdir}/system/busybox-httpd.service
}


REQUIRED_DISTRO_FEATURES = " systemd"