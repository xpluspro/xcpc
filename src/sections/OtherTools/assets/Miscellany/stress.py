import os

while True:
    os.system("python3 data.py > in")
    os.system("/usr/bin/time -f 'test Time=%es' ./test < in > out")
    os.system("/usr/bin/time -f 'std Time=%es' ./std < in > ans")
    if os.system("diff out ans >/dev/null"):
        print("WA")
        raise SystemExit(1)
    print("AC")
