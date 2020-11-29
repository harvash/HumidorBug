FROM postgres:12
MAINTAINER Harvash <harvash2004@yahoo.com>

ENV PG_MAJOR 12

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    postgresql-plpython3-$PG_MAJOR &&\
    mkdir -p /pgdb/logs &&\
    chown postgres:postgres /pgdb/logs