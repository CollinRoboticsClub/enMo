FROM python:slim-bookworm

WORKDIR /opt/app

COPY ./api/requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY ./api/server.py ./server.py
COPY ./webui ./webui

EXPOSE 8000
CMD ["python", "server.py"]
