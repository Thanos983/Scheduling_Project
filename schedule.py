# Scheduling problem rewritten in Python

file = 'test.txt'
activities = {}

with open(file, 'r') as f:
    for line in f:
        line = line.split()
        activities[line[0]] = line[1::]


def can_activity_be_done(activities, path):

    for a in activities.keys():
        if a not in path:
            for prereq in a[1::]:
                if prereq not in path:
                    return False

            return True  # it can be done
