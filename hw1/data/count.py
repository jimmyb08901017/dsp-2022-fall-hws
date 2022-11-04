num = [0,0,0,0,0,0]

with open("train_seq_01.txt") as f:
    contents = f.read()
    for x in contents:
        if x == 'A':
            num[0] += 1
        elif x == 'B':
            num[1] += 1
        elif x == 'C':
            num[2] += 1
        elif x == 'D':
            num[3] += 1
        elif x == 'E':
            num[4] += 1
        elif x == 'F':
            num[5] += 1
        else:
            continue

with open("train_seq_02.txt") as f:
    contents = f.read()
    for x in contents:
        if x == 'A':
            num[0] += 1
        elif x == 'B':
            num[1] += 1
        elif x == 'C':
            num[2] += 1
        elif x == 'D':
            num[3] += 1
        elif x == 'E':
            num[4] += 1
        elif x == 'F':
            num[5] += 1
        else:
            continue

with open("train_seq_03.txt") as f:
    contents = f.read()
    for x in contents:
        if x == 'A':
            num[0] += 1
        elif x == 'B':
            num[1] += 1
        elif x == 'C':
            num[2] += 1
        elif x == 'D':
            num[3] += 1
        elif x == 'E':
            num[4] += 1
        elif x == 'F':
            num[5] += 1
        else:
            continue

with open("train_seq_04.txt") as f:
    contents = f.read()
    for x in contents:
        if x == 'A':
            num[0] += 1
        elif x == 'B':
            num[1] += 1
        elif x == 'C':
            num[2] += 1
        elif x == 'D':
            num[3] += 1
        elif x == 'E':
            num[4] += 1
        elif x == 'F':
            num[5] += 1
        else:
            continue

with open("train_seq_05.txt") as f:
    contents = f.read()
    for x in contents:
        if x == 'A':
            num[0] += 1
        elif x == 'B':
            num[1] += 1
        elif x == 'C':
            num[2] += 1
        elif x == 'D':
            num[3] += 1
        elif x == 'E':
            num[4] += 1
        elif x == 'F':
            num[5] += 1
        else:
            continue
        
a = sum(num)
for i in range(len(num)):
    num[i] /= a
print(num)



