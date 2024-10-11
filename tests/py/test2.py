print("######################################")
print("        Python Version                ")
print("######################################")

import swigex0 as sw

print("=========== Essai sur la classe MyFibo")
print("--- Constructeur: n'a pas de conversion par typemap")
ff = sw.MyFibo()
ff.display()
ff.setValue(12)
ff.display()

print("--- Methode create: pointeur preserve")
ff = sw.MyFibo.create()
ff.display()
ff.setValue(12)
ff.display()

print("--- Work in place: necessite le pointeur preserve")
ff = sw.MyFibo.create()
ff.display()
sw.MyFibo.workInPlaceRef(11, ff)
ff.display()
sw.MyFibo.workInPlacePtr(22, ff)
ff.display()

print("=========== Essai sur la class Fibo")
print("--- Constructeur: implique la conversion par typemap")
fp = sw.Fibo()
print(type(fp))
print(fp)

print("--- Methode create: pointeur preserve")
fp = sw.Fibo.create()
fp.display()
fp.setValue(14)
fp.display()
