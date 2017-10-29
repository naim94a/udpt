FROM gcc

RUN \
  apt-get update && \
  DEBIAN_FRONTEND=noninteractive \
    apt-get -y install \
      cmake \
      libboost-program-options-dev \
      libboost-thread-dev \
      libsqlite3-dev \
  && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/

COPY . /usr/src/udpt
WORKDIR /usr/src/udpt

RUN \
  cmake -DCMAKE_BUILD_TYPE=Release . && \
  make udpt -j8

ENTRYPOINT [ "./udpt", "--interactive" ]
