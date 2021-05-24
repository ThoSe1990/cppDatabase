#!/usr/bin/python3.8

import sys

import pythonDatabase


pythonDatabase.Connect()

cmdLineArguments = sys.argv
print ("deleting value: " , cmdLineArguments[1])

pythonDatabase.Delete(cmdLineArguments[1])


pythonDatabase.Disconnect()
