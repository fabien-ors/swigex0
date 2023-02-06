library(swigex)

f1 = Fibo(50)
invisible(f1$display(FALSE))
f2 = Fibo(100, "Test 100")
invisible(f2$display())

vec = fib(40)
print(vec)
