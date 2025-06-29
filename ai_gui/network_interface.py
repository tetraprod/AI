import requests


API_ENDPOINT = 'https://httpbin.org/post'


def send_request(payload: dict):
    """Send payload to remote endpoint (placeholder)."""
    try:
        response = requests.post(API_ENDPOINT, json=payload, timeout=5)
        response.raise_for_status()
        return response.json()
    except Exception as e:
        print(f'Network request failed: {e}')
        return None
