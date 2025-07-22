FROM python:3.11-slim

WORKDIR /workspace/AI

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

EXPOSE 8000

CMD ["python", "-m", "unified_ai"]
