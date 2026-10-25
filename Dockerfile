###############################################################################
# Docker image with the complete CHASE toolchain:
#   - CHASE core-library, chase-backends, DSL_tool, logics_tool (+ Python API)
#   - external solvers used by the backends: gr1c, slugs, NuSMV (x86_64 only)
#
# Build (from the root of the core-library repository):
#   docker build -t chase .
#
# Run the demos:
#   docker run --rm -it chase
#   # inside the container: cd /opt/chase/demos/DATE21 && ...
#
# The other CHASE modules are cloned from GitHub. To build the image from
# local checkouts instead, override the corresponding stage, e.g.:
#   docker build -t chase \
#     --build-context third_party-src=../third_party \
#     --build-context chase-backends-src=../chase-backends \
#     --build-context dsl_tool-src=../DSL_tool \
#     --build-context logics_tool-src=../logics_tool \
#     --build-context demos-src=../demos .
###############################################################################

ARG UBUNTU_VERSION=22.04

FROM ubuntu:${UBUNTU_VERSION} AS base
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake git ca-certificates curl pkg-config \
        python3 python3-dev pybind11-dev uuid-dev \
        bison flex libboost-dev graphviz less \
    && rm -rf /var/lib/apt/lists/*

###############################################################################
# Sources of the CHASE modules (each stage can be overridden with
# --build-context <name>=<path>).
###############################################################################
FROM base AS fetch
ARG CHASE_REF=main
RUN for m in third_party chase-backends DSL_tool logics_tool demos; do \
        git clone --depth 1 --branch ${CHASE_REF} \
            https://github.com/chase-cps/$m /src/$m || \
        git clone --depth 1 https://github.com/chase-cps/$m /src/$m; \
    done

FROM scratch AS core-library-src
COPY . /
FROM scratch AS third_party-src
COPY --from=fetch /src/third_party /
FROM scratch AS chase-backends-src
COPY --from=fetch /src/chase-backends /
FROM scratch AS dsl_tool-src
COPY --from=fetch /src/DSL_tool /
FROM scratch AS logics_tool-src
COPY --from=fetch /src/logics_tool /
FROM scratch AS demos-src
COPY --from=fetch /src/demos /

###############################################################################
# External solvers.
###############################################################################
FROM base AS solvers
ARG GR1C_REF=v0.13.3
ARG SLUGS_REF=ac09bf652d4ff47a7018d6a41627af73a67a049c
ARG TARGETARCH

# gr1c (GR(1) synthesis)
RUN git clone https://github.com/tulip-control/gr1c /tmp/gr1c \
    && cd /tmp/gr1c && git checkout ${GR1C_REF} \
    && ./get-deps.sh && ./build-deps.sh \
    && make -j"$(nproc)" && make install prefix=/usr/local \
    && rm -rf /tmp/gr1c

# slugs (GR(1) synthesis) and its structured-slugs compiler
RUN git clone https://github.com/VerifiableRobotics/slugs /opt/slugs \
    && cd /opt/slugs && git checkout ${SLUGS_REF} \
    && sed -i 's/#ifndef __x86_64/#if !defined(__x86_64) \&\& !defined(__aarch64__)/' \
        src/BFAbstractionLibrary/compilerOptionGenerator.c \
    && cd src && make -j"$(nproc)" \
    && ln -s /opt/slugs/src/slugs /usr/local/bin/slugs

# NuSMV (model checking). Upstream only ships x86_64 Linux binaries.
RUN if [ "${TARGETARCH}" = "amd64" ]; then \
        curl -fsSL -o /tmp/nusmv.tgz \
            https://nusmv.fbk.eu/distrib/NuSMV-2.6.0-linux64.tar.gz \
        && echo "11b12e42bc04566c4ec1f7cc205fa8704382ac4d69e052154cbb59c9b7e8e08c  /tmp/nusmv.tgz" | sha256sum -c - \
        && tar xzf /tmp/nusmv.tgz -C /opt \
        && ln -s /opt/NuSMV-2.6.0-Linux/bin/NuSMV /usr/local/bin/NuSMV \
        && rm /tmp/nusmv.tgz; \
    else \
        echo "NuSMV: no prebuilt binary for ${TARGETARCH}, skipping."; \
    fi

###############################################################################
# CHASE.
###############################################################################
FROM solvers AS chase
ENV CHASE_INSTALL_DIR=/opt/chase
ARG BUILD_TYPE=Release

COPY --from=third_party-src    / /src/third_party
COPY --from=core-library-src   / /src/core-library
COPY --from=chase-backends-src / /src/chase-backends
COPY --from=dsl_tool-src       / /src/DSL_tool
COPY --from=logics_tool-src    / /src/logics_tool
COPY --from=demos-src          / /opt/chase/demos

# Install order matters: third_party and core-library first.
RUN for m in third_party core-library chase-backends DSL_tool logics_tool; do \
        echo "=== Building $m" \
        && cmake -S /src/$m -B /build/$m \
            -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
            -Dpybind11_DIR=/usr/lib/cmake/pybind11 \
        && cmake --build /build/$m -j"$(nproc)" \
        && cmake --install /build/$m || exit 1; \
    done \
    && (cd /build/core-library && ctest --output-on-failure) \
    && rm -rf /build /src

ENV PATH=/opt/chase/chase/bin:${PATH} \
    PYTHONPATH=/opt/chase/chase/lib \
    SLUGSCOMPILER=/opt/slugs/tools/StructuredSlugsParser/compiler.py

WORKDIR /opt/chase/demos
CMD ["/bin/bash"]
