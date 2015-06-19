#!/usr/bin/python
# -*- coding: iso-8859-1 -*-

import Tkinter

def getColorFromInt(c):
	if c < -40:
		return '#000020'
	elif c < -10:
		return '#0000' + '%02x' % (64 + c/2)
	elif c < 1:
		return '#000080'
	elif c < 10:
		return '#FFFF' + '%02x' % (180 - c*5)
	elif c < 25:
		return '#00' + '%02x' % (128 - c*4) + '00'
	elif c < 60:
		return '#' + '%02x' % (139 - c*2) + '%02x' % (69 - c) + '%02x' % (20 - c/3)
	elif c < 100:
		return '#' + '%02x' % (155 + c) + '%02x' % (155 + c) + '%02x' % (155 + c)
	return 'white'

class simpleapp_tk(Tkinter.Tk):
	def __init__(self,parent):
		Tkinter.Tk.__init__(self,parent)
		self.parent = parent
		self.initialize()
		self.minsize(width = 512, height = 512)

	def initialize(self):
		self.grid()

		button = Tkinter.Button(self,text = u"Load file",
					command = self.loadFile)
		button.grid(column = 1, row = 0)
		self.canvas = Tkinter.Canvas(self, width = 512, height = 512)
		self.canvas.grid(column = 1,row = 1)

		self.grid_columnconfigure(0,weight = 1)
		self.resizable(True,True)
		self.update()
		self.geometry(self.geometry())

	def loadFile(self):
		f = open('1.txt', 'r')
		len = int(f.readline())
		nl = 0
		for line in f:
			nc = 0
			for i in line.split():
				self.canvas.create_line(nc, nl, nc, nl + 1, fill = getColorFromInt(int(i)))
				nc += 1
			nl += 1

	def OnPressEnter(self,event):
		self.labelVariable.set( self.entryVariable.get()+" (You pressed ENTER)" )
		self.entry.focus_set()
		self.entry.selection_range(0, Tkinter.END)

if __name__ == "__main__":
	app = simpleapp_tk(None)
	app.title('Landscape python GUI')
	app.mainloop()
