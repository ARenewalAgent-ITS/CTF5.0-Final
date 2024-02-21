# export DOCKER_DEFAULT_PLATFORM=linux/amd64
# docker build -t so-many-random .
# docker run -d -it --rm -p 8001:1337 so-many-random
FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update --fix-missing -y
RUN apt-get install -y make socat python3-pip

RUN adduser --disabled-password --gecos "" user
RUN pip3 install pycryptodome

WORKDIR /home/user/

COPY chall.py chall.py

EXPOSE 1337
USER user

ENTRYPOINT [ "socat", "tcp-listen:1337,reuseaddr,fork", "EXEC:'timeout 60 stdbuf -i0 -o0 -e0 python3 chall.py'" ]