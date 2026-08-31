import subprocess

while True:
	with open("in", "w") as data:
		subprocess.run(["python3", "data.py"], stdout=data, check=True)
	with open("in") as data, open("out", "w") as output:
		subprocess.run(
			["/usr/bin/time", "-f", "test Time=%es", "./test"],
			stdin=data, stdout=output, check=True)
	with open("in") as data, open("ans", "w") as answer:
		subprocess.run(
			["/usr/bin/time", "-f", "std Time=%es", "./std"],
			stdin=data, stdout=answer, check=True)
	result = subprocess.run(["diff", "-u", "out", "ans"])
	if result.returncode > 1:
		result.check_returncode()
	if result.returncode == 1:
		print("WA")
		raise SystemExit(result.returncode)
	print("AC")
