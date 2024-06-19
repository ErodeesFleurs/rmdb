def generate_sql():
    # Create table SQL
    sql_statements = []
    sql_statements.append("create table warehouse (w_id int, name char(8), flo float);")

    # Insert statements with unique flo values
    flo_value = 1024.5
    for i in range(1, 3001):
        name = f"{i:08d}"[:8]  # Generate a name with exactly 8 characters
        sql_statements.append(f"insert into warehouse values({i}, '{name}', {flo_value:.1f});")
        flo_value -= 0.1  # Ensure unique flo values

    # Select statements before creating index
    flo_value = 1024.5
    for i in range(1, 3001):
        sql_statements.append(f"select * from warehouse where w_id = {i} and flo = {flo_value:.6f};")
        flo_value -= 0.1

    # Create unique index SQL
    sql_statements.append("create unique index idx_warehouse on warehouse(w_id, flo);")

    # Select statements after creating index
    flo_value = 1024.5
    for i in range(1, 3001):
        sql_statements.append(f"select * from warehouse where w_id = {i} and flo = {flo_value:.6f};")
        flo_value -= 0.1

    return "\n".join(sql_statements)

# Generate SQL and write to file
sql_script = generate_sql()

with open("warehouse_script.sql", "w") as file:
    file.write(sql_script)

print("SQL script has been written to warehouse_script.sql")
