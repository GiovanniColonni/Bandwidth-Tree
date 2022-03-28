import networkx as nx
import matplotlib.pyplot as plt

def printTree():
    tree_r = list()
    tree = list()
    with open("tree.txt","r") as f:
        tree_r = f.readlines()
    # ToDo ! create nodes + labels
    # create list of edges
    for r in tree_r:
        r = r.split("\n")[0] # remove \n
        r = r.split(",")
        for c in r[1:]: 
            if(c != "0"):
                tree.append((r[0],c))
    tree_g = nx.Graph()
    tree_g.add_edges_from(tree)

    nx.draw(tree_g)
    plt.show()

if __name__ == "__main__":
    printTree()