FROM ubuntu:latest

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y build-essential cmake libpq-dev libpqxx-dev

COPY . /app
WORKDIR /app

RUN mkdir build
RUN cd build && cmake ..
RUN cd build && make
RUN mkdir -p /app/bin && cp /app/build/bin/db_manager /app/bin/db_manager

ENTRYPOINT ["/app/bin/db_manager"]

CMD ["/app/bin/db_manager"]
