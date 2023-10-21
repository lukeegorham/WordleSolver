# num = input("Enter number of letters in word : ")
for num in range(5, 13):
    with open('linux.words', 'r') as f:
        words = []
        index = 0
        line = ""
        while True:
            prev_line = line
            line = f.readline()
            if line == "":
                break
            if len(line)-1 != int(num):
                continue   # only include properly sized words
            if line.lower() != line:
                continue   # exclude proper nouns
            if line == prev_line:
                continue   # don't add duplicate values
            if line[len(line)-3] == '\'':
                continue   # don't add contractions
            words.append(line)
            index = index + 1
        # print(f"{index} words read...")
        # words.sort()
        # print(f"{index} words sorted...")
        with open('../Dictionaries/' + str(num) + '.txt', 'w') as out:
            for line in words:
                out.write(line)
            print(f"{index} words written to " + str(num) + ".txt")
        f.close()
