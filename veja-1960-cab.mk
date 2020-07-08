######################################
#
# Makefile for cabsim-bass
#
######################################

# where to find the source code - locally in this case
VEJA_1960_CAB_SITE_METHOD = local
VEJA_1960_CAB_SITE = $($(PKG)_PKGDIR)/

# even though this is a local build, we still need a version number
# bump this number if you need to force a rebuild
VEJA_1960_CAB_VERSION = 1

# dependencies (list of other buildroot packages, separated by space)
VEJA_1960_CAB_DEPENDENCIES =

# LV2 bundles that this package generates (space separated list)
VEJA_1960_CAB_BUNDLES = veja-1960-cab.lv2

# call make with the current arguments and path. "$(@D)" is the build directory.
VEJA_1960_CAB_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D)/source


# build command
define VEJA_1960_CAB_BUILD_CMDS
	$(VEJA_1960_CAB_TARGET_MAKE)
endef

# install command
define VEJA_1960_CAB_INSTALL_TARGET_CMDS
	$(VEJA_1960_CAB_TARGET_MAKE) install DESTDIR=$(TARGET_DIR)
endef


# import everything else from the buildroot generic package
$(eval $(generic-package))
