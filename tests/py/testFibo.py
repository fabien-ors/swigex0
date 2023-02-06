import swigex

f1 = swigex.Fibo(50)
f1.display(False)
f2 = swigex.Fibo(100, "Test 100")
f2.display()

vec = swigex.fib(40)
print(vec)
