FROM python:slim-bookworm

# Install packages
# i2c-tools             : arm servos
# gcc, libc6-dev, make  : building wheel packages using pip
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
	apt-get -y upgrade && \
	apt-get -y install --no-install-recommends \
	i2c-tools gcc libc6-dev make && \
	rm -rf /var/lib/apt/lists/*

WORKDIR /opt/app

COPY ./api/requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY ./api/*.py ./
COPY ./webui ./webui

EXPOSE 8000

# FIXME: change this back to 4 workers later once I've fixed some other issues first
CMD ["gunicorn", "--access-logfile", "-", "-k", "uvicorn.workers.UvicornWorker", "-w", "1", "--bind", "0.0.0.0:8000", "server:app", "server.py"]
