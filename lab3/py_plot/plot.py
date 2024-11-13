import subprocess

benchIterations = [2, 4, 8, 16, 32, 64, 128, 256, 1000, 1500, 2500, 5000, 10000, 50000]
args1 = []
for i in range(0, len(benchIterations)):
    args1.append("isort" + str(i) + ".json")
    args1.append("qsort" + str(i) + ".json")

subprocess.run(['python', 'py_plot/pyperfplot.py', *args1])

# & C:/Users/Анатолий/AppData/Local/Programs/Python/Python312/python.exe c:/itmo_lab/lab3/lab3/lab3/py_plot/pyperfplot.py isort0.json qsort0.json isort1.json qsort1.json isort2.json qsort2.json isort3.json qsort3.json isort4.json qsort4.json isort5.json qsort5.json isort6.json qsort6.json isort7.json qsort7.json isort8.json qsort8.json isort9.json qsort9.json isort10.json qsort10.json isort11.json qsort11.json isort12.json qsort12.json isort13.json qsort13.json