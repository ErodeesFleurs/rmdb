import random
import string

def generate_random_name(length=8):
    return ''.join(random.choices(string.digits + string.ascii_letters, k=length))

def main():
    table_name = "warehouse"
    num_records = 3000

    # 文件名
    output_file = "warehouse_sql_statements.sql"
    
    with open(output_file, "w") as file:
        # 生成建表语句
        create_table_sql = f"create table {table_name} (w_id int, name char(8));\n"
        file.write(create_table_sql)

        # 生成插入语句
        insert_sql_list = []
        name_set = set()
        for w_id in range(1, num_records + 1):
            while True:
                name = generate_random_name()
                if name not in name_set:
                    name_set.add(name)
                    break
            insert_sql_list.append(f"insert into {table_name} values({w_id}, '{name}');\n")
        
        for insert_sql in insert_sql_list:
            file.write(insert_sql)
        
        # 生成选择语句
        select_sql_list = []
        for w_id in range(1, num_records + 1):
            select_sql_list.append(f"select * from {table_name} where w_id = {w_id};\n")

        for select_sql in select_sql_list:
            file.write(select_sql)
        
        # 生成创建索引语句
        create_index_sql = f"create index {table_name}(w_id);\n"
        file.write(create_index_sql)
        
        # 生成再次选择语句
        for select_sql in select_sql_list:
            file.write(select_sql)

if __name__ == "__main__":
    main()
