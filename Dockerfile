FROM alpine:latest
RUN apk upgrade && apk update && apk add --no-cache g++ openmpi openmpi-dev openssh make
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1
WORKDIR /usr/include
RUN wget -O Eigen.zip https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip && unzip Eigen.zip && rm Eigen.zip 
WORKDIR /usr/code
CMD /bin/sh