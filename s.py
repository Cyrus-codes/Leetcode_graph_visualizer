import os
import sys
import subprocess
import json
import urllib.parse
from http.server import HTTPServer, BaseHTTPRequestHandler

SD = os.path.dirname(os.path.abspath(__file__))
P = 8000
BD = os.path.join(SD, "bin")
PE = os.path.join(BD, "a.exe" if os.name == "nt" else "a")

def cp():
    if os.path.exists(PE):
        return True
    try:
        if os.name == "nt":
            bs = os.path.join(SD, "b.ps1")
            subprocess.run(["powershell", "-ExecutionPolicy", "Bypass", "-File", bs], cwd=SD, check=True)
        else:
            subprocess.run(["make"], cwd=SD, check=True)
        return os.path.exists(PE)
    except Exception:
        return False

class H(BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        pass

    def do_GET(self):
        parsed_url = urllib.parse.urlparse(self.path)
        path = parsed_url.path

        if path in ["/", "/index.html", "/v/a.html", "/v/index.html"]:
            hp = os.path.join(SD, "v", "a.html")
            if not os.path.exists(hp):
                self.send_error(404)
                return

            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Cache-Control", "no-store, no-cache, must-revalidate")
            self.end_headers()
            
            with open(hp, "r", encoding="utf-8") as f:
                self.wfile.write(f.read().encode("utf-8"))
        else:
            self.send_error(404)

    def do_POST(self):
        parsed_url = urllib.parse.urlparse(self.path)
        if parsed_url.path == "/api/parse":
            cl = int(self.headers.get("Content-Length", 0))
            if cl == 0:
                self.sj({"error": "Empty body"}, 400)
                return

            body = self.rfile.read(cl).decode("utf-8")
            try:
                data = json.loads(body)
            except json.JSONDecodeError:
                self.sj({"error": "JSON error"}, 400)
                return

            inp = data.get("input", "").strip()
            fmt = data.get("format", "auto").strip()

            if not inp:
                self.sj({"error": "Empty input"}, 400)
                return

            if not os.path.exists(PE):
                if not cp():
                    self.sj({"error": "Compile failed"}, 500)
                    return

            try:
                proc = subprocess.run(
                    [PE, fmt, inp],
                    capture_output=True,
                    text=True,
                    timeout=5
                )
                
                output = proc.stdout.strip()
                if not output:
                    err = proc.stderr.strip() or "Empty stdout"
                    self.sj({"error": err}, 400)
                    return

                try:
                    res = json.loads(output)
                    sc = 400 if "error" in res else 200
                    self.sj(res, sc)
                except json.JSONDecodeError:
                    self.sj({"error": "JSON parse error", "raw": output}, 500)
            except subprocess.TimeoutExpired:
                self.sj({"error": "Timeout"}, 500)
            except Exception as e:
                self.sj({"error": str(e)}, 500)
        else:
            self.send_error(404)

    def sj(self, d, s=200):
        self.send_response(s)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(json.dumps(d, indent=2).encode("utf-8"))

def ss():
    cp()
    server_address = ("", P)
    httpd = HTTPServer(server_address, H)
    print(f"http://localhost:{P}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        httpd.server_close()
        sys.exit(0)

if __name__ == "__main__":
    ss()
