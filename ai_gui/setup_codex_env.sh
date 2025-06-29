#!/bin/bash

# Setup Codex-like unrestricted Python environment
echo "Creating virtual environment..."
python3 -m venv codex_env
source codex_env/bin/activate

echo "Upgrading pip..."
pip install --upgrade pip

echo "Installing core libraries..."
pip install numpy matplotlib pandas nltk tqdm requests flask

echo "Downloading English quadgram data for crypto scoring..."
mkdir -p data
curl -o data/english_quadgrams.txt https://practicalcryptography.com/media/cryptanalysis/files/english_quadgrams.txt

echo "Creating starter Python script..."
cat << EOF > kryptos_solver.py
import itertools, math
from string import ascii_uppercase

def load_quadgrams(path='data/english_quadgrams.txt'):
    with open(path) as f:
        q = {}
        for line in f:
            k, v = line.split()
            q[k] = int(v)
    total = sum(q.values())
    log_total = math.log10(total)
    return q, log_total

def score(text, quad, log_total):
    return sum(math.log10(quad.get(text[i:i+4], 0.1)) - log_total for i in range(len(text)-3))

def decrypt(text, key):
    k = itertools.cycle(key)
    return ''.join(chr(((ord(c)-ord('A')) - (ord(next(k))-ord('A'))) % 26 + ord('A')) if c.isalpha() else c for c in text)

quad, log_total = load_quadgrams()
ciphertext = "OSQSKPZUBOPJLVXEKLRQUTTKRINSDTJCUFGSIMCAOBKEAZDRXBSKWFIOWSZIPGGFOZNKKHLTWFWUURWABGHLVTTNDUBQQJYKA"

best_score, best_key, best_plain = -1e9, "", ""
for key in itertools.product(ascii_uppercase, repeat=3):
    k = ''.join(key)
    plain = decrypt(ciphertext, k)
    s = score(plain, quad, log_total)
    if s > best_score:
        best_score, best_key, best_plain = s, k, plain

print(f"Best Key: {best_key}\\nScore: {best_score}\\nDecrypted: {best_plain[:100]}...")
EOF

echo "Ready. Activate with: source codex_env/bin/activate"
echo "Run solver with: python kryptos_solver.py"
