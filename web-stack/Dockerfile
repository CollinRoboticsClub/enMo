FROM python:slim-bookworm

WORKDIR /opt/app

COPY ./api/requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY ./api/*.py ./
COPY ./webui ./webui

EXPOSE 8000

# FIXME: change this back to 4 workers later once I've fixed some other issues first
CMD ["gunicorn", "--access-logfile", "-", "-k", "uvicorn.workers.UvicornWorker", "-w", "1", "--bind", "0.0.0.0:8000", "server:app", "server.py"]
