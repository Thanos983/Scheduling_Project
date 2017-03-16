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


def create_paths(activities, path):
    """
    Checks what activities can be done for a given path
    :param activities: Dictionary with the activities
    :param path: the current path
    :return: a list with all the possible paths for a given path
    """
    possible_paths = []

    for key in activities.keys():
        can_be_done = 0
        if key not in path and key > path[-1]:
            for prereq in activities[key][1::]:
                if prereq not in path:
                    can_be_done = 1
                    break

            if can_be_done == 0:
                new = path + key
                possible_paths.append(new)

    return possible_paths


def main():
    file = 'test.txt'
    activities = open_file(file)

    nodes = []
    nodes.append(create_paths(activities, ' '))  # Insert the first activities
    print(nodes)

    for i in range(activities.__len__()):
        new_paths = []
        for paths in nodes[i]:
            print(paths)
            new_paths += create_paths(activities, paths)

        nodes.append(new_paths)

    print(nodes)
if __name__ == '__main__':
    main()
