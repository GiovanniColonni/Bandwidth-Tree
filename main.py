from sys import stdin
import networkx as nx
import matplotlib.pyplot as plt
import random
from subprocess import Popen, PIPE

def hierarchy_pos(G, root=None, width=3., vert_gap = 0.3, vert_loc = 0, xcenter = 0.5):

    '''
    From Joel's answer at https://stackoverflow.com/a/29597209/2966723.  
    Licensed under Creative Commons Attribution-Share Alike 
    
    If the graph is a tree this will return the positions to plot this in a 
    hierarchical layout.
    
    G: the graph (must be a tree)
    
    root: the root node of current branch 
    - if the tree is directed and this is not given, 
      the root will be found and used
    - if the tree is directed and this is given, then 
      the positions will be just for the descendants of this node.
    - if the tree is undirected and not given, 
      then a random choice will be used.
    
    width: horizontal space allocated for this branch - avoids overlap with other branches
    
    vert_gap: gap between levels of hierarchy
    
    vert_loc: vertical location of root
    
    xcenter: horizontal location of root
    '''
    if not nx.is_tree(G):
        raise TypeError('cannot use hierarchy_pos on a graph that is not a tree')

    if root is None:
        if isinstance(G, nx.DiGraph):
            root = next(iter(nx.topological_sort(G)))  #allows back compatibility with nx version 1.11
        else:
            root = random.choice(list(G.nodes))

    def _hierarchy_pos(G, root, width=1., vert_gap = 0.2, vert_loc = 0, xcenter = 0.5, pos = None, parent = None):
        '''
        see hierarchy_pos docstring for most arguments

        pos: a dict saying where all nodes go if they have been assigned
        parent: parent of this branch. - only affects it if non-directed

        '''
    
        if pos is None:
            pos = {root:(xcenter,vert_loc)}
        else:
            pos[root] = (xcenter, vert_loc)
        children = list(G.neighbors(root))
        if not isinstance(G, nx.DiGraph) and parent is not None:
            children.remove(parent)  
        if len(children)!=0:
            dx = width/len(children) 
            nextx = xcenter - width/2 - dx/2
            for child in children:
                nextx += dx
                pos = _hierarchy_pos(G,child, width = dx, vert_gap = vert_gap, 
                                    vert_loc = vert_loc-vert_gap, xcenter=nextx,
                                    pos=pos, parent = root)
        return pos

            
    return _hierarchy_pos(G, root, width, vert_gap, vert_loc, xcenter)
def printTree():
    tree_r = list()
    
    with open("tree.txt","r") as f:
        tree_r = f.readlines()
    # ToDo ! create nodes + labels
    nodes = list()
    
    root = tree_r[0].split(",")[0].split(";")[0][1:]
    labeldict = {}
    for r in tree_r:
        r = r.split(",")[0]
        r = r.split(";")
        nodes.append((r[0][1:],{"s":r[1],"e":r[2],"amb":r[3][:len(r[3])-1]}))
        labeldict[r[0][1:]] = f"[{r[1]},{r[2]})-{r[3][:len(r[3])-1]}"
    # create list of edges
    edges = list()
    for r in tree_r:
        r = r.split("\n")[0] # remove \n
        r = r.split(",")
        for c in r[1:]: 
            if(c != "0"):
                edges.append((r[0].split(";")[0][1:],c))

    tree_g = nx.Graph()

    tree_g.add_nodes_from(nodes)
    tree_g.add_edges_from(edges)
    
    pos = hierarchy_pos(tree_g,root)
    nx.draw(tree_g,labels=labeldict,with_labels=True,pos=pos)
    plt.show()

def test_that():
    # compile
    p = Popen("g++ main.cpp classes/Node.cpp -o bandwidth",shell=True)
    ret = p.wait()
    if(ret != 0):
        print("compilation errors..exit")
        exit(0)

    p = Popen("./bandwidth A 1 1",shell=True,stdin = PIPE)
    ret = p.wait()
    return ret

if __name__ == "__main__":
    ret = test_that()
    if(ret == 0):
        printTree()
    else:
        print("Errors occurs")
    exit(0)
    