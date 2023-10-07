with open('sorted.txt', 'r') as f:
    words = []
    index = 0
    line = f.readline()
    while line != "":
        line = line.lower()
        words.append(line)
        index = index + 1
        line = f.readline()
    print(f"{index} words read...")
    with open('sorted.txt', 'w') as out:
        for line in words:
            out.write(line)
        print(f"{index} words written...")
    f.close()
    print("Finished!!!")
