# very simple script to calculate triangle connections of cuboid objects

list = [0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7,]

number_of_vertices = 8      # 8 vertices per cuboid
offset = 4                  # offset in final list calculation

final_list = []

for element in list:
    final_list.append(element + (number_of_vertices * offset))
    
print(final_list)