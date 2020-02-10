%define CHIPSET sc9830
%define KERNEL_VERSION 3.10
%define ARM arm
%define IMAGE zImage
%define DZIMAGE dzImage

Name: linux-%{KERNEL_VERSION}-%{CHIPSET}
Summary: The Linux Kernel
Version: Tizen_sc9830_20170324_1_a4771d9
Release: 1
License: GPL-2.0
Group: System/Kernel
Vendor: The Linux Community
URL: http://www.kernel.org
Source0: %{name}-%{version}.tar.gz
Source101: rpmlintrc

BuildRoot: %{_tmppath}/%{name}-%{PACKAGE_VERSION}-root
Provides: linux-%{KERNEL_VERSION}
%define __spec_install_post /usr/lib/rpm/brp-compress || :
%define debug_package %{nil}

BuildRequires:  lzop
BuildRequires:  binutils-devel
BuildRequires:  module-init-tools
BuildRequires:	python
BuildRequires:	gcc
BuildRequires:	bash
BuildRequires:	system-tools
BuildRequires:	sec-product-features
BuildRequires:	bc
ExclusiveArch:  %arm

%description
The Linux Kernel, the operating system core itself

%if "%{?sec_product_feature_kernel_defconfig}" == "undefined"
%define MODEL tizen_z2
%else
%define MODEL tizen_%{?sec_product_feature_kernel_defconfig}
%endif

%if "%{?sec_product_feature_system_carrier_type}" == "undefined"
%define CARRIER ""
%else
%define CARRIER %{?sec_product_feature_system_carrier_type}
%endif

%if "%{?sec_product_feature_system_region_name}" == "undefined"
%define REGION ""
%else
%define REGION %{?sec_product_feature_system_region_name}
%endif

%if "%{?sec_product_feature_system_operator_name}" == "undefined"
%define OPERATOR ""
%else
%define OPERATOR %{?sec_product_feature_system_operator_name}
%endif

%package -n linux-%{KERNEL_VERSION}-%{CHIPSET}_%{MODEL}
License: GPL-2.0
Summary: Linux support headers for userspace development
Group: System/Kernel
Requires(post): coreutils

%files -n linux-%{KERNEL_VERSION}-%{CHIPSET}_%{MODEL}
/var/tmp/kernel/mod_%{MODEL}
/var/tmp/kernel/kernel-%{MODEL}/%{DZIMAGE}
/var/tmp/kernel/kernel-%{MODEL}/%{DZIMAGE}-recovery

%post -n linux-%{KERNEL_VERSION}-%{CHIPSET}_%{MODEL}
mv /var/tmp/kernel/mod_%{MODEL}/lib/modules/* /lib/modules/.
mv /var/tmp/kernel/kernel-%{MODEL}/%{DZIMAGE} /var/tmp/kernel/.
mv /var/tmp/kernel/kernel-%{MODEL}/%{DZIMAGE}-recovery /var/tmp/kernel/.

%description -n linux-%{KERNEL_VERSION}-%{CHIPSET}_%{MODEL}
This package provides the %{CHIPSET}_eur linux kernel image & module.img.

%package -n linux-%{KERNEL_VERSION}-%{CHIPSET}_%{MODEL}-debuginfo
License: GPL-2.0
Summary: Linux support debug symbol
Group: System/Kernel

%files -n linux-%{KERNEL_VERSION}-%{CHIPSET}_%{MODEL}-debuginfo
/var/tmp/kernel/mod_%{MODEL}
/var/tmp/kernel/kernel-%{MODEL}

%description -n linux-%{KERNEL_VERSION}-%{CHIPSET}_%{MODEL}-debuginfo
This package provides the %{CHIPSET}_eur linux kernel's debugging files.

%package -n kernel-headers-%{KERNEL_VERSION}-%{CHIPSET}
License: GPL-2.0
Summary: Linux support headers for userspace development
Group: System/Kernel
Provides: kernel-headers, kernel-headers-tizen-dev
Obsoletes: kernel-headers

%description -n kernel-headers-%{KERNEL_VERSION}-%{CHIPSET}
This package provides userspaces headers from the Linux kernel. These
headers are used by the installed headers for GNU glibc and other system
 libraries.

%package -n kernel-devel-%{KERNEL_VERSION}-%{CHIPSET}
License: GPL-2.0
Summary: Linux support kernel map and etc for other package
Group: System/Kernel
Provides: kernel-devel-tizen-dev
Provides: kernel-devel-tizen

%description -n kernel-devel-%{KERNEL_VERSION}-%{CHIPSET}
This package provides kernel map and etc information.

%package -n linux-kernel-license
License: GPL-2.0
Summary: Linux support kernel license file
Group: System/Kernel

%description -n linux-kernel-license
This package provides kernel license file.

%prep
%setup -q

%build
%if 0%{?tizen_build_binary_release_type_eng}
%define RELEASE eng
%else
%define RELEASE usr
%endif

mkdir -p %{_builddir}/mod_%{MODEL}
make distclean

./release_obs.sh %{RELEASE} %{MODEL} %{CARRIER} %{REGION} %{OPERATOR}

cp -f arch/%{ARM}/boot/%{IMAGE} %{_builddir}/%{IMAGE}.%{MODEL}
cp -f arch/%{ARM}/boot/merged-dtb %{_builddir}/merged-dtb.%{MODEL}
cp -f arch/%{ARM}/boot/%{DZIMAGE} %{_builddir}/%{DZIMAGE}.%{MODEL}
cp -f arch/%{ARM}/boot/%{DZIMAGE} %{_builddir}/%{DZIMAGE}-recovery.%{MODEL}
cp -f System.map %{_builddir}/System.map.%{MODEL}
cp -f .config %{_builddir}/config.%{MODEL}
cp -f vmlinux %{_builddir}/vmlinux.%{MODEL}

make modules
make modules_install INSTALL_MOD_PATH=%{_builddir}/mod_%{MODEL}

	# prepare for devel package
	find %{_builddir}/%{name}-%{version} -name ".tmp_vmlinux*" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "\.*dtb*tmp" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "*\.*tmp" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "vmlinux" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "bzImage" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "zImage" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "dzImage" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "*.cmd" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "*\.ko" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "*\.o" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "*\.S" -exec rm -f {} \;
	find %{_builddir}/%{name}-%{version} -name "*\.c" -not -path "%{_builddir}/%{name}-%{version}/scripts/*" -exec rm -f {} \;
#remove all changed source codes for next build
cd %_builddir
mv %{name}-%{version} kernel-devel-%{MODEL}
/bin/tar -xf %{SOURCE0}
cd %{name}-%{version}

%install
mkdir -p %{buildroot}/usr
make mrproper
make headers_check
make headers_install INSTALL_HDR_PATH=%{buildroot}/usr

find  %{buildroot}/usr/include -name ".install" | xargs rm -f
find  %{buildroot}/usr/include -name "..install.cmd" | xargs rm -f
rm -rf %{buildroot}/usr/include/scsi
rm -f %{buildroot}/usr/include/asm*/atomic.h
rm -f %{buildroot}/usr/include/asm*/io.h

mkdir -p %{buildroot}/usr/share/license
cp -vf COPYING %{buildroot}/usr/share/license/linux-kernel

mkdir -p %{buildroot}/var/tmp/kernel/devel
mkdir -p %{buildroot}/var/tmp/kernel/kernel-%{MODEL}
mkdir -p %{buildroot}/var/tmp/kernel/license-%{MODEL}

mv %_builddir/mod_%{MODEL} %{buildroot}/var/tmp/kernel/mod_%{MODEL}

mv %_builddir/%{IMAGE}.%{MODEL} %{buildroot}/var/tmp/kernel/kernel-%{MODEL}/%{IMAGE}
mv %_builddir/merged-dtb.%{MODEL} %{buildroot}/var/tmp/kernel/kernel-%{MODEL}/merged-dtb
mv %_builddir/%{DZIMAGE}.%{MODEL} %{buildroot}/var/tmp/kernel/kernel-%{MODEL}/%{DZIMAGE}
mv %_builddir/%{DZIMAGE}-recovery.%{MODEL} %{buildroot}/var/tmp/kernel/kernel-%{MODEL}/%{DZIMAGE}-recovery

mv %_builddir/System.map.%{MODEL} %{buildroot}/var/tmp/kernel/kernel-%{MODEL}/System.map
mv %_builddir/config.%{MODEL} %{buildroot}/var/tmp/kernel/kernel-%{MODEL}/config
mv %_builddir/vmlinux.%{MODEL} %{buildroot}/var/tmp/kernel/kernel-%{MODEL}/vmlinux

mv %_builddir/kernel-devel-%{MODEL} %{buildroot}/var/tmp/kernel/devel/kernel-devel-%{MODEL}

# Create a symbolic link to refer the kernel source from /boot/kernel/devel/tizen-devel
mkdir -p %{buildroot}/boot/kernel/devel/
ln -s /var/tmp/kernel/devel/kernel-devel-%{MODEL} %{buildroot}/boot/kernel/devel/tizen-devel

find %{buildroot}/var/tmp/kernel/ -name "*.h" -exec chmod 644 {} \;
find %{buildroot}/var/tmp/kernel/ -name 'System.map' -not -path "%{buildroot}/var/tmp/kernel/devel/*" > develfiles.pre # for secure storage
find %{buildroot}/var/tmp/kernel/ -name 'vmlinux' -not -path "%{buildroot}/var/tmp/kernel/devel/*" >> develfiles.pre   # for TIMA
find %{buildroot}/var/tmp/kernel/ -name '*.ko' -not -path "%{buildroot}/var/tmp/kernel/devel/*"  >> develfiles.pre      # for TIMA
find %{buildroot}/var/tmp/kernel/ -name '*%{IMAGE}' -not -path "%{buildroot}/var/tmp/kernel/devel/*"  >> develfiles.pre   # for Trusted Boot
cat develfiles.pre | sed -e "s#%{buildroot}##g" | uniq | sort > develfiles

%clean
rm -rf %_builddir

%files -n kernel-headers-%{KERNEL_VERSION}-%{CHIPSET}
/usr/include/*

%files -n linux-kernel-license
/usr/share/license/*

%files -n kernel-devel-%{KERNEL_VERSION}-%{CHIPSET} -f develfiles
/var/tmp/kernel/devel/*
/boot/kernel/devel/tizen-devel
