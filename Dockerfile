FROM debian:bookworm

RUN apt update && apt install -y bison flex curl gcc g++ make texinfo zlib1g-dev g++ unzip bzip2 xz-utils git

RUN mkdir -p /tmp/djgpp
RUN git clone https://github.com/andrewwutw/build-djgpp.git /tmp/djgpp/build-djgpp
RUN cd /tmp/djgpp/build-djgpp && ./build-djgpp.sh 12.2.0

COPY ./docker/build.sh /build.sh

CMD ["/build.sh"]
