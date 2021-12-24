#!/usr/bin/env python
 
import tkinter as tk  
import serial
#import serial.tools.list_ports
import os
from random import Random
from time import sleep
import threading


master_ser = serial.Serial("COM30",115200,timeout=1)
 
window = tk.Tk()
 
window.title('My Window')

window.geometry('500x300')
 
cmd = [0x00,0x00]

l = tk.Label(window, bg='green', fg='white', width=20, text='手环调试工具')
l.pack()


def print_selection(v):
    l.config(text='you have selected ' + v)
    print(v)
    v_str = 'AT+TP+VAL=' + str(v) + '\r\n'
    master_ser.write(v_str.encode("utf-8"))
    
def button_up():
    master_ser.write(('AT+TP+UP\r\n').encode("utf-8"))
def button_down():
    master_ser.write(('AT+TP+DOWN\r\n').encode("utf-8"))
def button_left():
    master_ser.write(('AT+TP+LEFT\r\n').encode("utf-8"))
def button_right():
    master_ser.write(('AT+TP+RIGHT\r\n').encode("utf-8"))
def button_fun_1():
    master_ser.write(('AT+TP+F1\r\n').encode("utf-8"))
def button_fun_2():
    master_ser.write(('AT+TP+F2\r\n').encode("utf-8"))
def button_fun_3():
    master_ser.write(('AT+TP+F3\r\n').encode("utf-8"))

s = tk.Scale(window, label='try me', from_=0, to=390, orient=tk.HORIZONTAL, length=400, showvalue=0,tickinterval=40, resolution=1, command=print_selection)
s.pack()

b_0 = tk.Button(window, text = "上键", command = button_up)
b_0.place(x = 100, y = 100, width=40, height=25)
#b_0.pack()

b_1 = tk.Button(window, text = "下键", command = button_down)
b_1.place(x = 100, y = 200, width=40, height=25)

b_2 = tk.Button(window, text = "左键", command = button_left)
b_2.place(x = 180, y = 150, width=40, height=25)

b_3 = tk.Button(window, text = "右键", command = button_right)
b_3.place(x = 20, y = 150, width=40, height=25)

b_3 = tk.Button(window, text = "功能一", command = button_fun_1)
b_3.place(x = 400, y = 100, width=40, height=25)

b_3 = tk.Button(window, text = "功能二", command = button_fun_2)
b_3.place(x = 400, y = 150, width=40, height=25)

b_3 = tk.Button(window, text = "功能三", command = button_fun_3)
b_3.place(x = 400, y = 200, width=40, height=25)
#b_1.pack()




def mouse_press(event):
    print('鼠标按下')
    master_ser.write(('AT+TP+P\r\n').encode("utf-8"))

def mouse_release(event):
    print('鼠标抬起')
    master_ser.write(('AT+TP+Rel\r\n').encode("utf-8"))

s.bind('<ButtonPress-1>',mouse_press)
s.bind('<ButtonRelease-1>',mouse_release)
 

window.mainloop()
master_ser.close()
print("测试程序结束!")

