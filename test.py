import schedule

file = 'test.txt'
activities = schedule.open_file(file)

print(schedule.calculate_start_time('ABCDEFGI', activities=activities))
