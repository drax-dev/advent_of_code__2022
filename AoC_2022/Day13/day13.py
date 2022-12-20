import ast
from functools import cmp_to_key


def compare_lists(left_list, right_list):
    min_len = min(len(left_list), len(right_list))
    for i in range(0, min_len):
        left_value = left_list[i]
        right_value = right_list[i]
        if type(left_value) is int and type(right_value) is int:
            if left_value < right_value:
                return 1
            elif left_value > right_value:
                return -1
            else:
                continue
        if type(left_value) is int:
            left_value = [left_value]
        if type(right_value) is int:
            right_value = [right_value]

        result = compare_lists(left_value, right_value)
        if result != 0:
            return result

    if len(left_list) < len(right_list):
        return 1
    if len(left_list) > len(right_list):
        return -1
    return 0


def load_input(file_name):
    with open(file_name, mode="r") as file_handle:
        line_list = list(file_handle)
        sum_of_indexes = 0.0
        len_line_list = len(line_list)
        for i in range(0, len_line_list, 3):
            l_res = ast.literal_eval(line_list[i])
            print("left list", l_res)
            r_res = ast.literal_eval(line_list[i + 1])
            print("right list", r_res)
            result = compare_lists(l_res, r_res)
            if result == 1:
                sum_of_indexes += (i / 3) + 1
            print("result", result)
        print("Sum =", sum_of_indexes)


def load_input_part2(file_name):
    result_list = list()
    with open(file_name, mode="r") as file_handle:
        line_list = list(file_handle)
        len_line_list = len(line_list)
        for i in range(0, len_line_list - 1, 3):
            l_res = ast.literal_eval(line_list[i])
            print("left list", l_res)
            result_list.append(l_res)
            r_res = ast.literal_eval(line_list[i + 1])
            print("right list", r_res)
            result_list.append(r_res)
        result_list.append([[2]])
        result_list.append([[6]])
        result_list = sorted(result_list, key=cmp_to_key(compare_lists))
        result_list.reverse()
    return result_list


if __name__ == '__main__':
    # load_input('day13_input.txt')

    results = load_input_part2('day13_input.txt')
    first_index = results.index([[2]]) + 1
    second_index = results.index([[6]]) + 1
    final_result = first_index * second_index
    print(results)
    print("first index and second index", first_index, second_index)
    print("final result is", final_result)
