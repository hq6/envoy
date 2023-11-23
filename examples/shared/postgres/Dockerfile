FROM postgres:latest@sha256:ae77c56d006cadd9f612fcb7b53863d601d15b1db5bbac0e3de4f59a1103314f
COPY docker-healthcheck.sh /usr/local/bin/
HEALTHCHECK CMD ["docker-healthcheck.sh"]
