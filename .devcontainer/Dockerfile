FROM fedora

RUN echo -e 'assumeyes=True\ninstall_weak_deps=False\ntsflags=nodocs' >> /etc/dnf/dnf.conf\
    & dnf in ninja-build gcc gcc-c++ cmake qt5-qtbase-devel digikam-devel spdlog-devel\
    & dnf autoremove\
    & dnf clean all

