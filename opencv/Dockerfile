FROM python:slim-bookworm

WORKDIR /opt/app

COPY ./requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY ./*.py ./

EXPOSE 8000

CMD ["uvicorn", "--host", "0.0.0.0", "--port", "8000", "camera:app"]
