def read_file():
    with open('apps/twh2/web/deposit.html', 'rb') as fd:
        for line in fd:
            for sub_line in line.split(b'\r'):
                # Take into account any single '\r'
                # if not sub_line:
                #     # If you want to deal also with zero length groups, thsi must be improved...
                #     continue
                # Do something with the lines
                print(sub_line)
                # self.wfile.write(bytes(sub_line, 'utf-8'))
                # .wfile.write(sub_line)

# read_file()

file = open('apps/twh2/web/deposit.html', 'r', encoding='utf-8')
xx = file.read()
print(xx)