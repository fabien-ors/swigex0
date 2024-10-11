import swigex0 as sw

f1 = sw.Fibo.create(50)
f1.display(False)
f2 = sw.Fibo.create(100, "Test 100")
f2.display()

vec = sw.fib(40)
print(vec)
