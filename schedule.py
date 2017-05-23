# Title: Financial driven project scheduling: A dynamic programming approach
#
# Part A: Calculates all the possible paths
# 		and stores them in a linked list
# 		Input:A txt file with the names of the nodes and all the prerequicities
# 		Output: All the possible paths
#
# 		The names of the activities in the input file should be one letter A-Z or a-z
# 		This code supports maximum 52 activities.After that with sumbol : follows
# 		the period time of the activity and afterwords follows all the prerequicities
# 		of the given activity seperated by comma
#
# 		Example of the format given bellow:
#
# 			A:2/
# 			B:1/A
# 			C:1/A
# 			D:3/B
# 			a:2/A,B,D
# 			b:2/A,B,a
from math import floor
import json


def open_file(file):
    """
    Accepts the name of the file
    and returns a dictionary with the activities
    input:
        file:String
    output:
        activities: Dictionary
    """
    activities = {}
    with open(file, 'r') as f:
        for line in f:
            line = line.split()
            activities[line[0]] = line[1::]

    return activities


def create_npv():
    """Creates the Net Present Value array based on cashflows.txt"""

    cashflow_file = "cashflows.txt"
    cashflow = list()

    # Read the file and save it to a list as integers
    with open(cashflow_file) as file:
        for line in file:
            cashflow.append(list(int(x) for x in line.split()))

    discount_factor = 0.005
    temporary_cash = list()
    temp = list()

    # for every column in list cashflow
    for row in range(len(cashflow)):
        temp = list()
        for column in range(len(cashflow[row])):
            summary = 0
            for t in range(len(cashflow[row]) - column):
                summary += cashflow[row][t] / (1+discount_factor)**(t+1)

            summary *= 1/(discount_factor+1)**column
            # TODO: there is a +-0.1 difference in expected values
            temp.append(floor(summary*10)/10)

        temporary_cash.append(temp)

    return temporary_cash


def create_paths(activities, path):
    """
    Checks what activities can be done for a given path
    :param activities: Dictionary with the activities
    :param path: the current path
    :return: a list with all the possible paths for a given path
    """
    possible_paths = []

    for key in sorted(activities.keys(), reverse=True):
        can_be_done = 0
        if key not in path and key > path[-1]:
            for prereq in activities[key][1::]:
                if prereq not in path:
                    can_be_done = 1
                    break

            if can_be_done == 0:
                if path.startswith(' '):
                    new = key
                else:
                    new = path + key

                possible_paths.append(new)

    return possible_paths


def can_be_undone(activities, path):

    possible_activities = []

    for key in activities.keys():
        if key not in path:
            done = 0
            #  for all prerequisites
            for prereq in activities[key][1::]:
                if prereq not in path:
                    done = 1
                    break

            if done == 0:  # can be done
                possible_activities.append(key)

    return possible_activities


def calculate_start_time(path, activities):
    """ Finds the total period of a path to be constructed"""

    start_time = 0
    for activity in path:
        if activity in activities.keys():
            start_time += int(activities[activity][0])

    return start_time


def find_best_solution(nodes, activities):

    npv = create_npv()
    npv_next_stages = []

    state_out = ''
    npv_dict = {}

    nodes.insert(0, [])

    # Beginning from the last node until the first
    for i in range(nodes.__len__()-1, -1, -1):
        for path in nodes[i]:
            possible_paths = can_be_undone(activities, path)
            # print(path, possible_paths)

            for letter in possible_paths:
                state_out = ''.join(sorted(path + letter))
                time = calculate_start_time(path, activities)
                current_value = npv[ord(letter) - ord('A')][time]

                # For each state out we check if exists and if has a greater value
                if path in npv_dict.keys():
                    value = npv_dict[state_out][5] + current_value

                    if value > npv_dict[path][5]:
                        npv_dict[path] = [letter, time, current_value, state_out, npv_dict[state_out][5], value]

                else:
                    value = current_value
                    npv_dict[path] = [letter, time, current_value, state_out, "No Previous Stage",  value]

        # npv_next_stages.append(npv_dict)

    return npv_dict


def main():
    # TODO: The code bellow could be inserted in a function!

    file = 'test.txt'
    activities = open_file(file)

    nodes = list()
    nodes.append(create_paths(activities, ' '))  # Insert the first activities

    for i in range(activities.__len__()-1):
        new_paths = []
        for paths in nodes[i]:
            new_paths += create_paths(activities, paths)

        nodes.append(new_paths)

    npv_next_stages = find_best_solution(nodes, activities)

    for key in npv_next_stages:
        print(key)

    with open("test.json", "w") as f:
        json.dump(npv_next_stages, f, indent=4)

if __name__ == '__main__':
    main()
