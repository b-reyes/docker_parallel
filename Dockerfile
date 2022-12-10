FROM alpine:latest
RUN apk upgrade && apk update && apk add --no-cache g++ openmpi openmpi-dev openssh make
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1
CMD /bin/sh