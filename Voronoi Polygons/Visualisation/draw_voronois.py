import matplotlib.pyplot as plt

file=open("C:\\Users\\10527\\Desktop\\程序设计\\result.txt")
for line in file:
    line=line.strip()
    points=line.split(',')
    points_num=(points.__len__()-1)/2
    x_list=[]
    y_list=[]
    for i in range(int(points_num)):
        x_list.append(float(points[2*i]))
        y_list.append(float(points[2*i+1]))
    x_list.append(float(points[0]))
    y_list.append(float(points[1]))
    plt.plot(x_list,y_list,marker='o',linestyle='-',color='blue')
plt.show()