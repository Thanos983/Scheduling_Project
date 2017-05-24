import schedule

file = 'test.txt'
activities = schedule.open_file(file)

print(schedule.can_be_undone(activities=activities, path=' '))
