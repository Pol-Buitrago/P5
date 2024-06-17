increment = (440/44100)*40
index = 0
for i in range(40):
    print(index)
    increment += increment
    index = round(increment)
