FROM gitpod/workspace-full:build-branch-master

USER root

# Install packages
RUN apt-get update \
    && apt-get install -yq --no-install-recommends \
        python3=3.7.3-1 \
        python3-pip=18.1-5 \
        python3-setuptools=40.8.0-1 \
        python3-wheel=0.32.3-2 \
        ninja-build=1.8.2-1 \
        valgrind=1:3.14.0-2ubuntu6 \
        ccache=3.6-1 \
    && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* \
    && /usr/sbin/update-ccache-symlinks

USER gitpod

# Install pip packages
RUN pip3 install meson==0.53.0 gcovr==4.2
