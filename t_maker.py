import binascii

def bitstring_to_bytes(s):
    return int(s, 2).to_bytes((len(s) + 7) // 8, byteorder='big')

print("enter test name: ",end="")
filename= input()
filename += ".x2017"
print('enter assembly cdoe')
a = ""
output = ""
while True:
    a = input()
    if a == "quit":
        break
    elif a == "val":
        output += "00"
    elif a == "reg":
        output+= "01"
    elif a == "stk":
        output+= "10"
    elif a == "ptr":
        output+= "11"
    elif a == "mov":
        output+= "000"
    elif a == "cal":
        output+= "001"
    elif a == "ret":
        output += "010"
    elif a == "ref":
        output+="011"
    elif a == "add":
        output +="100"
    elif a =="print":
        output+="101"
    elif a == "not":
        output+="110"
    elif a == "equ":
        output+="111"
    else:
        output += a

leftover = len(output)%8
while leftover != 0:
    output = "0" + output
    leftover = len(output)%8
#print(output)
#print(hex(int(output, 2))[2:])
#binstr =bytes.fromhex(hex(int(output, 2))[2:])

with open(filename,'wb') as f:
    f.write(bitstring_to_bytes(output))