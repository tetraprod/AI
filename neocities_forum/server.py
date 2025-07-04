import json
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse

DATA = {"groups": []}
next_group_id = 1
next_post_id = 1

class Handler(BaseHTTPRequestHandler):
    def _set_json(self, status=200):
        self.send_response(status)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()

    def do_GET(self):
        parsed = urlparse(self.path)
        parts = parsed.path.strip('/').split('/')
        if parsed.path == '/groups':
            self._set_json()
            self.wfile.write(json.dumps(DATA["groups"]).encode())
            return
        if len(parts) == 2 and parts[0] == 'groups':
            try:
                gid = int(parts[1])
            except ValueError:
                self.send_error(400, 'Invalid group id')
                return
            group = next((g for g in DATA["groups"] if g["id"] == gid), None)
            if not group:
                self.send_error(404, 'Group not found')
                return
            self._set_json()
            self.wfile.write(json.dumps(group).encode())
            return
        self.send_error(404, 'Not found')

    def do_POST(self):
        length = int(self.headers.get('Content-Length', 0))
        body = self.rfile.read(length).decode()
        try:
            data = json.loads(body) if body else {}
        except json.JSONDecodeError:
            self.send_error(400, 'Invalid JSON')
            return
        parsed = urlparse(self.path)
        parts = parsed.path.strip('/').split('/')
        global next_group_id, next_post_id

        if parsed.path == '/groups':
            name = data.get('name')
            if not name:
                self.send_error(400, 'Missing name')
                return
            group = {"id": next_group_id, "name": name, "posts": [], "rules": []}
            next_group_id += 1
            DATA["groups"].append(group)
            self._set_json(201)
            self.wfile.write(json.dumps(group).encode())
            return

        if len(parts) == 3 and parts[0] == 'groups' and parts[2] == 'posts':
            try:
                gid = int(parts[1])
            except ValueError:
                self.send_error(400, 'Invalid group id')
                return
            group = next((g for g in DATA["groups"] if g["id"] == gid), None)
            if not group:
                self.send_error(404, 'Group not found')
                return
            text = data.get('text')
            if not text:
                self.send_error(400, 'Missing text')
                return
            post = {"id": next_post_id, "text": text}
            next_post_id += 1
            group['posts'].append(post)
            self._set_json(201)
            self.wfile.write(json.dumps(post).encode())
            return

        if len(parts) == 3 and parts[0] == 'groups' and parts[2] == 'rules':
            try:
                gid = int(parts[1])
            except ValueError:
                self.send_error(400, 'Invalid group id')
                return
            group = next((g for g in DATA["groups"] if g["id"] == gid), None)
            if not group:
                self.send_error(404, 'Group not found')
                return
            text = data.get('text')
            if not text:
                self.send_error(400, 'Missing text')
                return
            rule = {"text": text, "votes": 0}
            group['rules'].append(rule)
            self._set_json(201)
            self.wfile.write(json.dumps(rule).encode())
            return

        if len(parts) == 5 and parts[0] == 'groups' and parts[2] == 'rules' and parts[4] == 'vote':
            try:
                gid = int(parts[1])
                idx = int(parts[3])
            except ValueError:
                self.send_error(400, 'Invalid parameters')
                return
            group = next((g for g in DATA["groups"] if g["id"] == gid), None)
            if not group or idx < 0 or idx >= len(group['rules']):
                self.send_error(404, 'Rule not found')
                return
            delta = int(data.get('delta', 0))
            if delta not in (-1, 1):
                self.send_error(400, 'Invalid delta')
                return
            group['rules'][idx]['votes'] += delta
            self._set_json(200)
            self.wfile.write(json.dumps(group['rules'][idx]).encode())
            return

        self.send_error(404, 'Not found')

if __name__ == '__main__':
    server = ThreadingHTTPServer(('0.0.0.0', 8000), Handler)
    print('Serving on http://0.0.0.0:8000')
    server.serve_forever()
