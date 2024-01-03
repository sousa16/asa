from pulp import *
import time

start_time = time.time()
end_time = time.time()

# Read the first line for additional input
num_toys, num_special_packages, max_toys_per_day = map(int, input().split())

# Read toy data
toy_data = {}
for i in range(1, num_toys + 1):
    profit, capacity = map(int, input().split())
    toy_data[i] = (profit, capacity)

# Read special packages
special_packages = {}
index = 0
for _ in range(num_special_packages):
    package_info = list(map(int, input().split()))
    toys_ids = package_info[:-1]
    package_profit = package_info[-1]
    
    sum = 0
    for id in toys_ids:
        sum += toy_data[id][0]
    
    if (package_profit > sum):
        index += 1
        special_packages[index] = (toys_ids, package_profit)
        
# Create a linear programming problem
model = LpProblem("Toy_Maximization", LpMaximize)

# Decision variables for individual toys
toys_vars = LpVariable.dicts("Toy", toy_data.keys(), lowBound=0, cat="Integer")

# Decision variables for special packages
special_vars = LpVariable.dicts("Special_Package", special_packages.keys(), lowBound=0, cat="Integer")

# Objective Function: Maximize total profit
model += lpSum(special_packages[i][1] * special_vars[i] for i in special_packages) + lpSum(toy_data[i][0] * toys_vars[i] for i in toy_data)

# Constraint 1: Total toys produced should not exceed the maximum daily capacity
model += lpSum(toys_vars[i] for i in toy_data) + lpSum(3 * special_vars[i] for i in special_packages) <= max_toys_per_day, "Total_Toy_Capacity"

# Constraint 2: Individual toy capacity constraint
for i in toy_data:
    model += toys_vars[i] + lpSum(special_vars[j] for j in special_packages if i in special_packages[j][0]) <= toy_data[i][1], f"Individual_Toy_Capacity_{i}"

# Constraint 3: Individual toy capacity constraint in special packages
for i, (toys_in_package, _) in special_packages.items():
    for toy in toys_in_package:
        model += special_vars[i] <= min(toy_data[toy][1] for toy in toys_in_package), f"Special_Package_Individual_Toy_Capacity_{i}_{toy}"

# Solve the problem
model.solve(GLPK(msg=False))

# Display the results
print(model.objective.value())
print(f"Runtime: {end_time - start_time} seconds")