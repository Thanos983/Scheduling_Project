# Creates random test cases(graphs) in order to test the IFM theory
import random
import string

MINIMUM_NUMBER_OF_NODES = 2
MAXIMUM_NUMBER_OF_NODES = 10  # maximum available number of nodes is 52 but we use 10 for convenience
UPPER_LETTERS = string.ascii_uppercase + string.ascii_lowercase
NUMBER_OF_NODES = random.randint(MINIMUM_NUMBER_OF_NODES, MAXIMUM_NUMBER_OF_NODES)
LIKEHOOD_OF_CONNECTION = 0.3


def create_random_activities():
    """
    Create a dictionary alphabetically ordered with the activities
    and add period for each one
    e.g. {'A':[1,], 'B':[3,]}
    :return: activities as dictionary
    """

    activities = {}

    for i in range(NUMBER_OF_NODES):
        activities[UPPER_LETTERS[i]] = [random.randint(1, 4)]

    return activities


def add_random_connections(activities):
    """
    For each node, it connects it with another nodes with a possibility p in order to create a random graph
    :param activities:
    :return: none
    """
    # iterate through activities backwards in order to avoid cyclic graph
    for activity in sorted(activities.keys(), reverse=True):

        if activity == 'A':  # Because A will not connect to anything if we don't break it will create a infinite loop
            break

        connected = False
        while not connected:
            for connection in sorted(activities.keys(), reverse=True):
                if activity > connection:  # Check only the previous activities from the the current activity
                    possibility = random.random()  # random number on [0,1]

                    if possibility <= LIKEHOOD_OF_CONNECTION:
                        activities[activity].append(connection)
                        connected = True


# def draw_graph(activities):
#     graph = nx.DiGraph()
#     nx.draw(graph)
#     mpl.show(graph)


def main():
    activities = create_random_activities()
    add_random_connections(activities)
    print(activities)

if __name__ == '__main__':
    main()