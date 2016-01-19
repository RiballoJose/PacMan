import sys

def read_level():
        f = open(sys.argv[1], 'r')
        p = open('./../niveles.txt', 'a')
        matrix = []
        line ='\n'+"*"*28+'\n'
        while True:
            c = f.read(1)
            if not c:
                p.write('\n'+'*'*28)
                f.close()
                p.close()
                break
            elif c=='\n':
                p.write(line)
                for i in reversed(matrix):
                    p.write(i)
                matrix = []
                line = "\n"
            else:
                matrix.append(c)
                line += c

read_level()
