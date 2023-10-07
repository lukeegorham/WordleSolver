with open('words.txt', 'r') as f:
    words = []
    index = 0
    line = f.readline()
    while line is not "":
        words.append(line)
        index = index + 1
        line = f.readline()
    print(f"{index} words read...")
    words.sort()
    print(f"{index} words sorted...")
    with open('sorted.txt', 'w') as out:
        for line in words:
            out.write(line)
        print(f"{index} words written...")
    f.close()
    print("Finished!!!")
