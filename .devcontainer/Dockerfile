# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.183.0/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version: debian-10, debian-9, ubuntu-20.04, ubuntu-18.04
# ARG VARIANT="buster"
# FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

FROM fedora
# c++ develop
RUN dnf install --nodocs -y ninja-build gcc gcc-c++ cmake gdb lldb clang git && dnf clean all
# qt develop
RUN dnf install --nodocs -y  `dnf search qt5 | grep '^qt5' | grep 'devel.x86' | awk -F: '{print $1}' | grep -v private` && dnf clean all
# digikam
RUN dnf download digikam-libs && rpm -i --nodeps `ls | grep digikam-libs` && dnf install --nodocs -y digikam-devel && dnf clean all

# 创建用户
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=${USER_UID}

RUN groupadd -g ${USER_GID} ${USERNAME}\
    && useradd -u ${USER_UID} -g ${USER_GID} -m ${USERNAME} \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME
