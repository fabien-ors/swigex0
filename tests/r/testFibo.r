library(swigex0)

f1 = Fibo_create(50)
invisible(f1$display(FALSE))
f2 = Fibo_create(100, "Test 100")
invisible(f2$display())

vec = fib(40)
print(vec)
