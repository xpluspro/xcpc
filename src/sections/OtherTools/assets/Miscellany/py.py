def ItertoolsExamples():
	from itertools import product, permutations, combinations
	from itertools import combinations_with_replacement
	product('ABCD', 'xy')
	product(range(2), repeat=3)
	permutations('ABCD', 2)
	combinations('ABCD', 2)
	combinations_with_replacement('ABC', 2)

def RandomExamples():
	from random import randint, choice, sample, shuffle
	from random import choices, binomialvariate, normalvariate
	N = 10
	randint(1, N)                   # [1, N] 内的随机整数
	choice([1, 2, 3, 5, 8])        # 随机选择一个元素
	sample([1, 2, 3, 4, 5], k=2)  # 随机抽样两个元素
	a = list(range(N))
	shuffle(a)                      # 原地打乱列表
	l, r = sorted(choices(range(1, N + 1), k=2))
	binomialvariate(N, 0.5)
	normalvariate(0.0, 1.0)

def ListOperations():
	from functools import cmp_to_key
	a = list(range(100000))[::10]  # 每隔十个元素取一个
	a.sort()                        # 原地排序
	a.sort(key=lambda x: x % 10)   # 按末尾排序
	a.sort(key=cmp_to_key(lambda x, y: y - x))
	b = sorted(a)                   # 非原地排序
	a.reverse()                     # 原地翻转
	return b

def DictionaryOperations():
	from collections import defaultdict
	d = defaultdict(list)
	d['a'].append(2)
	d['a'].append(3)
	d['b'].append(4)
	print(d)
	d = defaultdict(lambda: 2)     # 自定义缺省值
	for key, value in d.items():
		print(key, value)

def FractionOperation():
	from fractions import Fraction
	a = Fraction(233, 1000)
	print(a.numerator, a.denominator, str(a))
	return Fraction(0.233).limit_denominator(1000)

def DecimalOperation():
	from decimal import Decimal, getcontext, FloatOperation
	from decimal import ROUND_HALF_EVEN
	getcontext().prec = 100
	getcontext().rounding = ROUND_HALF_EVEN
	getcontext().traps[FloatOperation] = True
	a = Decimal('114514.1919810')
	print(a, f'{a:.2f}')
	print(a.ln(), a.log10(), a.sqrt(), a ** 2)

def ComplexOperation():
	a = 1 + 2j
	print(a.real, a.imag, abs(a), a.conjugate())

def MemoizedSearch():
	from functools import cache

	@cache
	def fib(n):
		if n <= 2:
			return 1
		return fib(n - 1) + fib(n - 2)
	return fib

def FastIO():
	import sys, atexit, io
	_INPUT_LINES = sys.stdin.read().splitlines()
	input = iter(_INPUT_LINES).__next__
	_OUTPUT_BUFFER = io.StringIO()
	sys.stdout = _OUTPUT_BUFFER
	@atexit.register
	def write():
		sys.__stdout__.write(_OUTPUT_BUFFER.getvalue())
