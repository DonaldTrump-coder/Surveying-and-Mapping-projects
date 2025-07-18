import matplotlib.pyplot as plt

file=open("C:\\Users\\10527\\Desktop\\程序设计\\result.txt")
for line in file:
    line=line.strip()
    points=line.split(',')
    x1=float(points[0])
    y1=float(points[1])
    x2=float(points[2])
    y2=float(points[3])
    x3=float(points[4])
    y3=float(points[5])
    plt.plot([x1,x2,x3,x1],[y1,y2,y3,y1],marker='o',linestyle='-',color='blue')
plt.show()