FROM python:3.12

COPY *.py *.h *.cpp Makefile /app/
COPY .git /app/.git/

RUN cd /app && make

WORKDIR /app
