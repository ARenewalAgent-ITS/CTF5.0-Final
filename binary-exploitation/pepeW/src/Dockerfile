FROM node:latest
ARG flag=IHECKINLOVERACECONDITION


RUN apt-get update && \
    apt-get install -y golang

ENV GOPATH /go
ENV PATH $GOPATH/bin:/usr/local/go/bin:$PATH

RUN useradd $flag

WORKDIR /home/$flag
COPY app/ /home/$flag

RUN npm install
RUN go build main.go

RUN chown -R root:$flag /home/$flag
RUN chmod -R 777 /home/$flag

USER $flag

EXPOSE 3000
CMD ["node", "script.js"]
