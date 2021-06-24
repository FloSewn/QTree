import numpy as np
from matplotlib import pyplot as plt
from matplotlib.patches import Rectangle
import sys, os

class QTree:

    def __init__(self, index, center, scale, n_items):
        self.index   = index
        self.center  = center
        self.scale   = scale
        self.n_items = n_items

        half_scale  = scale * 0.5
        self.xy_min = ( center[0] - half_scale,
                        center[1] - half_scale )
        self.xy_max = ( center[0] + half_scale,
                        center[1] + half_scale )

    def plot(self, ax, r_args={}, show_id=False):
        r = Rectangle(self.xy_min, self.scale, self.scale, **r_args)
        ax.add_patch( r )
        if show_id:
            ax.text(self.center[0], self.center[1], self.index)


def clear_comments(lines):
    for i in range(len(lines)-1, 0, -1):
        line = lines[i].replace(' ','')
        if line[0] == '#':
            lines.pop( i )

def read_vertices(lines):
    vertices = []
    start, n_vert = 0, 0

    for i, line in enumerate(lines):
        line = line.split(" ")
        if line[0] == "VERTICES":
            start  = i + 1
            n_vert = int(line[1])
            break

    for i in range(start, start+n_vert):
        line = lines[i]
        (x,y) = ( float(s) for s in line.split(',') )
        vertices.append( (x,y) )

    return vertices


def read_qtrees( lines ):
    qtrees = []
    start, n_qtree = 0, 0

    for i, line in enumerate(lines):
        line = line.split(" ")
        if line[0] == "QTREE":
            start  = i + 1
            n_qtree = int(line[1])
            break

    for i in range(start, start+n_qtree):
        line = lines[i]
        (cx,cy,s,n) = ( float(s) for s in line.split(',') )
        qtrees.append( QTree( i, (cx, cy), s, n) )

    return qtrees




def main():
    if len(sys.argv) < 2:
        print("plot_quadtree.py <quadtree.txt>")
        sys.exit(1)

    file_name = sys.argv[1]

    with open(file_name, 'r') as f:
        lines = f.readlines()

    clear_comments( lines )
    vertices = np.array( read_vertices( lines ) )
    qtrees = read_qtrees( lines )


    fig, ax = plt.subplots(1,1,dpi=200)
    for q in qtrees:
        q.plot(ax, {'color': 'k', 'fill': False})

    ax.scatter( vertices[:,0], vertices[:,1], marker='x', color='r' )

    ax.set_aspect('equal')

    plt.show()







if __name__ == '__main__': main()
