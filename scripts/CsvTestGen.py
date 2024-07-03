import csv
import os
import re


def parse(csv_name):

    # csv_name = 'order_line.csv'
    txt_name = csv_name.replace("csv", "sql")

    with open(txt_name, "w") as f:
        with open(csv_name, "r") as d:
            is_header = 1
            name = []
            for row in csv.reader(d, skipinitialspace=True):
                if is_header == 1:
                    name = row
                    print(name)
                    is_header = 2
                elif is_header == 2:
                    type = []
                    for i in row:
                        if any(char.isalpha() for char in i):
                            type.append("char(" + str(len(i)) + ")")
                        elif ":" in i and "-" in i:
                            type.append("datetime")
                        elif "." in i:
                            type.append("float")
                        else:
                            type.append("int")

                    table_name = csv_name.split(".")[0]
                    f.write(f"create table {table_name} (")
                    for i in range(len(name)):
                        if i != len(name) - 1:
                            f.write(name[i] + " " + type[i] + ", ")
                        else:
                            f.write(name[i] + " " + type[i] + ");\n")

                    values = ""
                    for k, i in enumerate(row):
                        if type[k] == "datetime" or "char" in type[k]:
                            values += "'" + i + "'"
                        else:
                            values += i
                        if k != len(row) - 1:
                            values += ", "
                    f.write(f"insert into {table_name} values(" + values + ");\n")

                    is_header = 0

                else:
                    values = ""
                    for k, i in enumerate(row):
                        if type[k] == "datetime":
                            values += "'" + i + "'"
                        elif "char" in type[k]:
                            length = int(type[k].split("(")[-1].split(")")[0])
                            values += "'" + i[:length] + "'"
                        else:
                            values += i
                        if k != len(row) - 1:
                            values += ", "
                    f.write(f"insert into {table_name} values(" + values + ");\n")


for file in os.listdir(os.getcwd()):
    if ".csv" in file:
        parse(file)

sql = []
for file in os.listdir(os.getcwd()):
    if ".sql" in file:
        with open(file, "r") as f:
            sql += f.readlines()

with open("all.sql", "w") as f:
    for i in sql:
        f.write(i)
