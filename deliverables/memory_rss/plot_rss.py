import subprocess
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

#CHANGE THESE VARIABLES TO PERFORM OTHER TESTS
algorithms = {"boost","tarjan","nuutila1","nuutila1_iterative", "nuutila2","nuutila2_iterative", "pearce1", "pearce2", "pearce2_iterative"}
vertexes = {1000, 3000, 5000, 10000}
edges = {0.01, 0.2, 0.7}
n_iterations = 7
path_bin = "./SCCApp"
file_out = "tmp.txt"

#running the algorithm
for vertex in vertexes:
    for iteation in range(0, n_iterations):
        for algorithm in algorithms:
            for edge in edges:
                command = path_bin+" --mem-analysis "+algorithm+" "+str(vertex)+" "+str(edge)+" "+file_out
                print(command)
                subprocess.call(command, shell=True)

#getting the results
df = pd.read_csv(file_out, sep=",", names = ["vertex", "edge", "sccs", "mem_sw", "mem_peak", "mem_algorithm (byte)", "algorithm"])
remove_command = "rm "+file_out
subprocess.call(remove_command, shell=True)

#plot

paper_rc = {'lines.linewidth': 2.4, 'lines.markersize': 0.2}
sns.set_context("notebook", rc=paper_rc, font_scale=2.3)
g = sns.factorplot(data=df, col="vertex", x="edge", y="mem_algorithm (byte)", hue="algorithm", col_wrap=2, aspect=1,
                   legend_out=True, size=10)  # , palette="YlGnBu_d")#, kind="point")#, estimator=median, orient="v")
g.set_xticklabels(rotation=30)

n_ticks = []
for ax in g.axes.flat:
    n_ticks = ax.get_xticks()

x_vals = np.linspace(1, n_ticks[-1], num=len(n_ticks))# // 3)
g.set(xticks=x_vals)

g.savefig("output.png")