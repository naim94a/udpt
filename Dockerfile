FROM rust:latest

ARG BUILD_ARGS

COPY . /usr/src/udpt
WORKDIR /usr/src/udpt

RUN cargo build --release ${BUILD_ARGS}

CMD ["target/release/udpt-rs", "-c", "/usr/src/udpt/udpt.toml"]
